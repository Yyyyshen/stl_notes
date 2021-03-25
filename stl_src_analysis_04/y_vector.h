#pragma once

/**
 * vector关键点
 */

class y_alloc { /* 默认空间配置器 */ };
template<class T, class Alloc>
class y_simple_alloc { /* 更方便得以元素大小为配置单位 */ };
template<class T, class Alloc = y_alloc>
class y_vector {
public:
	//内嵌型别定义
	typedef T			value_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef value_type* iterator;	//vector支持随机存取，普通指针满足需求；例如，vector<int>::iterator iter;中iter就是int*
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;

protected:
	iterator start;				//当前使用空间的头
	iterator finish;			//当前使用空间的尾
	iterator end_of_storage;	//当前可用空间的尾，也就是实际配置大小可能比使用者需求量大，以备扩充

public:
	//利用三个迭代器实现首尾标识、大小、容量、判空、[]运算符、前后端元素等
	iterator begin() { return start; }
	iterator end() { return finish; }
	size_type size() const { return size_type(end() - begin()); }
	size_type capacity() const { return size_type(end_of_storage - begin()); }
	bool empty() const { return begin() == end(); }
	reference operator[](size_type n) { return *(begin() + n); }
	reference front() { return *begin(); }
	reference back() { return*(end() - 1); }
	//空间配置器
	typedef y_simple_alloc<value_type, Alloc> data_allocator;
	//各种构造
	y_vector() : start(0), finish(0), end_of_storage(0) {} //空vec
	y_vector(size_type n, const T& value) { fill_initialize(n, value); } //填充一定数量的某值
	explicit y_vector(size_type n) { fill_initialize(n, T()); }
	~y_vector() { destroy(start, finish); deallocate(); }

protected:
	void fill_initialize(size_type n, const T& value) {
		//填充内容予以初始化
		start = allocate_and_fill(n, value);
		finish = start + n;
		end_of_storage = finish;
	}
	iterator allocate_and_fill(size_type n, const T& x) {
		iterator result = data_allocator::allocate(n);
		uninitialized_fill_n(result, n, x); //具体填充实现第二节已了解
		return result;
	}
	void deallocate() {
		if (start) data_allocator::deallocate(start, end_of_storage - start);
	}
	void insert_aux(iterator position, const T& x) {
		if (finish != end_of_storage) {
			//确认是否有备用空间，若有再备用空间起始位置构造一个元素，并以当前最后一个元素为初值
			construct(finish, *(finish - 1));
			++finish;
			//调整末尾位置后拷贝数据
			T x_copy = x;
			copy_backward(position, finish - 2, finish - 1);
			*position = x_copy;
		}
		else {
			//无备用空间
			const size_type old_size = size();
			const size_type len = old_size != 0 ? 2 * old_size : 1;//如果是一开始为空构造，先定为1；否则成倍扩容
			iterator new_start = data_allocator::allocate(len);//重新分配空间
			iterator new_finish = new_start;
			try
			{
				//拷贝原来内容到新空间
				new_finish = uninitialized_copy(start, position, new_start);//第二章了解过
				construct(new_finish, x);//在结尾添加新元素
				++new_finish;//调整指针
				new_finish = uninitialized_copy(position, finish, new_finish);//将原vector备用空间内容也拷贝过来
			}
			catch (...)
			{
				//拷贝发生异常，释放空间
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, len);
				throw;
			}
			//析构并释放原vector空间
			destroy(begin(), end());
			deallocate();
			//调整迭代器成员为新的值
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + len;
			//由此可见，如果对vector操作引起了重新分配空间，指向原vector的迭代器就会失效了，要注意
		}
	}

public:
	//尾部增加元素
	void push_back(const T& x) {
		//先判断是否有备用空间
		if (finish != end_of_storage)
		{
			//还有空间
			construct(finish, x);//见stl_src_analysis_02中的全局函数
			++finish;
		}
		else //无备用空间
			insert_aux(end(), x);
	}
	//尾部删除元素
	void pop_back() {
		--finish;//先前移，finish才表示最后一个元素
		destroy(finish);
	}
	//清除范围内元素并移动位置
	iterator erase(iterator first, iterator last) {
		iterator i = copy(last, finish, first);//拷贝范围后元素到要删除的开端，返回拷贝后最后一个元素的指针位置
		destroy(i, finish);//删除拷贝后最后一个元素位置到结尾的元素
		//调整迭代器
		finish = finish - (last - first);
		return first;
	}
	//删除某个位置元素
	iterator erase(iterator position) {
		if (position + 1 != end())
			copy(position + 1, finish, position);//相当于将指定位置后元素整体前移；如果删除的就是最后一个元素，相当于pop_back();
		//调整范围
		--finish;
		destroy(finish);
		return position;
	}
	//整体清除
	void clear() { erase(begin(), end()); }
	//从position开始，插入n个元素，初值为x
	void insert(iterator position, size_type n, const T& x) {
		if (n != 0) //传0的话没意义
		{
			if (size_type(end_of_storage - finish) >= n) //检查空间是否够用
			{
				T x_copy = x;
				const size_type elems_after = finish - position;//计算插入点之后的元素个数
				iterator old_finish = finish;//记录当前结尾
				if (elems_after > n)
				{
					//后面元素大于新增个数
					uninitialized_copy(finish - n, finish, finish);
					finish += n;
					copy_backward(position, old_finish - n, old_finish);
					fill(position, position + n, x_copy);//从插入点填充新元素
				}
				else
				{
					//后面元素个数小于等于新增个数
					uninitialized_fill_n(finish, n - elems_after, x_copy);
					finish += n - elems_after;
					uninitialized_copy(position, old_finish, finish);
					finish += elems_after;
					fill(position, old_finish, x_copy);
				}

			}
			else //空间不够
			{
				//确定新长度
				const size_type old_size = size();
				const size_type len = old_size + max(old_size, n); //添加个数小于现有大小，还是成倍扩容；否则扩容要添加的大小
				//配置新空间，同push_back中的处理
				iterator new_start = data_allocator::allocate(len);
				iterator new_finish = new_start;
				try
				{
					//拷贝原来内容到新空间
					new_finish = uninitialized_copy(start, position, new_start);//第二章了解过
					construct(new_finish, x);//在结尾添加新元素
					++new_finish;//调整指针
					new_finish = uninitialized_copy(position, finish, new_finish);//将原vector备用空间内容也拷贝过来
				}
				catch (...)
				{
					//拷贝发生异常，释放空间
					destroy(new_start, new_finish);
					data_allocator::deallocate(new_start, len);
					throw;
				}
				//析构并释放原vector空间
				destroy(begin(), end());
				deallocate();
				//调整迭代器成员为新的值
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + len;
			}
		}
	}
};