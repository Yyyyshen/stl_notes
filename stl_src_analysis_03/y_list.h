#pragma once

/**
 * ����������ṹ
 */

template<typename T>
class ListItem {
public:
	T value() const { return _value; }
	ListItem* next() const { return _next; }
	//...
private:
	T _value;
	ListItem* _next;
};

template<typename T>
class List {
public:
	void insert_front(T value) { /**/ };
	void insert_end(T value) { /**/ };
	void display(std::ostream& os = std::cout)const { /**/ };
	ListItem<T>* front() { return _front; }
	//...
private:
	ListItem<T>* _end;
	ListItem<T>* _front;
	long _size;
};

/**
 * Ϊ�����ṹ���һ��������ʱ
 * ��ΪӦ������ָ��
 * dereferenceȡֵʱ��Ӧ����һ��ListItem����++����ʱ��Ӧ��ָ����һ��ListItem����
 */

template<class Item>
struct ListIter {
	Item* ptr;//��������������ϵ
	ListIter(Item* p = 0) :ptr(p) {  }
	//ȱʡ��copy ctor��operator=�ɱ������ṩ������Ҫ����
	//ָ���������
	Item& operator*() const { return *ptr; }
	Item* operator->() const { return ptr; }
	//ǰ����++����
	ListIter& operator++() { ptr = ptr->next(); return *this; }
	ListIter operator++(int) { ListIter temp = *this; ++(*this); return temp; }
	//�Ƚϲ���������
	bool operator==(const ListIter& i) const { return ptr == i.ptr; }
	bool operator!=(const ListIter& i) const { return ptr != i.ptr; }
};

//����find�������ñȽϲ��������Ԫ�أ���Ҫȫ������
template <typename T>
bool operator!=(const ListItem<T>& item, T n) {
	return item.value() != n;
}
template <typename T>
bool operator==(const ListItem<T>& item, T n) {
	return item.value() == n;
}

/**
 * Ϊ��ʵ�����List��Iter���޿ɱ���ı�¶��Listʵ��ϸ��
 * ���ԣ�Ϊ��ʵ�ַ�װ��STL�������ṩר��������
 */