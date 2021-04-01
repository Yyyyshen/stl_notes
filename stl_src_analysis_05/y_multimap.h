#pragma once

/**
 * multimap
 * ��һ����map��֮ͬ��
 */

template<class Key, class T, class Compare, class Alloc>
class y_multimap {
public:
	//typedefs����Ա��mapһ��
	typedef rb_tree<Key, pair<Key, T>, y_select1st<pair<Key, T>>, Compare, Alloc> rep_type;
	rep_type t;
	typedef typename rep_type::iterator iterator;
public:
	//���죬�����������insert_equal�������ط�һ��
	template<class InputIterator>
	y_multimap(InputIterator first, InputIterator last) :t(Compare()) { t.insert_equal(first, last); }
	iterator insert(const pair<Key, T>& x) { return t.insert_equal(x); }
	//...
};