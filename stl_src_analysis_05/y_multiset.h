#pragma once

/**
 * multiset
 * 仅看下与set不同的地方
 */
template<class Key, class Compare, class Alloc>
class y_multiset {
public:
	//typedefs...与set相同，略
	//成员
	template<class T>
	struct identity :public unary_function<T, T> {
		const T& operator()(const T& x) const { return x; }
	};//仿函数
	typedef rb_tree<Key, Key, identity<Key>, Compare, Alloc> rep_type;
	rep_type t;//底层采用红黑树
	typedef typename rep_type::iterator iterator;
public:
	//构造，除了插入方法使用insert_equal，允许相同键值之外，没有区别
	template<class InputIterator>
	y_multiset(InputIterator first, InputIterator last) :t(Compare()) { t.insert_equal(first, last); }
	//插入方法
	iterator insert(const Key& x) { return t.insert_equal(x); }
	//...其他基本一致
};