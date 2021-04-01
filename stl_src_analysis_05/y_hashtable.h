#pragma once

/**
 * hashtable
 *
 * ���ݿ���������ϣ����ÿ��Ԫ�ز�ֻ��һ���ڵ�Ԫ�أ���������һ��Ͱ��bucket���ڵ㣬Ͱ�ڵ���һ��list
 */

template <class Value>
struct __hashtable_node {
	__hashtable_node* next;
	Value val;
};

template<class Value, class Key, class HashFcn, class ExtracKey, class EqualKey, class Alloc> class hashtable;

//���������
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

	node* cur;//������Ŀǰ��ָ�ڵ�
	hashtable* ht;//�������������ϵ

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
template<class Value/*ʵֵ*/, class Key/*��ֵ*/, class HashFcn/*ɢ�к���*/, class ExtracKey/*ȡ��ֵ*/, class EqualKey/*�жϼ�ֵ�Ƿ����*/, class Alloc>
class hashtable {
public:
	typedef HashFcn hasher;
	typedef EqualKey key_equal;
	typedef size_t size_type;
	typedef Value value_type;
	typedef Key key_type;
private:
	//������������
	hasher hash;
	key_equal equals;
	ExtracKey get_key;

	typedef __hashtable_node<Value> node;
	typedef y_simple_alloc<node, Alloc> node_allocator;
	typedef __hashtable_iterator<Value, Key, HashFcn, ExtracKey, EqualKey, Alloc> iterator;
	//ά��Ͱ�ڵ㣬ֱ����vector��Ϊbuckets
	vector<node, Alloc> buckets;
	size_type num_elements;
public:
	size_type bucket_count() const { return buckets.size(); }
	//...
	node* new_node(const value_type& obj) {
		node* n = node_allocator::allocate();
		n->next = 0;
		try {
			construct(&n->val, obj);//���·���ڵ�ռ��Ϲ���Ԫ��
			return n;
		}
		catch (...) {
			//ʧ������տռ�
			node_allocator::deallocate(n);
		}
	}
	void delete_node(node* n) {
		destroy(&n->val);//����Ԫ��
		node_allocator::deallocate(n);//�ͷſռ�
	}
	//���캯��
	hashtable(size_type n, const HashFcn& hf, const EqualKey& eql) :hash(hf), equals(eql), get_key(ExtracKey()), num_elements(0) { initialize_buckets(n); }
	void initialize_buckets(size_type n) {
		const size_type n_buckets = next_size(n);//���紫50������53
		buckets.reserve(n_buckets);
		buckets.insert(buckets.end(), n_buckets, (node*)0);
		num_elements = 0;
	}
	size_type next_size(size_type n) const { return n;/*����ʡ���ˣ�ʵ��Ӧ���Ƿ��ش���n����ӽ�n������*/ }
	//����������������ظ�
	pair<iterator, bool> insert_unique(const value_type& obj) {
		resize(num_elements + 1);//�ж��Ƿ���Ҫ�ؽ����
		return insert_unique_noresize(obj);
	}
	void resize(size_type num_elements_hint) {
		//�ж�ԭ���ǽ�Ԫ�ظ��������������Դ���������+1����bucket��С�Ƚϣ����ǰ�ߴ����ؽ�
		const size_type old_n = buckets.size();
		if (num_elements_hint > old_n)
		{
			const size_type n = next_size(num_elements_hint);//�ҳ���һ������
			if (n > old_n)
			{
				vector<node*, Alloc> temp(n, (node*)0);//�����µ�buckets
				try {
					for (size_type bucket = 0; bucket < old_n; ++bucket)
					{
						node* first = buckets[bucket];//����ÿ��bucket
						while (first) {
							//�ҳ��ڵ������ĸ���bucket��
							size_type new_bucket = bkt_num(first->val, n);
							buckets[bucket] = first->next;//��bucketָ���Ӧ���е���һ���ڵ㣨������洦��ѭ����
							first->next = temp[new_bucket];
							temp[new_bucket] = first;//����ǰ�ڵ������bucket����Ϊ��Ӧ���еĵ�һ���ڵ�
							first = buckets[bucket];//�ص��ɵ�bucket��ָ�������У�׼��������һ���ڵ�
						}
					}
					buckets.swap(temp);//�¾ɶԵ�
				}
				catch (...) {

				}
			}
		}
	}
	pair<iterator, bool> insert_unique_noresize(const value_type& obj) {
		const size_type n = bkt_num(obj);//����objӦλ�ڵ�n��bucket
		node* first = buckets[n];//ָ���Ӧ����ͷ������һЩ�ж�
		for (node* cur = first; cur; cur = cur->next)
			if (equals(get_key(cur->val), get_key(obj)))
				return pair<iterator, bool>(iterator(cur, this), false);//�ȱ���һ�飬�����ظ���ֱ�ӷ���ʧ��
		//����ѭ�����������ֵ
		node* temp = new_node(obj);
		temp->next = first;//�����½ڵ㲢�����Ϊ��ǰ����ͷ
		buckets[n] = temp;
		++num_elements;//�ۼ�Ԫ�ظ�������
		return pair<iterator, bool>(iterator(temp, this), true);
	}
	//�����ظ��Ĳ������
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
				//������ͬ�ģ�����󷵻�
				node* temp = new_node(obj);
				temp->next = cur->next;
				cur->next = temp;
				++num_elements;
				return iterator(temp, this);
			}
		//û���ظ��������½ڵ�
		node* temp = new_node(obj);
		temp->next = first;
		buckets[n] = temp;
		++num_elements;
		return iterator(temp, this);
	}
	//�ж�Ԫ�ظ����ĸ�Ͱ��
	size_type bkt_num(const value_type& obj, size_t n) const { return bkt_num_key(get_key(obj), n); }//����ʵֵ��buckets����
	size_type bkt_num(const value_type& obj) const { return bkt_num_key(get_key(obj)); }//ֻ����ʵֵ
	size_type bkt_num_key(const key_type& key) const { return bkt_num_key(key, buckets.size()); }//���ռ�ֵ
	size_type bkt_num_key(const key_type& key, size_t n) const { return hash(key) % n; }//���ռ�ֵ��buckets�������ù�ϣ��������
	//������ɾ��
	void clear() {
		//���ÿһ��bucket����list��ÿ��Ԫ��ɾ��
		for (size_type i = 0; i < buckets.size(); ++i) {
			node* cur = buckets[i];
			while (cur != 0) {
				node* next = cur->next;
				delete_node(cur);
				cur = next;
			}
			buckets[i] = 0;//�ÿ�ָ��
		}
		num_elements = 0;//Ԫ�ظ�����0
		//vector����û���ͷţ��Ա���ԭ�д�С
	}
	void copy_from(const hashtable& ht) {
		//�Ƚ��Լ���ղ��������ܹ����նԷ���״̬
		buckets.clear();
		buckets.reserve(ht.buckets.size());//reserveֻ�ı�ռ䣬resize�ı�size������һ��reserve��������Ҫinsert
		buckets.insert(buckets.end(), ht.buckets.size(), (node*)0);
		try {
			for (size_type i = 0; i < ht.buckets.size(); ++i)
			{
				//����ÿ��Ԫ�ع���
				if (const node* cur = ht.buckets[i])
				{
					node* copy = new_node(cur->val);
					buckets[i] = copy;
					//��ÿ��Ͱ��listԪ�ظ�ֵ
					for (node* next = cur->next; next; cur = next, next = cur->next) {
						copy->next = new_node(next->val);
						copy = copy->next;
					}
				}
			}
			num_elements = ht.num_elements;//��¼�ڵ���
		}
		catch (...) {
			clear();
		}
	}
	//��ѯ���
	iterator find(const key_type& key) {
		size_type n = bkt_num_key(key);
		node* first;
		for (first = buckets[n]; first && !equals(get_key(first->val), key); first = first->next)
		{
			//����ֱ����ѭ����ֹ�����жԱȼ�ֵ���ɹ��򷵻�
		}
		return iterator(first, this);
	}
	size_type count(const key_type& key) const {
		const size_type n = bkt_num_key(key);//�ȿ����ĸ�Ͱ��
		size_type result = 0;
		//���������м�����ͬ��
		for (const node* cur = buckets[n]; cur; cur = cur->next)
			if (equals(get_key(cur->val), key))
				++result;
		return result;
	}
};

//һЩ�����Ĺ�ϣ����
template<class Key> struct y_hash {  };

inline size_t __stl_hash_string(const char* s)
{
	unsigned long h = 0;
	for (; *s; ++s)
		h = 5 * h + *s;//��һ�������ۼ�ÿ���ֽ�
	return size_t(h);
}
//����ģ���ػ�������������㺯��дһЩ�º���
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
//...�������������ػ���д����һ�����Թ�