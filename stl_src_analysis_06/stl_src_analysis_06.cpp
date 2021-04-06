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

int main()
{
	test_numeric();
}

