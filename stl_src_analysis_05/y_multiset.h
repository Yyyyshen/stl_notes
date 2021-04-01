#pragma once

/**
 * multiset
 * ��������set��ͬ�ĵط�
 */
template<class Key, class Compare, class Alloc>
class y_multiset {
public:
	//typedefs...��set��ͬ����
	//��Ա
	template<class T>
	struct identity :public unary_function<T, T> {
		const T& operator()(const T& x) const { return x; }
	};//�º���
	typedef rb_tree<Key, Key, identity<Key>, Compare, Alloc> rep_type;
	rep_type t;//�ײ���ú����
	typedef typename rep_type::iterator iterator;
public:
	//���죬���˲��뷽��ʹ��insert_equal��������ͬ��ֵ֮�⣬û������
	template<class InputIterator>
	y_multiset(InputIterator first, InputIterator last) :t(Compare()) { t.insert_equal(first, last); }
	//���뷽��
	iterator insert(const Key& x) { return t.insert_equal(x); }
	//...��������һ��
};