#pragma once

/**
 * hashmap
 */

template<class Key, class T, class HashFcn, class EqualKey, class Alloc>
class y_hash_map {
private:
	template<class Pair> struct select1st { /*�º���*/ };
	typedef hashtable<pair<const Key, T>, Key, HashFcn, select1st<pair<const Key, T>>, EqualKey, Alloc> ht;
	ht rep;
	//����Ԫ�ظ�Ϊpair���ͣ����Ըı�ʵֵ֮�⣬ûʲô�ر�֮��
	//...�����ظ�������
};