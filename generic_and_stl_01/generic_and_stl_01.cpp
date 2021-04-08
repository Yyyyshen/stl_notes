// generic_and_stl_01.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

/**
 * 参考《泛型编程与STL》
 */
using namespace std;
//第2章 算法与区间
//从简单的例子开始
char* y_strchr(char* s, int c) {
	//一个标准C函数，查找字符串中c出现的第一个位置，是一种线性查找
	while (*s != '\0' && *s != c)
		++s;
	return *s == c ? s : (char*)0;
}
//而上述函数想要做到一般化，那么首先要确定指针指向数据的尾端，常见的方式是提供size计数或提供一个指向结尾的指针
char* y_find(char* first, char* last, int c) {
	//使用指向序列结尾的指针比size计数方式更恰当
	//因为，累加指针比维护一个计数值要容易，另外指针比较有一致性，函数参数和返回值都具有相同的表示方式，就可以更加泛化
	while (first != last && *first != c)
		++first;
	return first;
}
//使用时（这里的方式就很像迭代器了）
void test_find() {
	char c[5] = { 'a','b','c','d','e' };
	char* ret = y_find(c, c + 5, 'f');
	if (ret == c + 5) // 以arr+size方式表示结尾，是一种比较特殊的pass-the-end指针，不能进行取值操作，但可用于指针运算、做对比操作
		cout << "not find" << endl;
	else
		cout << "find" << endl;
}
//C中只能使用基本类型，C++则可以使用模板实现泛型化
template<class Iterator, class T>
Iterator y_find(Iterator first, Iterator last, const T& value) {
	while (first != last && *first != value)
		++first;
	return first;
}
//再增加一个满足条件元素
template<class InputIterator, class Predicate>
InputIterator y_find_if(InputIterator first, InputIterator last, Predicate pred) {
	while (first != last && !pred(*first))
		++first;
	return first;
}
//很多是跟《STL剖析》重复的内容，简单看看，巩固一下即可


int main()
{
	test_find();
}

