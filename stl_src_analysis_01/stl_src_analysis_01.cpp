// stl_src_analysis_01.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

/**
 * 参考：《STL源码剖析》（侯杰）
 */
using namespace std;
#include <vector>
#include <algorithm>
/**
 * 六大组件
 * 概览
 *
 * 容器(containers):常用类模板，如vector、list、deque、set、map等
 *
 * 算法(algorithms):常用函数模板，如sort、search、copy、erase等
 *
 * 迭代器(iterators):处在容器和算法中间，泛型指针，将各种指针相关操作予以重载（原生指针也是一种迭代器）
 *
 * 仿函数(functors):行为类似函数，作为算法的某种策略，实现角度是重载了()操作符（一般函数指针可视为狭义仿函数）
 *
 * 适配器(adapters):修饰容器或仿函数或迭代器接口的东西，如queue、stack看似容器，其实只能算底层基于deque的适配器
 *
 * 配置器(allocators):负责空间配置和管理，实现上看是一个动态分配空间、管理空间、释放空间的类模板
 *
 * 按c++标准，头文件没有扩展名，某些版本同时存在.h和无扩展名版，还有一些只有.h版本
 * 可读性较强的开源版本是GNU C++所附的SGI STL，可以在GCC的include子目录下找到头文件
 */
void* foo = nullptr;
//从一些c++语法开始
template <typename T>
class print {
public:
	void operator()(const T& elem) {
		cout << elem << ' ';
	}
};
//临时对象；STL常将此技巧应用于仿函数和算法
void test_temp_obj() {
	int ia[6] = { 0,1,2,3,4,5 };
	vector<int> iv(ia, ia + 6);
	//print<int>()表示一个临时对象，而不是一个函数调用操作，for_each结束则对象生命周期结束
	for_each(iv.begin(), iv.end(), print<int>());
}
//静态常量整型成员在class内部直接初始化
template <typename T>
class integerClass {
public:
	static const int _datai = 5;
	static const long _datal = 3L;
	static const char _datac = 'a';
};
void testIntegerClass() {
	cout << integerClass<int>::_datac << endl;
}
//操作符increment/decremenet/dereference
//迭代器都需要实现前进(++)和取值(*)操作
class INT {
	friend ostream& operator<<(ostream& os, const INT& i);
public:
	INT(int i) :m_i(i) {};
	//前置++，返回自增后的值
	INT& operator++()
	{
		++(this->m_i);
		return *this;
	}
	const INT operator++(int)
	{
		INT temp = *this;
		++(*this);
		return temp;//后置++，返回原值
	}
	//前后置--，同++
	INT& operator--()
	{
		--(this->m_i);
		return *this;
	}
	const INT operator--(int)
	{
		INT temp = *this;
		--(*this);
		return temp;
	}
	//dereference取值
	int& operator*() const {
		return (int&)m_i;
	}
private:
	int m_i;
};

ostream& operator<<(ostream& os, const INT& i) {
	os << '[' << i.m_i << ']';
	return os;
}

void test_INT() {
	INT I(5);
	cout << I++;
	cout << ++I;
	cout << I--;
	cout << --I;
	cout << *I;
}
//前闭后开区间表示法
//任何一个STL算法都需要获得由迭代器标示的区间[first,last)
//last表示最后一个元素的下一个位置
template <class InputIterator, class T>
InputIterator my_find(InputIterator first, InputIterator last, const T& value)
{
	while (first != last && *first != value) ++first;
	return first;
}
template<class InputIterator, class Function>
Function my_for_each(InputIterator first, InputIterator last, Function f) {
	for (; first != last; ++first)
		f(*first);
	return f;
}
//operator()重载
template<class T>
struct my_plus { 
	//重载了()操作符，结构体形成了一个仿函数
	T operator()(const T& x, const T& y) const { return x + y; }
};
void test_func_call() {
	my_plus<int> plusobj;
	cout << plusobj(3, 5) << endl;//仿函数
	cout << my_plus<int>()/*临时对象*/(3, 5)/*调用*/ << endl;
}


int main()
{
	//test_temp_obj();
	//test_INT();
	test_func_call();
}

