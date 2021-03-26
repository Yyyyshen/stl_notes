#pragma once

/**
 * deque理解
 * 由一段段定量连续空间，串接在整个deque头或者尾
 * 实现的最大任务就是在这些分段的连续空间上维护其整体连续的假象，并提供随机存取接口
 * 因此，数据结构设计和迭代器前进后退操作都很繁琐
 * 采用一块map（不是map容器）作为主控，这里指的是一块连续空间，每个元素（node）都是指针
 * 指向另一段较大的连续线性空间，称为缓冲区，该缓冲区是deque的存储主体
 */

inline size_t __y_deque_buff_size(size_t n, size_t sz) {
	//计算缓冲区容纳大小
	//n不为0则表示由使用者定义为n；不为0时，验证元素大小，小于512传回512/sz，大于等于512返回1
	//例如，创建一个deqeu<int>并令其缓冲区尾32字节，则每块缓冲区容纳32/sizeof(int)个元素
	return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
}

//迭代器设计：必须能够指出分段连续空间在哪，其次必须能够判断自己是否处在缓冲区边缘，若是则前进后退操作需要跳跃至另一个缓冲区
template<class T, class Ref, class Ptr, size_t BufSiz>
struct __y_deque_iterator {
	typedef __y_deque_iterator<T, T&, T*, BufSiz> iterator;
	typedef __y_deque_iterator<T, const T&, const T*, BufSiz> const_iterator;
	static size_t buffer_size() { return __y_deque_buff_size(BufSiz, sizeof(T)); }
	//未继承std::iterator，需要自行撰写五个迭代器型别
	//typedef y_random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T** map_pointer;
	typedef __y_deque_iterator self;
	//与容器的联系
	T* cur;				//缓冲区current元素
	T* first;			//缓冲区头
	T* last;			//缓冲区尾
	map_pointer node;	//指向管理缓冲区的map

	/*
						  |———》------------------------------------
						  |		   |	|	|	|	|	|	|  ...	  |  buffer
						  |		   ------------------------------------
						  |		   |					|			  |
						  |		   |					|			  |
						  |		   |--------------------|-------|	  |
						  |								|		|	  |
						  |							 -----------------------------
	map					  |							 | cur | first | last | node |  iterator
	  ↘				  |							 -----------------------------
		  --------------------------------									  |
		  |		|	|	| * |	|	|	|									  |
		  --------------------------------									  |
						 node<------------------------------------------------|

								管理器、缓冲区、迭代器的关系
	*/

	//跳缓冲区
	void set_node(map_pointer new_node) {
		node = new_node;
		first = *new_node;//新缓冲区首地址
		last = first + difference_type(buffer_size());
	}
	//各运算符重载
	reference operator*() const { return *cur; } //返回当前指向元素的值
	pointer operator->() const { return &(operator*()); } //指向元素的地址
	difference_type operator-(const self& x) const {
		return difference_type(buffer_size()) * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
	}
	self& operator++() {
		//前置++
		++cur; //切换至下一个元素
		if (cur == last)
		{
			//如果达到尾端，则切换缓冲区，指向下一个节点的第一个元素
			set_node(node + 1);
			cur = first;
		}
		return *this;
	}
	self operator++(int) {
		//后置++（另外提一句，后置++和前置的区分就是，后置接收一个int型参数，这只是一个规定，没什么理由，参数也不会被使用）
		self temp = *this;
		++(*this);
		return temp;
	}
	self& operator--() {
		//前置--，原理同++
		if (cur == first)
		{
			//如果当前指向的是本节点第一个元素，则切换至上一个缓冲区最后一个元素
			set_node(node - 1);
			cur = last;
		}
		--cur;
		return *this;
	}
	self operator--(int) {
		//后置--，标准写法
		self temp = *this;
		--(*this);
		return temp;
	}
	//随机存取，跳跃n个距离
	self& operator+=(difference_type n) {
		difference_type offset = n + (cur - first);
		if (offset >= 0 && offset < difference_type(buffer_size()))
		{
			//目标位置还在本缓冲区内
			cur += n;
		}
		else
		{
			//目标位置超出范围，先计算节点偏移（并且这里其实已经用正负逻辑实现了+和-，后面都可以直接复用）
			difference_type node_offset = offset > 0 ? offset / difference_type(buffer_size()) : -difference_type((-offset - 1) / buffer_size()) - 1;
			//切换至正确节点
			set_node(node + node_offset);
			//计算新缓冲区中的偏移位置并定位
			cur = first + (offset - node_offset * difference_type(buffer_size()));
		}
		return *this;
	}
	self operator+(difference_type n) const {
		self temp = *this;
		return temp += n;//复用+=
	}
	self& operator-=(difference_type n) {
		return *this += -n;//加负号复用+=
	}
	self operator-(difference_type n) const {
		self temp = *this;
		return temp -= n;//复用-=
	}
	//随机存取，用[]运算符
	reference operator[](difference_type n) const {
		return *(*this + n);//其实还是复用了上面的运算符，跳跃了n个距离
	}
	//比较运算符
	bool operator==(const self& x) const { return cur == x.cur; }
	bool operator!=(const self& x) const { return !(*this == x); }
	bool operator<(const self& x) const { return (node == x->node) ? (cur < x.cur) : (node < x.node); }//同缓冲区比值，不同缓冲区比节点
};

//deque结构，维护指向map的指针以及start和finish两个迭代器
template<class T, class Alloc = y_alloc, size_t BufSiz = 0>
class y_deque {
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

protected:
	typedef pointer* map_pointer;

protected:
	map_pointer map;//相当于T** ，是指向一小块连续空间的指针，小块连续空间内的每个元素又都是指向另一较大块缓冲区的指针
	size_type map_size;//map内可容纳多少指针

public:
	typedef __y_deque_iterator<T, T&, T*, BufSiz> iterator;
protected:
	iterator start;
	iterator finish;

	//...

public:
	iterator begin() { return start; };
	iterator end() { return finish; };
	//调用了iterator::operator操作符重载实现以下许多函数操作
	reference operator[](size_type n) {
		return start[difference_type(n)];
	}
	reference front() { return *start; }
	reference back() {
		iterator temp = finish;
		--temp;
		return *temp;
	}
	size_type size() const /* 题外话，函数定义后面跟const，表示该函数不能修改此对象的成员变量 */ { return finish - start;; }
	size_type max_size() const { return size_type(-1); }
	bool empty() const { return finish == start; }

protected:
	//内存管理
	typedef y_simple_alloc<value_type, Alloc> data_allocator;//专属空间配置器，服务于每个元素
	typedef y_simple_alloc<pointer, Alloc> map_allocator;//专属空间配置器，服务于map

	void fill_initialize(size_type n, const value_type& value) {
		create_map_and_nodes(n);//安排好map结构
		map_pointer cur;
		try {
			for (cur = start.node; cur < finish.node; ++cur)
				uninitialized_fill(*cur, *cur + iterator::buffer_size(), value);
			uninitialized_fill(finish.first, finish.cur, value);
		}
		catch (...)
		{
			//...
		}
	}
	void create_map_and_nodes(size_type num_elements) {
		//需要节点数=元素个数/每个缓冲区可容纳元素个数+1，刚好整除就是多配了一个缓冲区
		size_type num_nodes = num_elements / iterator::buffer_size() + 1;
		//一个map管理的缓冲区数量，最少8个，最多为所需节点数+2（前后预留一个，扩充时用）
		map_size = max(initial_map_size(), num_nodes + 2);
		map = map_allocator::allocate(map_size);
		//以下令nstart和nfinish指向map所拥有的全部节点最中央区段，目的是使头尾两端扩充能量一样大，每个节点可对应一个缓冲区
		map_pointer nstart = map + (map_size - num_nodes) / 2;
		map_pointer nfinish = nstart + num_nodes - 1;
		map_pointer cur;
		try {
			for (cur = nstart; cur <= finish; ++cur)
				*cur = allocate_node();
		}
		catch (...) {
			//...
		}
		//为迭代器赋值
		start.set_node(nstart);//设置对应节点
		finish.set_node(nfinish);
		start.cur = start.first;//设置当前指向，头尾分别指向第一个和最后一个元素
		finish.cur = finish.first + num_elements % iterator::buffer_size();
	}
	size_type initial_map_size() const { return 8; }
	void reserve_map_at_back(size_type nodes_to_add = 1) {
		if (nodes_to_add + 1 > map_size - (finish.node - map))
			//map尾端节点备用空间不足，换一个map（同vector，配置更大的，拷贝原来的，释放原来的）
			reallocate_map(nodes_to_add, false);
	}
	void reserve_map_at_front(size_type nodes_to_add = 1) {
		if (nodes_to_add > start.node - map)
			//map前端节点备用空间不足，换一个map（同vector，配置更大的，拷贝原来的，释放原来的）
			reallocate_map(nodes_to_add, true);
	}
	void reallocate_map(size_type nodes_to_add, bool add_at_front) {
		size_type old_num_nodes = finish.node - start.node + 1;
		size_type new_num_nodes = old_num_nodes + nodes_to_add;
		map_pointer new_nstart;
		if (map_size > 2 * new_num_nodes)
		{
			new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
			if (new_nstart < start.node)
				copy(start.node, finish.node + 1, new_nstart);
			else
				copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
		}
		else
		{
			size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;//一样是成倍扩容或扩容所需大小，并在首尾各保留一个
			//配置新空间给新map
			map_pointer new_map = map_allocator::allocate(new_map_size);
			new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
			//拷贝原map内容
			copy(start.node, finish.node + 1, new_nstart);
			//释放原map
			map_allocator::deallocate(map, map_size);
			//设置新map
			map = new_map;
			map_size = new_map_size;
		}
		//重新设定迭代器
		start.set_node(new_nstart);
		finish.set_node(new_nstart + old_num_nodes - 1);
	}
	void allocate_node() { /* todo */ }
	void deallocate_node() { /* todo */ }
public:
	//构造
	y_deque(int n, const value_type& value) :start(), finish(), map(0), map_size(0) { fill_initialize(n, value); }
	//结尾插入
	void push_back(const value_type& t) {
		if (finish.cur != finish.last - 1)
		{
			//当前指向的还不是最后一个缓冲区的最后一个元素
			construct(finish.cur, t);//直接在当前位置构造元素
			++finish.cur;//调整指向
		}
		else
			push_back_aux(t);//只剩一个元素备用空间
	}
	void push_back_aux(const value_type& t) {
		value_type t_copy = t;
		reserve_map_at_back();//在某些条件下，必须重新换一个map
		*(finish.node + 1) = allocate_node();//配置一个新缓冲区
		try {
			construct(finish.cur, t_copy);
			//设定finish迭代器指向的节点和位置
			finish->set_node(finish.node + 1);
			finish.cur = finish.first;
		}
		catch (...) {
			//...
		}
	}
	//头插入
	void push_front(const value_type& t) {
		if (start.cur != start.first)
		{
			//第一缓冲区前尚有备用空间
			construct(start.cur - 1, t);
			--start.cur;
		}
		else
			push_front_aux(t);//无备用空间
	}
	void push_front_aux(const value_type& t) {
		value_type t_copy = t;
		reserve_map_at_front();//某些条件下必须换一个map
		//配置新缓冲区
		*(start.node - 1) = allocate_node();
		try {
			start.set_node(start.node - 1);
			start.cur = start.last - 1;
			construct(start.cur, t_copy);
		}
		catch (...) {
			//计算基本不会异常，最多是构造时发生，由于这个顺序，发生异常要恢复迭代器状态
			start.set_node(start.node + 1);
			start.cur = start.first;
			deallocate_node(*(start.node - 1));
			throw;
		}
	}
	//尾删除
	void pop_back() {
		if (finish.cur != finish.first)
		{
			//最后一个缓冲区有一个及以上元素
			--finish.cur;//调整指针
			destroy(finish.cur);//析构元素
		}
		else
			pop_back_aux();//最后一个缓冲区无元素
	}
	void pop_back_aux() {
		deallocate_node(finish.first);//释放最后一个缓冲区
		finish.set_node(finish.node - 1);//调整迭代器状态
		finish.cur = finish.last - 1;
		destroy(finish.cur);//析构元素
	}
	//头删除
	void pop_front() {
		if (start.cur != start.last - 1)
		{
			//第一个缓冲有一个以上元素
			destroy(start.cur);//析构元素
			++start.cur;//调整迭代器
		}
		else
			pop_front_aux();//第一缓冲区只剩一个元素
	}
	void pop_front_aux() {
		destroy(start.cur);//析构第一缓冲区最后一个元素
		deallocate_node(start.first);//释放缓冲区
		start.set_node(start.node + 1);//调整迭代器
		start.cur = start.first;
	}
	//清除deque，需要注意，deque最初状态（即使无任何元素）保有一个缓冲区，因此clear后，也需要保留一个
	void clear() {
		for (map_pointer node = start.node + 1; node < finish.node; ++node)
		{
			destroy(*node, *node + iterator::buffer_size());//析构所有元素
			data_allocator::deallocate(*node, iterator::buffer_size());//释放缓冲区内存
		}
		if (start.node != finish.node)
		{
			//释放后有头尾两个缓冲区，则析构所有元素后释放尾缓冲区，保留头缓冲区
			destroy(start.cur, start.last);
			destroy(finish.first, finish.cur);
			data_allocator::deallocate(finish.first, iterator::buffer_size());
		}
		else //只有一个缓冲区，只释放元素，保留缓冲区
			destroy(start.cur, finish.cur);
		finish = start;
	}
	//根据迭代器清除某个元素
	iterator erase(iterator pos) {
		iterator next = pos;
		++next;
		difference_type index = pos - start;//删除点之前元素个数
		if (index < (size() >> 1))
		{
			//看清楚点前后那边需要移动的元素较少
			copy_backward(start, pos, next);
			pop_front();
		}
		else
		{
			copy(next, finish, pos);
			pop_back();//移动后去除多余元素
		}
		return start + index;
	}
	//根据迭代器清除某个段的元素
	iterator erase(iterator first, iterator last) {
		if (first == start && last ==  finish)
		{
			//要删除区间就是整个deque，直接复用clear()
			clear();
			return finish;
		}
		else
		{
			difference_type n = last - first;//清除区间长度
			difference_type elems_before = first - start;//比较清除区间前方元素个数
			if (elems_before < (size() - n) / 2)
			{
				//那边少就移动哪边，之后去除多余元素，释放冗余缓冲区
				copy_backward(start, first, last);
				iterator new_start = start + n;
				destroy(start, new_start);
				for (map_pointer cur = start.node; cur < new_start.node; ++cur)
					data_allocator::deallocate(*cur, iterator::buffer_size());
				start = new_start;//设置新起点
			}
			else
			{
				//清除区间后方元素较少
				copy(last, finish, first);
				iterator new_finish = finish - n;
				destroy(new_finish, finish);
				for (map_pointer cur = new_finish + 1; cur <= finish.node; ++cur)
					data_allocator::deallocate(*cur, iterator::buffer_size());
				finish = new_finish;
			}
			return start + elems_before;
		}
	}
	//在某处插入一个元素（某个点之前）
	iterator insert(iterator position, const value_type& x) {
		if (position.cur == start.cur) //插入点为最前端，复用push_front;
		{
			push_front(x);
			return start;
		}
		if (position.cur == finish.cur) //插入点为尾端，复用push_back;
		{
			push_back(x);
			iterator temp = finish;
			--temp;
			return temp;
		}
		return insert_aux(position, x);
	}
	iterator insert_aux(iterator pos, const value_type& x) {
		difference_type index = pos - index;//计算插入点之前元素
		//同删除一样，比较插入点前后元素个数，移动较少的那边
		value_type x_copy = x;
		if (index < size() / 2)
		{
			push_front(front());//先往最前端插入一个元素，可以复用备用空间不够的情况
			iterator front1 = start;
			++front1;
			iterator front2 = front1;
			++front2;
			pos = start + index;
			iterator pos1 = pos;
			++pos1;
			copy(front2, pos1, front1);//元素移动，把原第二个元素到插入点的元素前移一位
		}
		else
		{
			push_back(back());
			iterator back1 = finish;
			--back1;
			iterator back2 = back1;
			--back2;
			pos = start + index;
			copy_backward(pos, back2, back1);//将插入点到原倒数第二个元素位置元素后移一位
		}
		*pos = x_copy;//移动后将插入值设定
		return pos;
	}
};