#pragma once

/**
 * hashset
 */

template<class Value, class HashFcn, class EqualKey, class Alloc>
class y_hash_set {
private:
	template<class T>struct identity {/*仿函数重载()*/ };
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
	typedef typename ht::const_iterator iterator;//与set相同，不允许通过迭代器随意修改键值和实值，型别定义都用const
	typedef typename ht::const_iterator const_iterator;

	hasher hash_func() const { return rep.hash_func(); }
	key_equal key_eq() const { return rep.key_eq(); }
public:
	//默认大小100，调整为最接近且更大的质数
	y_hash_set() :rep(100, hasher(), key_equal()) {}
	explicit y_hash_set(size_type n) :rep(n, hasher(), key_equal()) {}
	//...略过一些构造函数
	template<class InputIterator>
	y_hash_set(InputIterator first, InputIterator last) : rep(100, hasher(), key_equal()) { rep.insert_unique(first, last); }
	//插入操作不允许键值重复
	pair<iterator, bool> insert(const value_type& obj) {
		pair<typename ht::iterator, bool> p = rep.insert_unique(obj);
		return pair<iterator, bool>(p.first, p.second);
	}
	//一些操作函数，直接转发调用
	size_type size() const { return rep.size(); }
	size_type empty() const { return rep.empty(); }
	//...都是直接调用底层容器的操作，略过
};