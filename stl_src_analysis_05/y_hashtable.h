#pragma once

/**
 * hashtable
 *
 * 根据开链法，哈希表内每个元素不只是一个节点元素，还可能是一个桶（bucket）节点，桶节点是一个list
 */

template <class Value>
struct __hashtable_node {
	__hashtable_node* next;
	Value val;
};

template<class Value, class Key, class HashFcn, class ExtracKey, class EqualKey, class Alloc> class hashtable;

//迭代器设计
template<class Value, class Key, class HashFcn, class ExtracKey, class EqualKey, class Alloc>
struct __hashtable_iterator {
	typedef hashtable<Value, Key, HashFcn, ExtracKey, EqualKey, Alloc> hashtable;
	typedef __hashtable_iterator<Value, Key, HashFcn, ExtracKey, EqualKey, Alloc> iterator;
	//typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtracKey, EqualKey, Alloc> const_iterator;
	typedef __hashtable_node<Value> node;
	typedef forward_iterator_tag iterator_category;
	typedef Value value_type;
	typedef Value& reference;
	typedef Value* pointer;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	node* cur;//迭代器目前所指节点
	hashtable* ht;//保持容器连结关系

	__hashtable_iterator(node* n, hashtable* tab) :cur(n), ht(tab) {}
	__hashtable_iterator() {}
	reference operator*() const { return cur->val; }
	pointer operator->() const { return &(operator*()); }
	iterator& operator++() {
		const node* old = cur;
		cur = cur->next;
		if (!cur)
		{
			size_type bucket = ht->bkt_num(old->val);
			while (!cur && ++bucket < ht->bukets.zixe)
				cur = ht->bucket[bucket];
		}
		return *this;
	}
	iterator operator++(int) {
		iterator temp = *this;
		++(*this);
		return temp;
	}
	bool operator==(const iterator& it) const { return cur == it->cur; }
	bool operator!=(const iterator& it) const { return cur != it->cur; }

};
#include <vector>
template<class Value/*实值*/, class Key/*键值*/, class HashFcn/*散列函数*/, class ExtracKey/*取键值*/, class EqualKey/*判断键值是否相等*/, class Alloc>
class hashtable {
public:
	typedef HashFcn hasher;
	typedef EqualKey key_equal;
	typedef size_t size_type;
	typedef Value value_type;
	typedef Key key_type;
private:
	//几个函数对象
	hasher hash;
	key_equal equals;
	ExtracKey get_key;

	typedef __hashtable_node<Value> node;
	typedef y_simple_alloc<node, Alloc> node_allocator;
	typedef __hashtable_iterator<Value, Key, HashFcn, ExtracKey, EqualKey, Alloc> iterator;
	//维护桶节点，直接用vector作为buckets
	vector<node, Alloc> buckets;
	size_type num_elements;
public:
	size_type bucket_count() const { return buckets.size(); }
	//...
	node* new_node(const value_type& obj) {
		node* n = node_allocator::allocate();
		n->next = 0;
		try {
			construct(&n->val, obj);//在新分配节点空间上构造元素
			return n;
		}
		catch (...) {
			//失败则回收空间
			node_allocator::deallocate(n);
		}
	}
	void delete_node(node* n) {
		destroy(&n->val);//析构元素
		node_allocator::deallocate(n);//释放空间
	}
	//构造函数
	hashtable(size_type n, const HashFcn& hf, const EqualKey& eql) :hash(hf), equals(eql), get_key(ExtracKey()), num_elements(0) { initialize_buckets(n); }
	void initialize_buckets(size_type n) {
		const size_type n_buckets = next_size(n);//例如传50，返回53
		buckets.reserve(n_buckets);
		buckets.insert(buckets.end(), n_buckets, (node*)0);
		num_elements = 0;
	}
	size_type next_size(size_type n) const { return n;/*这里省略了，实际应该是返回大于n且最接近n的质数*/ }
	//插入操作，不允许重复
	pair<iterator, bool> insert_unique(const value_type& obj) {
		resize(num_elements + 1);//判断是否需要重建表格
		return insert_unique_noresize(obj);
	}
	void resize(size_type num_elements_hint) {
		//判断原则是将元素个数（新增后，所以传进来的是+1）与bucket大小比较，如果前者大，则重建
		const size_type old_n = buckets.size();
		if (num_elements_hint > old_n)
		{
			const size_type n = next_size(num_elements_hint);//找出下一个质数
			if (n > old_n)
			{
				vector<node*, Alloc> temp(n, (node*)0);//建立新的buckets
				try {
					for (size_type bucket = 0; bucket < old_n; ++bucket)
					{
						node* first = buckets[bucket];//处理每个bucket
						while (first) {
							//找出节点落在哪个新bucket内
							size_type new_bucket = bkt_num(first->val, n);
							buckets[bucket] = first->next;//旧bucket指向对应串行的下一个节点（方便后面处理循环）
							first->next = temp[new_bucket];
							temp[new_bucket] = first;//将当前节点插入新bucket，成为对应串行的第一个节点
							first = buckets[bucket];//回到旧的bucket所指待处理串行，准备处理下一个节点
						}
					}
					buckets.swap(temp);//新旧对调
				}
				catch (...) {

				}
			}
		}
	}
	pair<iterator, bool> insert_unique_noresize(const value_type& obj) {
		const size_type n = bkt_num(obj);//决定obj应位于第n个bucket
		node* first = buckets[n];//指向对应串行头，进行一些判断
		for (node* cur = first; cur; cur = cur->next)
			if (equals(get_key(cur->val), get_key(obj)))
				return pair<iterator, bool>(iterator(cur, this), false);//先遍历一遍，发现重复的直接返回失败
		//脱离循环，则插入新值
		node* temp = new_node(obj);
		temp->next = first;//产生新节点并令其成为当前串行头
		buckets[n] = temp;
		++num_elements;//累加元素个数计数
		return pair<iterator, bool>(iterator(temp, this), true);
	}
	//允许重复的插入操作
	iterator insert_equal(const value_type& obj) {
		resize(num_elements + 1);
		return insert_equal_noresize(obj);
	}
	iterator insert_equal_noresize(const value_type& obj) {
		const size_type n = bkt_num(obj);
		node* first = buckets[n];
		for (node* cur = first; cur; cur = cur->next)
			if (equals(get_key(cur->val), get_key(obj)))
			{
				//发现相同的，插入后返回
				node* temp = new_node(obj);
				temp->next = cur->next;
				cur->next = temp;
				++num_elements;
				return iterator(temp, this);
			}
		//没有重复，增加新节点
		node* temp = new_node(obj);
		temp->next = first;
		buckets[n] = temp;
		++num_elements;
		return iterator(temp, this);
	}
	//判断元素该在哪个桶里
	size_type bkt_num(const value_type& obj, size_t n) const { return bkt_num_key(get_key(obj), n); }//接收实值和buckets个数
	size_type bkt_num(const value_type& obj) const { return bkt_num_key(get_key(obj)); }//只接收实值
	size_type bkt_num_key(const key_type& key) const { return bkt_num_key(key, buckets.size()); }//接收键值
	size_type bkt_num_key(const key_type& key, size_t n) const { return hash(key) % n; }//接收键值和buckets个数，用哈希函数计算
	//拷贝和删除
	void clear() {
		//针对每一个bucket，将list中每个元素删除
		for (size_type i = 0; i < buckets.size(); ++i) {
			node* cur = buckets[i];
			while (cur != 0) {
				node* next = cur->next;
				delete_node(cur);
				cur = next;
			}
			buckets[i] = 0;//置空指向
		}
		num_elements = 0;//元素个数置0
		//vector则是没有释放，仍保留原有大小
	}
	void copy_from(const hashtable& ht) {
		//先将自己清空并调整到能够接收对方的状态
		buckets.clear();
		buckets.reserve(ht.buckets.size());//reserve只改变空间，resize改变size，所以一般reserve操作后需要insert
		buckets.insert(buckets.end(), ht.buckets.size(), (node*)0);
		try {
			for (size_type i = 0; i < ht.buckets.size(); ++i)
			{
				//复制每个元素过来
				if (const node* cur = ht.buckets[i])
				{
					node* copy = new_node(cur->val);
					buckets[i] = copy;
					//对每个桶内list元素赋值
					for (node* next = cur->next; next; cur = next, next = cur->next) {
						copy->next = new_node(next->val);
						copy = copy->next;
					}
				}
			}
			num_elements = ht.num_elements;//记录节点数
		}
		catch (...) {
			clear();
		}
	}
	//查询相关
	iterator find(const key_type& key) {
		size_type n = bkt_num_key(key);
		node* first;
		for (first = buckets[n]; first && !equals(get_key(first->val), key); first = first->next)
		{
			//这里直接在循环终止条件中对比键值，成功则返回
		}
		return iterator(first, this);
	}
	size_type count(const key_type& key) const {
		const size_type n = bkt_num_key(key);//先看在哪个桶内
		size_type result = 0;
		//遍历，看有几个相同的
		for (const node* cur = buckets[n]; cur; cur = cur->next)
			if (equals(get_key(cur->val), key))
				++result;
		return result;
	}
};

//一些常见的哈希函数
template<class Key> struct y_hash {  };

inline size_t __stl_hash_string(const char* s)
{
	unsigned long h = 0;
	for (; *s; ++s)
		h = 5 * h + *s;//按一定规则累加每个字节
	return size_t(h);
}
//利用模板特化和上面基础计算函数写一些仿函数
template<> struct y_hash<char*> {
	size_t operator()(const char* s) const { return __stl_hash_string(s); }
};
template<> struct y_hash<const char*> {
	size_t operator()(const char* s) const { return __stl_hash_string(s); }
};
template<> struct y_hash<char> {
	size_t operator()(char x) const { return x; }
};
template<> struct y_hash<unsigned char> {
	size_t operator()(unsigned char x) const { return x; }
};
//...各种数据类型特化，写法都一样，略过