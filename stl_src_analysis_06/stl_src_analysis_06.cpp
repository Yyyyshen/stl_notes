// stl_src_analysis_06.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <functional>
using namespace std;
/**
 * 算法
 */
int* arr = nullptr;
//对于一个数组，查询其中特定值
int* find(int* arr_head, int arr_size, int value) {
	int i;
	for (i = 0; i < arr_size; ++i)
		if (arr_head[i] == value)
			break;
	return &(arr_head[i]);
}
//这样的作法暴露了太多容器实现细节，如需要传递数组大小，应该更加抽象化
int* find(int* begin, int* end, int value) {
	while (begin != end && *begin != value)
		++begin;
	return begin;
}
//上面只针对整数型，可以改成模板
template<typename T>
T* find(T* begin, T* end, const T& value) {
	while (begin != end && *begin != value)
		++begin;
	return begin;
}
//跳出原生指针的思想，完全泛化
template<class Iterator, class T>
Iterator find(Iterator begin, Iterator end, const T& value) {
	while (begin != end && *begin != value)
		++begin;
	return begin;
}

/**
 * 数值算法
 */
#include "y_numeric.h"
#include <numeric>
void test_numeric() {
	int ia[5] = { 1,2,3,4,5 };
	vector<int> iv(ia, ia + 5);
	cout << accumulate(iv.begin(), iv.end(), 0) << endl;//15
	cout << accumulate(iv.begin(), iv.end(), 0, minus<int>()) << endl;//-15
	cout << inner_product(iv.begin(), iv.end(), iv.begin(), 10) << endl;//10+1*1+2*2+3*3+4*4+5*5=65
	cout << inner_product(iv.begin(), iv.end(), iv.begin(), 10, minus<int>(), plus<int>()) << endl;//10-1+1-2+2-3+3-4+4-5+5=-20
	//迭代器绑定到cout，作为输出格式使用
	ostream_iterator<int> oite(cout, " ");
	partial_sum(iv.begin(), iv.end(), oite);//1 3 6 10 15,每个元素是前面几个元素相加
	cout << endl;
	partial_sum(iv.begin(), iv.end(), oite, minus<int>());//改变操作，1 -1 -4 -8 -13
	cout << endl;
	adjacent_difference(iv.begin(), iv.end(), oite);//1 1 1 1 1 (第一个元素直接输出，后面每个元素等于当前位置-前一个位置元素）
	cout << endl;
	adjacent_difference(iv.begin(), iv.end(), oite, plus<int>());//改变操作，1 3 5 7 9
	cout << endl;
	cout << y_power(10, 3) << endl;//幂，1000
	cout << y_power(10, 3, plus<int>()) << endl;//改变操作，30
	int n = 3;
	iota(iv.begin(), iv.end(), n);//向区间内填入n,n+1,n+2...
	for (int i = 0; i < iv.size(); ++i)
		cout << iv[i] << ' ';
	cout << endl;
}
/**
 * 基本算法
 */
#include "y_algobase.h"
#include <algorithm>
void test_algobase() {
	int ia[9] = { 0,1,2,3,4,5,6,7,8 };
	vector<int> iv1(ia, ia + 5);
	vector<int> iv2(ia, ia + 9);
	//cout << *(mismatch(iv1.begin(), iv1.end(), iv2.begin()).first) << endl;
	cout << *(mismatch(iv1.begin(), iv1.end(), iv2.begin()).second) << endl;//判断两个区间的第一个不匹配点（实际操作应加上是否为end()判断）
	cout << equal(iv1.begin(), iv1.end(), iv2.begin()) << endl;//判断区间内是否相同，第二序列多出来的元素忽略
	cout << equal(iv1.begin(), iv1.end(), &ia[3], less<int>())<<endl;//true;{0,1,2,3,4}小于{3,4,5,6,7}
	fill(iv1.begin(), iv1.end(), 9);//将iv1填充9
	for_each(iv1.begin(), iv1.end(), display<int>());//display为仿函数，输出元素；9 9 9 9 9
	cout << endl;
	fill_n(iv1.begin(), 3, 7);//从迭代器指定位置，填写3个7
	for_each(iv1.begin(), iv1.end(), display<int>());//for_each为遍历算法；7 7 7 9 9
	cout << endl;
	vector<int>::iterator ite1 = iv1.begin();//指向7
	vector<int>::iterator ite2 = ite1;
	advance(ite2, 3);//指向9
	iter_swap(ite1, ite2);//交换两迭代器所指元素
	for_each(iv1.begin(), iv1.end(), display<int>());//9 7 7 7 9
	cout << endl;
	cout << max(*ite1, *ite2) << endl;//9(*ite1)
	cout << min(*ite1, *ite2) << endl;//7(*ite2)
	swap(*iv1.begin(), *iv2.begin());//交换两数值
	for_each(iv1.begin(), iv1.end(), display<int>());//0 7 7 7 9
	cout << endl;
	for_each(iv2.begin(), iv2.end(), display<int>());//0 1 2 3 4 5 6 7 8
	cout << endl;
	string str1[] = { "Yshen" };
	string str2[] = { "yshen" };
	cout << lexicographical_compare(str1, str1 + 1, str2, str2 + 1/* ,greater<string>() */) << endl;//1；比较字符串，大写字母更小，所以返回真
}

int main()
{
	//test_numeric();
	test_algobase();
}

