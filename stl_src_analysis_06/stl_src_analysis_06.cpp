// stl_src_analysis_06.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

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


int main()
{
	std::cout << "Hello World!\n";
}

