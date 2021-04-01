#pragma once

/**
 * map
 *
 * 所有元素都是pair，同时拥有键值和实值，不允许两个元素有相同键值
 */

template<class T1, class T2>
struct y_pair {
	typedef T1 first_type;
	typedef T2 second_type;

	T1 first;
	T2 second;
	y_pair() :first(T1()), second(T2()) {}
	y_pair(const T1& a, const T2& b) :first(a), second(b) {}
};
//仿函数，返回pair中第一个元素
template <class _Pair>
struct y_select1st : public unary_function<_Pair, typename _Pair::first_type> {
	const typename _Pair::first_type& operator()(const _Pair& __x) const {
		return __x.first;
	}
};

//map想要通过迭代器修改键值是不行的，因为关系到排列规则，但于set不同的是，可以修改实值
//同样以红黑树为底层机制，大多数操作函数直接转发调用
template<class Key, class T, class Compare, class Alloc>
class y_map {
public:
	typedef Key key_type;
	typedef T data_type;
	typedef T mapped_type;
	typedef y_pair<const Key, T> value_type;
	typedef Compare key_compare;
	//内部类定义一个比较函数
	class value_compare :public binary_function<value_type, value_type, bool> {
		friend class y_map<Key, T, Compare, Alloc>;
	protected:
		Compare comp;
		value_compare(Compare c) :comp(c) {}
	public:
		bool operator()(const value_type& x, const value_type& y) const {
			return comp(x.first, y.first);//只比较键值
		}
	};
private:
	//红黑树表现map
	typedef rb_tree<key_type, value_type, y_select1st<value_type>, key_compare, Alloc> rep_type;
	rep_type t;
public:
	typedef typename rep_type::pointer pointer;
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::reference reference;
	typedef typename rep_type::const_reference const_reference;
	typedef typename rep_type::iterator iterator;//允许修改实值
	typedef typename rep_type::const_iterator const_iterator;
	typedef typename rep_type::reverse_iterator reverse_iterator;
	typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
	typedef typename rep_type::size_type size_type;
	typedef typename rep_type::difference_type difference_type;
public:
	//构造，添加元素用底层红黑树的insert_unique，multimap才允许相同键值存在
	y_map() :t(Compare()) {}
	explicit y_map(const Compare& comp) :t(comp) {}
	template<class InputIterator>
	y_map(InputIterator first, InputIterator last) : t(Compare()) { t.insert_unique(first, last); }
	template<class InputIterator>
	y_map(InputIterator first, InputIterator last, const Compare& comp) : t(comp) { t.insert_unique(first, last); }
	y_map(const y_map<Key, T, Compare, Alloc>& x) :t(x.t) {}
	y_map<Key, T, Compare, Alloc>& operator=(const y_map<Key, T, Compare, Alloc>& x) { t = x.t; return *this; }
public:
	//操作函数
	key_compare key_comp() const { return t, key_comp(); }
	value_compare value_comp() const { return value_compare(t.key_comp()); }
	iterator begin() { return t.begin(); }
	const_iterator begin() const { return t.begin(); }
	iterator end() { return t.end(); }
	const_iterator end() const { return t.end(); }
	//...reverse迭代器相关，略
	bool empty() const { t.empty(); }
	size_type size() const { return t.size(); }
	size_type max_size() const { return t.max_size(); }
	//下标操作符
	T& operator[](const key_type& k) { return (*((insert(value_type(k, T()))).first)).second; } //实值以引用方式返回，左右值都适用
	void swap(y_map<Key, T, Compare, Alloc>& x) { t.swap(x.t); }
	pair<iterator, bool> insert(const value_type& x) { return t.insert_unique(x); }
	iterator insert(iterator position, const value_type& x) { return t.insert_unique(position, x); }
	template<class InputIterator>
	void insert(InputIterator first, InputIterator last) { t.insert_unique(first, last); }
	void erase(iterator position) { t.erase(position); }
	size_type erase(const key_type& x) { return t.erase(x); }
	void erase(iterator first, iterator last) { t.erase(first, last); }
	void clear() { t.clear(); }
	//...略过一些直接转调用的封装
	//操作符
	friend bool operator==(const y_map&, const y_map&);
	friend bool operator<(const y_map&, const y_map&);
};

template<class Key, class T, class Compare, class Alloc>
inline bool operator==(const y_map<Key, T, Compare, Alloc>& x, const y_map<Key, T, Compare, Alloc>& y) {
	return x.t == y.t;
}

template<class Key, class T, class Compare, class Alloc>
inline bool operator<(const y_map<Key, T, Compare, Alloc>& x, const y_map<Key, T, Compare, Alloc>& y) {
	return x.t < y.t;
}