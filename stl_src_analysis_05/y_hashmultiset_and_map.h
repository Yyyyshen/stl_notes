#pragma once

/**
 * hash_multiset
 * hash_multimap
 */

template<class Value, class HashFcn, class EqualKey, class Alloc>
class y_hash_multiset {
private:
	template<class T>struct identity {};
	typedef hashtable<Value, Value, HashFcn, identity<Value>, EqualKey, Alloc> ht;
	ht rep;
	//底层容器用hashtable，插入操作用insert_equal，无其他特殊之处
};

template<class Key, class T, class HashFcn, class EqualKey, class Alloc>
class y_hash_multimap {
private:
	template<class Pair>struct select1st {};
	typedef hashtable<pair<const Key, T>, T, HashFcn, select1st<pair<const Key, T>>, EqualKey, Alloc> ht;
	ht rep;
	//底层容器用hashtable，除了元素使用了pair，没什么特别的
};