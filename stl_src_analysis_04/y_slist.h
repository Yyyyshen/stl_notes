#pragma once

/**
 * slist
 * 根据STL习惯，插入操作会将新元素插入于指定位置之前（也就是插入到指定位置，指定位置原元素向后移）
 * 而单向链表无法知道前一个位置，必须要从头找起，效率不高；所以提供了insert_after和erase_after
 * 并且，只提供push_front，元素次序与插入顺序相反
 */

struct __y_slist_node_base {
	__y_slist_node_base* next;
};
//节点和迭代器设计比list复杂，运用了继承
template<class T>
struct __y_slist_node :public __y_slist_node_base {
	T data;
};
//全局函数，已知某节点，插入新节点于其后
inline __y_slist_node_base* __y_slist_make_link(__y_slist_node_base* prev_node, __y_slist_node_base* new_node) {
	new_node->next = prev_node->next;
	prev_node->next = new_node;
	return new_node;
}
//单链表大小
inline size_t __y_slist_size(__y_slist_node_base* node) {
	size_t result = 0;
	for (; node != 0; node = node->next)
		++result;
	return result;
}
//迭代器设计
struct __y_slist_iterator_base {
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef forward_iterator_tag iterator_category;

	__y_slist_node_base* node;//指向节点基本结构
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
		//前置++
		incr();
		return *this;
	}
	self operator++(int) {
		//后置++
		self temp = *this;
		incr();
		return temp;
	}
	//不实现--，因为是forward iterator
	//未重载==，所以在比较slist迭代器是否相同时，会调用__y_slist_iterator_base的==，视其node是否相同而定
};
//slist数据结构
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
		list_node* node = list_node_allocator::allocate();//配置空间
		try {
			construct(&node->data, x);//构造元素
			node->next = 0;
		}
		catch (...) {
			list_node_allocator::deallocate();//构造失败则释放空间
		}
		return node;
	}
	static void destroy_node(list_node* node) {
		destroy(&node->data);//析构
		list_node_allocator::deallocate(node);//释放
	}
private:
	list_node_base head;//头部（不是指针）
public:
	y_slist() { head.next = 0; }
	~y_slist() { clear(); }
public:
	iterator begin() { return iterator((list_node*)head.next); }
	iterator end() { return iterator(0); }
	size_type size() const { return __y_slist_size(head.next); }
	bool empty() const { return head.next == 0; }

	//交换两个slist，其实就是交换head互指
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