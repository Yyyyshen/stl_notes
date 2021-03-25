#pragma once

/**
 * �˽�listʵ��
 */

template<class T>
struct __y_list_node {
	//�ڵ����
	typedef void* void_pointer;
	void_pointer prev; //˫������
	void_pointer next;
	T data;
};

//���ڲ��������ռ䣬�޷�����ָͨ����Ϊ��������list������Ӧ��ָ��list�ڵ�������������ܹ��������ݼ���ȡֵ����Ա��ȡ��
//˫�������ܹ�ǰ���ƣ�����list�ṩ����Bidirectional Iterators
//list��insert��splice���Ӻϣ����������ԭlist������ʧЧ��ɾ������ʱҲֻ��ָ��ɾ��Ԫ�صĵ�����ʧЧ
struct y_bidirectional_iterator_tag { /* ���������ͱ�� */ };
template<class T, class Ref, class Ptr>
struct __y_list_iterator {
	//���������
	typedef __y_list_iterator<T, T&, T*> iterator;
	typedef __y_list_iterator<T, Ref, Ptr> self;
	typedef y_bidirectional_iterator_tag iterator_category;
	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef __y_list_node<T>* link_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	link_type node; // �������ڲ���һ����ָͨ��ָ��list�ڵ�

	//����
	__y_list_iterator() {}
	__y_list_iterator(link_type x) : node(x) {}
	__y_list_iterator(const iterator& x) : node(x.node) {}
	//���ز�����
	bool operator==(const self& x) const { return node == x.node; }
	bool operator!=(const self& x) const { return node != x.node; }
	reference operator*() const { return (*node).data; } //������ȡֵ
	pointer operator->() const { return &(operator*()); } //��ָԪ�ص�ֵ�ĵ�ַ
	self& operator++() {
		//�������������ڵ�ǰ��1
		node = (link_type)((*node).next);
		return *this;
	}
	self operator++(int) {
		//��������
		self temp = *this;
		++(*this);
		return temp;
	}
	self& operator--() {
		//�������Լ����ڵ����1
		node = (link_type)((*node).prev);
		return *this;
	}
	self operator--(int) {
		//����--
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
	link_type node;//��״˫������ֻ��Ҫһ��ָ�룬�����ڻ�״����β�˼�һ���հ׽ڵ㣬���ܷ���STLǰ�պ�Ҫ�󣬳�Ϊlast������
	iterator begin() { return (link_type)((*node).next); }
	iterator end() { return node; }
	bool empty() const { return node->next == node; }
	size_type size() const {
		size_type result = 0;
		distance(begin(), end(), result);
		return result;
	}
	//...
	//���������������

	//Ǩ�Ʋ���
	void transfer(iterator position, iterator first, iterator last) {
		//��ĳ��Χ��Ԫ��ǰ�Ƶ�ĳ�ض�λ��
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
		//�Ը÷���Ϊ��׼���ɷǳ������ʵ��splice��merge��reverse��sosrt�ȷ���
	}
};