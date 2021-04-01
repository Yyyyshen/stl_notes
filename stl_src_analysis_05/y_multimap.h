#pragma once

/**
 * multimap
 * 看一下与map不同之处
 */

template<class Key, class T, class Compare, class Alloc>
class y_multimap {
public:
	//typedefs，成员与map一致
	typedef rb_tree<Key, pair<Key, T>, y_select1st<pair<Key, T>>, Compare, Alloc> rep_type;
	rep_type t;
	typedef typename rep_type::iterator iterator;
public:
	//构造，插入操作改用insert_equal，其他地方一致
	template<class InputIterator>
	y_multimap(InputIterator first, InputIterator last) :t(Compare()) { t.insert_equal(first, last); }
	iterator insert(const pair<Key, T>& x) { return t.insert_equal(x); }
	//...
};