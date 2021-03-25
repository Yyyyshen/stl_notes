#pragma once

/**
 * 了解list实现
 */

template<class T>
struct __y_list_node {
	//节点设计
	typedef void* void_pointer;
	void_pointer prev; //双向链表
	void_pointer next;
	T data;
};

//由于不是连续空间，无法以普通指针作为迭代器，list迭代器应有指向list节点的能力，并且能够递增、递减、取值、成员存取等
//双向链表能够前后移，所以list提供的是Bidirectional Iterators
//list的insert、splice（接合）都不会造成原list迭代器失效，删除操作时也只有指向被删除元素的迭代器失效
struct y_bidirectional_iterator_tag { /* 迭代器类型标记 */ };
template<class T, class Ref, class Ptr>
struct __y_list_iterator {
	//迭代器设计
	typedef __y_list_iterator<T, T&, T*> iterator;
	typedef __y_list_iterator<T, Ref, Ptr> self;
	typedef y_bidirectional_iterator_tag iterator_category;
	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef __y_list_node<T>* link_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	link_type node; // 迭代器内部有一个普通指针指向list节点

	//构造
	__y_list_iterator() {}
	__y_list_iterator(link_type x) : node(x) {}
	__y_list_iterator(const iterator& x) : node(x.node) {}
	//重载操作符
	bool operator==(const self& x) const { return node == x.node; }
	bool operator!=(const self& x) const { return node != x.node; }
	reference operator*() const { return (*node).data; } //解引用取值
	pointer operator->() const { return &(operator*()); } //所指元素的值的地址
	self& operator++() {
		//迭代器自增，节点前进1
		node = (link_type)((*node).next);
		return *this;
	}
	self operator++(int) {
		//后置自增
		self temp = *this;
		++(*this);
		return temp;
	}
	self& operator--() {
		//迭代器自减，节点后退1
		node = (link_type)((*node).prev);
		return *this;
	}
	self operator--(int) {
		//后置--
		self temp = *this;
		--(*this);
		return temp;
	}

};

template<class T, class Alloc = y_alloc>
class y_list {
protected:
	typedef __y_list_node<T> list_node;
	typedef __y_list_iterator<T, T&, T*> iterator;
	typedef size_t size_type;
public:
	typedef list_node* link_type;
protected:
	link_type node;//环状双向链表，只需要一个指针，刻意在环状链表尾端加一个空白节点，就能符合STL前闭后开要求，称为last迭代器
	iterator begin() { return (link_type)((*node).next); }
	iterator end() { return node; }
	bool empty() const { return node->next == node; }
	size_type size() const {
		size_type result = 0;
		distance(begin(), end(), result);
		return result;
	}
	//...
	//大多数操作都类似

	//迁移操作
	void transfer(iterator position, iterator first, iterator last) {
		//将某范围内元素前移到某特定位置
		if (position != last)
		{
			(*((link_type)((*last.node).prev))).next = position.node;
			(*((link_type)((*first.node).prev))).next = last.node;
			(*((link_type)((*position.node).prev))).next = first.node;
			link_type tmp = link_type((*position.node).prev);
			(*position.node).prev = (*last.node).prev;
			(*last.node).prev = (*first.node).prev;
			(*first.node).prev = tmp;
		}
		//以该方法为基准，可非常方便的实现splice、merge、reverse、sosrt等方法
	}
};