// stl_src_analysis_03.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>

/**
 * 迭代器与traits编程
 *
 * 迭代器模式提供一种方法，使之能够依序访问某个容器内各元素又无需暴露容器内部表达方式
 * 它将容器和算法分开独立设计，又能很好的结合使用
 */
using namespace std;
void test_sample() {
	//以find为例看迭代器如何作用在容器和算法之间
	const int arr_size = 7;
	int ia[arr_size] = { 0,1,2,3,4,5,6 };
	vector<int> ivec(ia, ia + arr_size);
	//接受两个迭代器及目标值，返回目标值的迭代器
	auto it_vec = find(ivec.begin(), ivec.end(), 4);
	if (it_vec == ivec.end())
		cout << "4 not found in ivec" << endl;
	else
		cout << "4 found in ivec" << endl;
}
/**
 * 迭代器型为类似指针
 * 最重要的是对operator*和operator->进行重载
 * 先简单模仿下智能指针，见：y_smart_ptr.h
 * 之后自己实现一个链表的迭代器
 */
#include "y_list.h"
void test_iter() {
	//使用方式
	List<int> mylist;
	for (int i = 0; i < 5; ++i)
	{
		mylist.insert_end(i);
	}
	ListIter<ListItem<int>> begin(mylist.front());
	ListIter<ListItem<int>> end;
	ListIter<ListItem<int>> iter;
	iter = find(begin, end, 3);
	if (iter == end)
		cout << "not found" << endl;
	else
		cout << "found" << endl;
}
/**
 * 在算法中运用迭代器时，会用到相应类型
 * 而c++没有typeof()，用typeid()也不够用
 * 解决办法是用参数推导机制
 */
template<class I, class T>
void func_impl(I iter, T t) //编译器会自动进行参数推导
{
	T temp; //这里解决了迭代器所指类型问题

	//...func()原本的工作
}
//封装
template<class I>
void func(I iter) {
	func_impl(iter, *iter);
}
void test_func() {
	int i = 0;
	func(&i);
}
/**
 * 除了上面情况
 * 迭代器相应型别并不只有“迭代器所指对象类型”一种
 * 并非所有情况都能用参数推导机制取得，还需要其他方式
 * 例如，当value type必须用于函数返回值，参数推导就不够了
 */
template<class T>
struct y_iter {
	typedef T value_type;
	T* ptr;
	y_iter(T* p = 0) :ptr(p) {}
	T& operator*() const { return *ptr; }
	//...
};
template<class I>
typename I::value_type //这里必须加typename，T本身是个模板参数，在被编译器具象化之前，并不知道y_iter<T>::value_type是类型还是成员，所以需要显式告知编译器
func_return(I iter)
{
	return *iter;
}
void test_func_return() {
	y_iter<int> iter(new int(8));
	cout << func_return(iter) << endl;
}
/**
 * 上面是一个方案，但并非所有迭代器都是class type
 * 如果不是class type（例如原生指针）就无法定义内嵌类型
 * 但STL泛型思维必须能够接受原生指针作为迭代器
 * 所以，需要使用到template partial specialization模板偏特化
 * 如果有一个以上模板参数，可以针对某些参数进行特化；基于此，可以针对“迭代器模板参数为指针”者，设计特化版迭代器
 * 例如：
 */
template<class I>
struct y_iterator_traits_v {
	typedef typename I::value_type value_type;//如果I定义有自己的value type，那么提取出来的value_type就是I::value_type
};
template<class I>
typename y_iterator_traits_v<I>::value_type
func_traits(I iter) {
	return *iter;
}
//偏特化，应对迭代器是原生指针
template<class T>
struct y_iterator_traits_v<T*> {
	typedef T value_type;
};
//针对pointer-to-const
template<class T>
struct y_iterator_traits_v<const T*> {
	typedef T value_type;
};
//至此，无论是迭代器、原生指针、const指针，都可以通过traits取出正确的value_type
//最常用的五种型别为：value type,difference type,pointer,reference,iterator catagoly
//完整traits
#include "y_traits.h"
void test_traits() {

}

int main()
{
	test_func_return();
}

