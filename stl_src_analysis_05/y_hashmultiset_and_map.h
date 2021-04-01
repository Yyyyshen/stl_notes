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
	//�ײ�������hashtable�����������insert_equal������������֮��
};

template<class Key, class T, class HashFcn, class EqualKey, class Alloc>
class y_hash_multimap {
private:
	template<class Pair>struct select1st {};
	typedef hashtable<pair<const Key, T>, T, HashFcn, select1st<pair<const Key, T>>, EqualKey, Alloc> ht;
	ht rep;
	//�ײ�������hashtable������Ԫ��ʹ����pair��ûʲô�ر��
};