// stl_src_analysis_08.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

/**
 * 适配器
 */
using namespace std;
/**
 * 概念和分类
 * 应用于容器
 *	前面看过的queue和stack就是基于deque的容器适配器
 * 应用于仿函数
 *	由于仿函数是operator()操作符的重载，而对于一般函数或者成员函数要想无缝与其他适配器或算法结合，stl提供了很多仿函数适配器
 * 应用于迭代器
 *	insert iterators，将一般迭代器的赋值操作转为插入操作
 *	reverse iterators，将一般迭代器的行进方向逆转，使原本前进的++变成后退操作，反之亦然
 *	iostream iterators，将迭代器绑定到iostream对象上。例如绑定std::cout，方便输出
 */
void test_adapter() {

}
/**
 * container adapters
 */
void test_container_adapter() {
	//在前面容器章节实现过，略
}
/**
 * iterator adapters
 */
#include "y_iterator_adapters.h"
void test_iterator_adapter() {

}

int main()
{
	std::cout << "Hello World!\n";
}
