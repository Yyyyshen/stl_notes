#pragma once

/**
 * deque���
 * ��һ�ζζ��������ռ䣬����������dequeͷ����β
 * ʵ�ֵ���������������Щ�ֶε������ռ���ά�������������ļ��󣬲��ṩ�����ȡ�ӿ�
 * ��ˣ����ݽṹ��ƺ͵�����ǰ�����˲������ܷ���
 * ����һ��map������map��������Ϊ���أ�����ָ����һ�������ռ䣬ÿ��Ԫ�أ�node������ָ��
 * ָ����һ�νϴ���������Կռ䣬��Ϊ���������û�������deque�Ĵ洢����
 */

inline size_t __y_deque_buff_size(size_t n, size_t sz) {
	//���㻺�������ɴ�С
	//n��Ϊ0���ʾ��ʹ���߶���Ϊn����Ϊ0ʱ����֤Ԫ�ش�С��С��512����512/sz�����ڵ���512����1
	//���磬����һ��deqeu<int>�����仺����β32�ֽڣ���ÿ�黺��������32/sizeof(int)��Ԫ��
	return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
}

//��������ƣ������ܹ�ָ���ֶ������ռ����ģ���α����ܹ��ж��Լ��Ƿ��ڻ�������Ե��������ǰ�����˲�����Ҫ��Ծ����һ��������
template<class T, class Ref, class Ptr, size_t BufSiz>
struct __y_deque_iterator {
	typedef __y_deque_iterator<T, T&, T*, BufSiz> iterator;
	typedef __y_deque_iterator<T, const T&, const T*, BufSiz> const_iterator;
	static size_t buffer_size() { return __y_deque_buff_size(BufSiz, sizeof(T)); }
	//δ�̳�std::iterator����Ҫ����׫д����������ͱ�
	//typedef y_random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T** map_pointer;
	typedef __y_deque_iterator self;
	//����������ϵ
	T* cur;				//������currentԪ��
	T* first;			//������ͷ
	T* last;			//������β
	map_pointer node;	//ָ�����������map

	/*
						  |��������------------------------------------
						  |		   |	|	|	|	|	|	|  ...	  |  buffer
						  |		   ------------------------------------
						  |		   |					|			  |
						  |		   |					|			  |
						  |		   |--------------------|-------|	  |
						  |								|		|	  |
						  |							 -----------------------------
	map					  |							 | cur | first | last | node |  iterator
	  �K				  |							 -----------------------------
		  --------------------------------									  |
		  |		|	|	| * |	|	|	|									  |
		  --------------------------------									  |
						 node<------------------------------------------------|

								�����������������������Ĺ�ϵ
	*/

	//��������
	void set_node(map_pointer new_node) {
		node = new_node;
		first = *new_node;//�»������׵�ַ
		last = first + difference_type(buffer_size());
	}
	//�����������
	reference operator*() const { return *cur; } //���ص�ǰָ��Ԫ�ص�ֵ
	pointer operator->() const { return &(operator*()); } //ָ��Ԫ�صĵ�ַ
	difference_type operator-(const self& x) const {
		return difference_type(buffer_size()) * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
	}
	self& operator++() {
		//ǰ��++
		++cur; //�л�����һ��Ԫ��
		if (cur == last)
		{
			//����ﵽβ�ˣ����л���������ָ����һ���ڵ�ĵ�һ��Ԫ��
			set_node(node + 1);
			cur = first;
		}
		return *this;
	}
	self operator++(int) {
		//����++��������һ�䣬����++��ǰ�õ����־��ǣ����ý���һ��int�Ͳ�������ֻ��һ���涨��ûʲô���ɣ�����Ҳ���ᱻʹ�ã�
		self temp = *this;
		++(*this);
		return temp;
	}
	self& operator--() {
		//ǰ��--��ԭ��ͬ++
		if (cur == first)
		{
			//�����ǰָ����Ǳ��ڵ��һ��Ԫ�أ����л�����һ�����������һ��Ԫ��
			set_node(node - 1);
			cur = last;
		}
		--cur;
		return *this;
	}
	self operator--(int) {
		//����--����׼д��
		self temp = *this;
		--(*this);
		return temp;
	}
	//�����ȡ����Ծn������
	self& operator+=(difference_type n) {
		difference_type offset = n + (cur - first);
		if (offset >= 0 && offset < difference_type(buffer_size()))
		{
			//Ŀ��λ�û��ڱ���������
			cur += n;
		}
		else
		{
			//Ŀ��λ�ó�����Χ���ȼ���ڵ�ƫ�ƣ�����������ʵ�Ѿ��������߼�ʵ����+��-�����涼����ֱ�Ӹ��ã�
			difference_type node_offset = offset > 0 ? offset / difference_type(buffer_size()) : -difference_type((-offset - 1) / buffer_size()) - 1;
			//�л�����ȷ�ڵ�
			set_node(node + node_offset);
			//�����»������е�ƫ��λ�ò���λ
			cur = first + (offset - node_offset * difference_type(buffer_size()));
		}
		return *this;
	}
	self operator+(difference_type n) const {
		self temp = *this;
		return temp += n;//����+=
	}
	self& operator-=(difference_type n) {
		return *this += -n;//�Ӹ��Ÿ���+=
	}
	self operator-(difference_type n) const {
		self temp = *this;
		return temp -= n;//����-=
	}
	//�����ȡ����[]�����
	reference operator[](difference_type n) const {
		return *(*this + n);//��ʵ���Ǹ�������������������Ծ��n������
	}
	//�Ƚ������
	bool operator==(const self& x) const { return cur == x.cur; }
	bool operator!=(const self& x) const { return !(*this == x); }
	bool operator<(const self& x) const { return (node == x->node) ? (cur < x.cur) : (node < x.node); }//ͬ��������ֵ����ͬ�������Ƚڵ�
};

//deque�ṹ��ά��ָ��map��ָ���Լ�start��finish����������
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
	map_pointer map;//�൱��T** ����ָ��һС�������ռ��ָ�룬С�������ռ��ڵ�ÿ��Ԫ���ֶ���ָ����һ�ϴ�黺������ָ��
	size_type map_size;//map�ڿ����ɶ���ָ��

public:
	typedef __y_deque_iterator<T, T&, T*, BufSiz> iterator;
protected:
	iterator start;
	iterator finish;

	//...

public:
	iterator begin() { return start; };
	iterator end() { return finish; };
	//������iterator::operator����������ʵ��������ຯ������
	reference operator[](size_type n) {
		return start[difference_type(n)];
	}
	reference front() { return *start; }
	reference back() {
		iterator temp = finish;
		--temp;
		return *temp;
	}
	size_type size() const /* ���⻰��������������const����ʾ�ú��������޸Ĵ˶���ĳ�Ա���� */ { return finish - start;; }
	size_type max_size() const { return size_type(-1); }
	bool empty() const { return finish == start; }

protected:
	//�ڴ����
	typedef y_simple_alloc<value_type, Alloc> data_allocator;//ר���ռ���������������ÿ��Ԫ��
	typedef y_simple_alloc<pointer, Alloc> map_allocator;//ר���ռ���������������map

	void fill_initialize(size_type n, const value_type& value) {
		create_map_and_nodes(n);//���ź�map�ṹ
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
		//��Ҫ�ڵ���=Ԫ�ظ���/ÿ��������������Ԫ�ظ���+1���պ��������Ƕ�����һ��������
		size_type num_nodes = num_elements / iterator::buffer_size() + 1;
		//һ��map����Ļ���������������8�������Ϊ����ڵ���+2��ǰ��Ԥ��һ��������ʱ�ã�
		map_size = max(initial_map_size(), num_nodes + 2);
		map = map_allocator::allocate(map_size);
		//������nstart��nfinishָ��map��ӵ�е�ȫ���ڵ����������Σ�Ŀ����ʹͷβ������������һ����ÿ���ڵ�ɶ�Ӧһ��������
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
		//Ϊ��������ֵ
		start.set_node(nstart);//���ö�Ӧ�ڵ�
		finish.set_node(nfinish);
		start.cur = start.first;//���õ�ǰָ��ͷβ�ֱ�ָ���һ�������һ��Ԫ��
		finish.cur = finish.first + num_elements % iterator::buffer_size();
	}
	size_type initial_map_size() const { return 8; }
	void reserve_map_at_back(size_type nodes_to_add = 1) {
		if (nodes_to_add + 1 > map_size - (finish.node - map))
			//mapβ�˽ڵ㱸�ÿռ䲻�㣬��һ��map��ͬvector�����ø���ģ�����ԭ���ģ��ͷ�ԭ���ģ�
			reallocate_map(nodes_to_add, false);
	}
	void reserve_map_at_front(size_type nodes_to_add = 1) {
		if (nodes_to_add > start.node - map)
			//mapǰ�˽ڵ㱸�ÿռ䲻�㣬��һ��map��ͬvector�����ø���ģ�����ԭ���ģ��ͷ�ԭ���ģ�
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
			size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;//һ���ǳɱ����ݻ����������С��������β������һ��
			//�����¿ռ����map
			map_pointer new_map = map_allocator::allocate(new_map_size);
			new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
			//����ԭmap����
			copy(start.node, finish.node + 1, new_nstart);
			//�ͷ�ԭmap
			map_allocator::deallocate(map, map_size);
			//������map
			map = new_map;
			map_size = new_map_size;
		}
		//�����趨������
		start.set_node(new_nstart);
		finish.set_node(new_nstart + old_num_nodes - 1);
	}
	void allocate_node() { /* todo */ }
	void deallocate_node() { /* todo */ }
public:
	//����
	y_deque(int n, const value_type& value) :start(), finish(), map(0), map_size(0) { fill_initialize(n, value); }
	//��β����
	void push_back(const value_type& t) {
		if (finish.cur != finish.last - 1)
		{
			//��ǰָ��Ļ��������һ�������������һ��Ԫ��
			construct(finish.cur, t);//ֱ���ڵ�ǰλ�ù���Ԫ��
			++finish.cur;//����ָ��
		}
		else
			push_back_aux(t);//ֻʣһ��Ԫ�ر��ÿռ�
	}
	void push_back_aux(const value_type& t) {
		value_type t_copy = t;
		reserve_map_at_back();//��ĳЩ�����£��������»�һ��map
		*(finish.node + 1) = allocate_node();//����һ���»�����
		try {
			construct(finish.cur, t_copy);
			//�趨finish������ָ��Ľڵ��λ��
			finish->set_node(finish.node + 1);
			finish.cur = finish.first;
		}
		catch (...) {
			//...
		}
	}
	//ͷ����
	void push_front(const value_type& t) {
		if (start.cur != start.first)
		{
			//��һ������ǰ���б��ÿռ�
			construct(start.cur - 1, t);
			--start.cur;
		}
		else
			push_front_aux(t);//�ޱ��ÿռ�
	}
	void push_front_aux(const value_type& t) {
		value_type t_copy = t;
		reserve_map_at_front();//ĳЩ�����±��뻻һ��map
		//�����»�����
		*(start.node - 1) = allocate_node();
		try {
			start.set_node(start.node - 1);
			start.cur = start.last - 1;
			construct(start.cur, t_copy);
		}
		catch (...) {
			//������������쳣������ǹ���ʱ�������������˳�򣬷����쳣Ҫ�ָ�������״̬
			start.set_node(start.node + 1);
			start.cur = start.first;
			deallocate_node(*(start.node - 1));
			throw;
		}
	}
	//βɾ��
	void pop_back() {
		if (finish.cur != finish.first)
		{
			//���һ����������һ��������Ԫ��
			--finish.cur;//����ָ��
			destroy(finish.cur);//����Ԫ��
		}
		else
			pop_back_aux();//���һ����������Ԫ��
	}
	void pop_back_aux() {
		deallocate_node(finish.first);//�ͷ����һ��������
		finish.set_node(finish.node - 1);//����������״̬
		finish.cur = finish.last - 1;
		destroy(finish.cur);//����Ԫ��
	}
	//ͷɾ��
	void pop_front() {
		if (start.cur != start.last - 1)
		{
			//��һ��������һ������Ԫ��
			destroy(start.cur);//����Ԫ��
			++start.cur;//����������
		}
		else
			pop_front_aux();//��һ������ֻʣһ��Ԫ��
	}
	void pop_front_aux() {
		destroy(start.cur);//������һ���������һ��Ԫ��
		deallocate_node(start.first);//�ͷŻ�����
		start.set_node(start.node + 1);//����������
		start.cur = start.first;
	}
	//���deque����Ҫע�⣬deque���״̬����ʹ���κ�Ԫ�أ�����һ�������������clear��Ҳ��Ҫ����һ��
	void clear() {
		for (map_pointer node = start.node + 1; node < finish.node; ++node)
		{
			destroy(*node, *node + iterator::buffer_size());//��������Ԫ��
			data_allocator::deallocate(*node, iterator::buffer_size());//�ͷŻ������ڴ�
		}
		if (start.node != finish.node)
		{
			//�ͷź���ͷβ����������������������Ԫ�غ��ͷ�β������������ͷ������
			destroy(start.cur, start.last);
			destroy(finish.first, finish.cur);
			data_allocator::deallocate(finish.first, iterator::buffer_size());
		}
		else //ֻ��һ����������ֻ�ͷ�Ԫ�أ�����������
			destroy(start.cur, finish.cur);
		finish = start;
	}
	//���ݵ��������ĳ��Ԫ��
	iterator erase(iterator pos) {
		iterator next = pos;
		++next;
		difference_type index = pos - start;//ɾ����֮ǰԪ�ظ���
		if (index < (size() >> 1))
		{
			//�������ǰ���Ǳ���Ҫ�ƶ���Ԫ�ؽ���
			copy_backward(start, pos, next);
			pop_front();
		}
		else
		{
			copy(next, finish, pos);
			pop_back();//�ƶ���ȥ������Ԫ��
		}
		return start + index;
	}
	//���ݵ��������ĳ���ε�Ԫ��
	iterator erase(iterator first, iterator last) {
		if (first == start && last ==  finish)
		{
			//Ҫɾ�������������deque��ֱ�Ӹ���clear()
			clear();
			return finish;
		}
		else
		{
			difference_type n = last - first;//������䳤��
			difference_type elems_before = first - start;//�Ƚ��������ǰ��Ԫ�ظ���
			if (elems_before < (size() - n) / 2)
			{
				//�Ǳ��پ��ƶ��ıߣ�֮��ȥ������Ԫ�أ��ͷ����໺����
				copy_backward(start, first, last);
				iterator new_start = start + n;
				destroy(start, new_start);
				for (map_pointer cur = start.node; cur < new_start.node; ++cur)
					data_allocator::deallocate(*cur, iterator::buffer_size());
				start = new_start;//���������
			}
			else
			{
				//��������Ԫ�ؽ���
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
	//��ĳ������һ��Ԫ�أ�ĳ����֮ǰ��
	iterator insert(iterator position, const value_type& x) {
		if (position.cur == start.cur) //�����Ϊ��ǰ�ˣ�����push_front;
		{
			push_front(x);
			return start;
		}
		if (position.cur == finish.cur) //�����Ϊβ�ˣ�����push_back;
		{
			push_back(x);
			iterator temp = finish;
			--temp;
			return temp;
		}
		return insert_aux(position, x);
	}
	iterator insert_aux(iterator pos, const value_type& x) {
		difference_type index = pos - index;//��������֮ǰԪ��
		//ͬɾ��һ�����Ƚϲ����ǰ��Ԫ�ظ������ƶ����ٵ��Ǳ�
		value_type x_copy = x;
		if (index < size() / 2)
		{
			push_front(front());//������ǰ�˲���һ��Ԫ�أ����Ը��ñ��ÿռ䲻�������
			iterator front1 = start;
			++front1;
			iterator front2 = front1;
			++front2;
			pos = start + index;
			iterator pos1 = pos;
			++pos1;
			copy(front2, pos1, front1);//Ԫ���ƶ�����ԭ�ڶ���Ԫ�ص�������Ԫ��ǰ��һλ
		}
		else
		{
			push_back(back());
			iterator back1 = finish;
			--back1;
			iterator back2 = back1;
			--back2;
			pos = start + index;
			copy_backward(pos, back2, back1);//������㵽ԭ�����ڶ���Ԫ��λ��Ԫ�غ���һλ
		}
		*pos = x_copy;//�ƶ��󽫲���ֵ�趨
		return pos;
	}
};