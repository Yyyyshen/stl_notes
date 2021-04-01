#pragma once

/**
 * hashmap
 */

template<class Key, class T, class HashFcn, class EqualKey, class Alloc>
class y_hash_map {
private:
	template<class Pair> struct select1st { /*仿函数*/ };
	typedef hashtable<pair<const Key, T>, Key, HashFcn, select1st<pair<const Key, T>>, EqualKey, Alloc> ht;
	ht rep;
	//除了元素改为pair类型，可以改变实值之外，没什么特别之处
	//...不再重复性敲了
};