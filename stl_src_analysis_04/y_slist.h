#pragma once

/**
 * slist
 * ����STLϰ�ߣ���������Ὣ��Ԫ�ز�����ָ��λ��֮ǰ��Ҳ���ǲ��뵽ָ��λ�ã�ָ��λ��ԭԪ������ƣ�
 * �����������޷�֪��ǰһ��λ�ã�����Ҫ��ͷ����Ч�ʲ��ߣ������ṩ��insert_after��erase_after
 * ���ң�ֻ�ṩpush_front��Ԫ�ش��������˳���෴
 */

struct __y_slist_node_base {
	__y_slist_node_base* next;
};
//�ڵ�͵�������Ʊ�list���ӣ������˼̳�
template<class T>
struct __y_slist_node :public __y_slist_node_base {
	T data;
};
//ȫ�ֺ�������֪ĳ�ڵ㣬�����½ڵ������
inline __y_slist_node_base* __y_slist_make_link(__y_slist_node_base* prev_node, __y_slist_node_base* new_node) {
	new_node->next = prev_node->next;
	prev_node->next = new_node;
	return new_node;
}
//�������С
inline size_t __y_slist_size(__y_slist_node_base* node) {
	size_t result = 0;
	for (; node != 0; node = node->next)
		++result;
	return result;
}
//���������
struct __y_slist_iterator_base {
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef forward_iterator_tag iterator_category;

	__y_slist_node_base* node;//ָ��ڵ�����ṹ
	__y_slist_iterator_base(__y_slist_node_base* x) :node(x) {}

	void incr() { node = node->next; }
	bool operator==(const __y_slist_iterator_base& x) const { return node == x.node; }
	bool operator!=(const __y_slist_iterator_base& x) const { return node != x.node; }
};
template<class T, class Ref, class Ptr>
struct __y_slist_iteraotr :public __y_slist_iterator_base {
	typedef __y_slist_iteraotr<T, T&, T*> iterator;
	typedef __y_slist_iteraotr<T, const T&, const T*> const_iterator;
	typedef __y_slist_iteraotr<T, Ref, Ptr> self;

	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef __y_slist_node<T> list_node;

	__y_slist_iteraotr(list_node* x) :__y_slist_iterator_base(x) {}
	__y_slist_iteraotr() :__y_slist_iterator_base(0) {}
	__y_slist_iteraotr(const iterator& x) :__y_slist_iterator_base(x.node) {}

	reference operator*() const { return ((list_node*)node)->data; }
	pointer operator->() const { return &(operator*()); }
	self& operator++() {
		//ǰ��++
		incr();
		return *this;
	}
	self operator++(int) {
		//����++
		self temp = *this;
		incr();
		return temp;
	}
	//��ʵ��--����Ϊ��forward iterator
	//δ����==�������ڱȽ�slist�������Ƿ���ͬʱ�������__y_slist_iterator_base��==������node�Ƿ���ͬ����
};
//slist���ݽṹ
template<class T, class Alloc = y_alloc>
class y_slist {
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef __y_slist_iteraotr<T, T&, T*> iterator;
	typedef __y_slist_iteraotr<T, const T&, const T*> const_iterator;

private:
	typedef __y_slist_node<T> list_node;
	typedef __y_slist_node_base list_node_base;
	typedef __y_slist_iterator_base iterator_base;
	typedef y_simple_alloc<list_node, Alloc> list_node_allocator;

	static list_node* create_node(const value_type& x) {
		list_node* node = list_node_allocator::allocate();//���ÿռ�
		try {
			construct(&node->data, x);//����Ԫ��
			node->next = 0;
		}
		catch (...) {
			list_node_allocator::deallocate();//����ʧ�����ͷſռ�
		}
		return node;
	}
	static void destroy_node(list_node* node) {
		destroy(&node->data);//����
		list_node_allocator::deallocate(node);//�ͷ�
	}
private:
	list_node_base head;//ͷ��������ָ�룩
public:
	y_slist() { head.next = 0; }
	~y_slist() { clear(); }
public:
	iterator begin() { return iterator((list_node*)head.next); }
	iterator end() { return iterator(0); }
	size_type size() const { return __y_slist_size(head.next); }
	bool empty() const { return head.next == 0; }

	//��������slist����ʵ���ǽ���head��ָ
	void swap(y_slist& L) {
		list_node_base* temp = head.next;
		head.next = L.head.next;
		L.head.next = temp;
	}
public:
	reference front() { return ((list_node*)head.next)->data; }
	void push_front(const value_type& x) {
		__y_slist_make_link(&head, create_node(x));
	}
	void pop_front() {
		list_node* node = (list_node*)head.next;
		head.next = node->next;
		destroy_node(node);
	}
};