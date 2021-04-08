#pragma once

/**
 * iterator adapters
 */

template<class Container>
class y_back_insert_iterator {
	//将迭代器赋值操作改为从容器尾端插入操作
protected:
	Container* container;
public:
	typedef output_iterator_tag iterator_category;//注意类型
	typedef void value_type;
	typedef void difference_type;
	typedef void pointer;
	typedef void reference;

	explicit y_back_insert_iterator(Container& x) :container(x) {}
	y_back_insert_iterator<Container>& operator=(const typename Container::value_type& value) {
		container->push_back(value);//赋值转为调用push_back
		return *this;
	}
	//其他操作符关闭功能，直接返回自己
	y_back_insert_iterator<Container>& operator*() { return *this; }
	y_back_insert_iterator<Container>& operator++() { return *this; }
	y_back_insert_iterator<Container>& operator++(int) { return *this; }
};
//辅助函数，帮助我们使用back_insert_iterator
template<class Container>
inline y_back_insert_iterator<Container> y_back_inserter(Container& x) {
	return y_back_insert_iterator<Container>(x);
}
//往头端插入的front_insert_iterator同理，只是改变了转调用的函数
template<class Container>
class y_front_insert_iterator {
	//将迭代器赋值操作改为从容器头端插入操作
protected:
	Container* container;
public:
	typedef output_iterator_tag iterator_category;//注意类型
	typedef void value_type;
	typedef void difference_type;
	typedef void pointer;
	typedef void reference;

	explicit y_front_insert_iterator(Container& x) :container(x) {}
	y_front_insert_iterator<Container>& operator=(const typename Container::value_type& value) {
		container->push_front(value);//赋值转为调用push_front
		return *this;
	}
	//其他操作符关闭功能，直接返回自己
	y_front_insert_iterator<Container>& operator*() { return *this; }
	y_front_insert_iterator<Container>& operator++() { return *this; }
	y_front_insert_iterator<Container>& operator++(int) { return *this; }
};
//辅助函数，帮助我们使用front_insert_iterator
template<class Container>
inline y_front_insert_iterator<Container> y_front_inserter(Container& x) {
	return y_front_insert_iterator<Container>(x);
}
//在指定位置操作，并将迭代器右移
template<class Container>
class y_insert_iterator {
	//将迭代器赋值操作改为从容器头端插入操作
protected:
	Container* container;
	typename Container::iterator iter;
public:
	typedef output_iterator_tag iterator_category;//注意类型
	typedef void value_type;
	typedef void difference_type;
	typedef void pointer;
	typedef void reference;

	explicit y_insert_iterator(Container& x, typename Container::iterator i) :container(x), iter(i) {}
	y_insert_iterator<Container>& operator=(const typename Container::value_type& value) {
		iter = container->insert(iter, value);//赋值转为调用insert
		++iter;
		return *this;
	}
	//其他操作符关闭功能，直接返回自己
	y_insert_iterator<Container>& operator*() { return *this; }
	y_insert_iterator<Container>& operator++() { return *this; }
	y_insert_iterator<Container>& operator++(int) { return *this; }
};
//辅助函数，帮助我们使用insert_iterator
template<class Container, class Iterator>
inline y_insert_iterator<Container> y_inserter(Container& x, Iterator i) {
	typedef typename Container::iterator iter;
	return y_insert_iterator<Container>(x, iter(i));
}

//reverse iterator 将迭代器移动行为倒转，双向序列容器几乎都提供了反转迭代器
//其中rbegin与end不同的是，前者指向最后一个元素，后者指向最后一个元素的后一个位置
//begin与rend不同的是，前者指向第一个元素，后者指向第一个元素的前一个元素
template<class Iterator>
class y_reverse_iterator {
protected:
	Iterator current;
public:
	typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
	typedef typename iterator_traits<Iterator>::value_type value_type;
	typedef typename iterator_traits<Iterator>::difference_type difference_type;
	typedef typename iterator_traits<Iterator>::pointer pointer;
	typedef typename iterator_traits<Iterator>::reference reference;

	typedef Iterator iterator_type;
	typedef y_reverse_iterator<Iterator> self;
public:
	y_reverse_iterator() {}
	explicit y_reverse_iterator(iterator_type x) : current(x) {}
	y_reverse_iterator(const self& x) :current(x.current) {}

	iterator_type base() const { return current; } //返回对应的正向迭代器
	reference operator*() const {
		Iterator tmp = current;
		return *--tmp;
		//对逆向迭代器取值，就是将对应的正向迭代器后退一格取值
	}
	pointer operator->() const { return &(operator*()); }
	//前进变后退
	self& operator++() {
		--current;
		return *this;
	}
	self operator++(int) {
		//后置
		self temp = *this;
		--current;
		return temp;
	}
	//后退变前进
	self& operator--() {
		++current;
		return *this;
	}
	self operator--(int) {
		self temp = *this;
		++current;
		return temp;
	}
	//加减操作也逆转过来
	self operator+(difference_type n) const {
		return self(current - n);
	}
	self& operator+=(difference_type n) {
		current -= n;
		return *this;
	}
	self operator-(difference_type n) const {
		return self(current + n);
	}
	self operator-=(difference_type n) {
		current += n;
		return *this;
	}
	//下标操作符，需要注意的是第一个*和+，会调用本类内的operator*,operator+，但第二个*不会，判断法则是依据处理的类型而定的
	reference operator[](difference_type n) const { return *(*this + n); }
};


//stream iterator
//将迭代器绑定到数据流对象上，可以方便的让其拥有输入输出能力
//在迭代器做++操作时，会被引导调用迭代器内部维护的stream成员，从而实现输入输出
template<class T, class Distance = ptrdiff_t>
class y_istream_iterator {
	//将输入流对象绑定的迭代器
	friend bool operator==<>(const y_istream_iterator<T, Distance>& x, const y_istream_iterator<T, Distance>& y);//STL中使用宏定义__STL_NULL_TMPL_ARGS为<>符号
protected:
	istream* stream;
	T value;
	bool end_marker;
	void read() {
		end_marker = (*stream) ? true : false;
		if (end_marker) *stream >> value;//很关键，输入之后，stream状态可能会改变，要反复判断end_marker，读到eof或类型不符的资料时，处于false状态
		end_marker = (*stream) ? true : false;
	}
public:
	typedef input_iterator_tag iterator_category;
	typedef T value_type;
	typedef Distance difference_type;
	typedef const T* pointer;
	typedef const T& reference;//因本身为input iterator类型，所以用const型成员比较保险

	y_istream_iterator() :stream(&cin), end_marker(false) {}//默认使用cin
	y_istream_iterator(istream& s) :stream(&s) { read(); }

	reference operator*() const { return value; }
	pointer operator->() const { return &(operator*()); }

	y_istream_iterator<T, Distance>& operator++() {
		read();
		return *this;
	}
	y_istream_iterator<T, Distance> operator++(int) {
		y_istream_iterator<T, Distance> temp = *this;
		read();
		return temp;
	}
};
template<class T>
class y_ostream_iterator {
	//输出流对象绑定到迭代器
protected:
	ostream* stream;
	const char* string;
public:
	typedef output_iterator_tag iterator_category;
	typedef void value_type;
	typedef void difference_type;
	typedef void pointer;
	typedef void reference;

	y_ostream_iterator(ostream& s) :stream(&s), string(0) {}
	y_ostream_iterator(ostream& s, const char* c) :stream(&s), string(c) {}

	y_ostream_iterator<T>& operator=(const T& value) {
		*stream << value;
		if (string) *steam << string;
		return *this;
	}

	y_ostream_iterator<T>& operator*() { return *this; }
	y_ostream_iterator<T>& operator++() { return *this; }
	y_ostream_iterator<T>& operator++(int) { return *this; }
};