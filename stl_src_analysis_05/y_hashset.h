#pragma once

/**
 * hashset
 */

template<class Value, class HashFcn, class EqualKey, class Alloc>
class y_hash_set {
private:
	template<class T>struct identity {/*�º�������()*/ };
	typedef hashtable<Value, Value, HashFcn, identity<Value>, EqualKey, Alloc> ht;
	ht rep;
public:
	typedef typename ht::key_type key_type;
	typedef typename ht::value_type value_type;
	typedef typename ht::hasher hasher;
	typedef typename ht::key_equal key_equal;
	typedef typename ht::size_type size_type;
	typedef typename ht::difference_type difference_type;
	typedef typename ht::const_pointer pointer;
	typedef typename ht::const_pointer const_pointer;
	typedef typename ht::const_reference reference;
	typedef typename ht::const_reference const_reference;
	typedef typename ht::const_iterator iterator;//��set��ͬ��������ͨ�������������޸ļ�ֵ��ʵֵ���ͱ��嶼��const
	typedef typename ht::const_iterator const_iterator;

	hasher hash_func() const { return rep.hash_func(); }
	key_equal key_eq() const { return rep.key_eq(); }
public:
	//Ĭ�ϴ�С100������Ϊ��ӽ��Ҹ��������
	y_hash_set() :rep(100, hasher(), key_equal()) {}
	explicit y_hash_set(size_type n) :rep(n, hasher(), key_equal()) {}
	//...�Թ�һЩ���캯��
	template<class InputIterator>
	y_hash_set(InputIterator first, InputIterator last) : rep(100, hasher(), key_equal()) { rep.insert_unique(first, last); }
	//��������������ֵ�ظ�
	pair<iterator, bool> insert(const value_type& obj) {
		pair<typename ht::iterator, bool> p = rep.insert_unique(obj);
		return pair<iterator, bool>(p.first, p.second);
	}
	//һЩ����������ֱ��ת������
	size_type size() const { return rep.size(); }
	size_type empty() const { return rep.empty(); }
	//...����ֱ�ӵ��õײ������Ĳ������Թ�
};