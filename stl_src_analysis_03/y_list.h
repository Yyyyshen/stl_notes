#pragma once

/**
 * 单链表基本结构
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
 * 为上述结构设计一个迭代器时
 * 行为应该类似指针
 * dereference取值时，应返回一个ListItem对象；++递增时，应该指向下一个ListItem对象
 */

template<class Item>
struct ListIter {
	Item* ptr;//保持与容器的联系
	ListIter(Item* p = 0) :ptr(p) {  }
	//缺省的copy ctor和operator=由编译器提供，不需要重载
	//指针操作重载
	Item& operator*() const { return *ptr; }
	Item* operator->() const { return ptr; }
	//前后置++重载
	ListIter& operator++() { ptr = ptr->next(); return *this; }
	ListIter operator++(int) { ListIter temp = *this; ++(*this); return temp; }
	//比较操作符重载
	bool operator==(const ListIter& i) const { return ptr == i.ptr; }
	bool operator!=(const ListIter& i) const { return ptr != i.ptr; }
};

//由于find函数内用比较操作符检查元素，需要全局重载
template <typename T>
bool operator!=(const ListItem<T>& item, T n) {
	return item.value() != n;
}
template <typename T>
bool operator==(const ListItem<T>& item, T n) {
	return item.value() == n;
}

/**
 * 为了实现针对List的Iter，无可避免的暴露了List实现细节
 * 所以，为了实现封装，STL容器都提供专属迭代器
 */