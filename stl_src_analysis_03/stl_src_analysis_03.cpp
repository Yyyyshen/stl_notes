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
 * 
 */


int main()
{
	std::cout << "Hello World!\n";
}

