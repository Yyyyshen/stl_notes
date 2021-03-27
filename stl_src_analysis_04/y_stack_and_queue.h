#pragma once
/**
 * stack
 * FILO先进后出
 */
template<class T, class Sequence = deque<T>>
class y_stack {
	friend bool operator==(const y_stack&, const y_stack&);
	friend bool operator<(const y_stack&, const y_stack&);
public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type size_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;
protected:
	Sequence c;//底层容器，默认使用deque，用list也可以
public:
	//利用底层容器实现功能
	bool empty() const { return c.empty(); }
	size_type size() const { return c.size(); }
	reference top() { return c.back(); }
	const_reference top() const { return c.back(); }
	void push(const value_type& x) { c.push_back(x); }
	void pop() { c.pop_back(); }//末端进末端出
	//由于元素要满足先进后出，只有stack顶端元素才会被取用，所以也不提供迭代器
};

template<class T, class Sequence>
bool operator==(const y_stack<T, Sequence>& x, const y_stack<T, Sequence>& y) {
	return x.c == y.c;
}
template<class T, class Sequence>
bool operator<(const y_stack<T, Sequence>& x, const y_stack<T, Sequence>& y) {
	return x.c < y.c;
}

/**
 * queue
 * FIFO先进先出
 */
template<class T, class Sequence = deque<T>>
class y_queue {
	friend bool operator==(const y_queue&, const y_queue&);
	friend bool operator<(const y_queue&, const y_queue&);
public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type size_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;
protected:
	Sequence c;//底层容器，默认使用deque，用list也可以
public:
	//利用底层容器实现功能
	bool empty() const { return c.empty(); }
	size_type size() const { return c.size(); }
	reference front() { return c.front(); }
	const_reference front() const { return c.front(); }
	reference back() { return c.back(); }
	const_reference back() const { return c.back(); }
	void push(const value_type& x) { c.push_back(x); }
	void pop() { c.pop_front(); }//末端进前端出
	//由于元素要满足先进先出，只有首尾端用到，所以也不提供迭代器
};

template<class T, class Sequence>
bool operator==(const y_queue<T, Sequence>& x, const y_queue<T, Sequence>& y) {
	return x.c == y.c;
}
template<class T, class Sequence>
bool operator<(const y_queue<T, Sequence>& x, const y_queue<T, Sequence>& y) {
	return x.c < y.c;
}