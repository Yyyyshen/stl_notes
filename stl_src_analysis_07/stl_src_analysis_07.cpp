// stl_src_analysis_07.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

/**
 * 仿函数
 */
using namespace std;
/**
 * 函数对象
 * 实现上是类别定义中重载了operator()运算符，然后以临时对象的形式作为函数调用
 *
 * 仿函数分类
 * 以操作数划分为，一元、二元
 * 功能划分，算术、关系、逻辑
 */
void test_func_obj() {
	//两种调用方式
	greater<int> ig;
	cout << boolalpha << ig(4, 6) << endl;//false，手动创建对象
	cout << greater<int>()(6, 4) << endl;//true，使用临时对象
}
/**
 * 可适配性
 * 每个仿函数必须定义自己的相应型别（类似迭代器定义自己的型别）
 * 主要用来表现函数参数类型和返回值类型
 *
 * stl中定义了两个class，分别代表一元和二元仿函数，没有成员，只有类型定义
 * 其他仿函数只需要根据自己类型选择继承其中一个class，就可以实现可适配性
 */
template<class Arg, class Result>
struct y_unary_function {
	//一元函数参数类型和返回值类型定义
	typedef Arg argument_type;
	typedef Result result_type;
};
//示例
template<class T>
struct y_negate :public y_unary_function<T, T> {
	T operator()(const T& x) const { return -x; }
};
//仿函数适配器，在第八章还会提及
template<class Predicate>
class y_unary_negate {
public:
	bool operator()(const typename Predicate::argument_type& x) const {}
	//...
};

template<class Arg1, class Arg2, class Result>
struct y_binary_function {
	//二元函数类型定义
	typedef Arg1 first_argument_type;
	typedef Arg2 second_argument_type;
	typedef Result result_type;
};
//示例
template<class T>
struct y_plus :public y_binary_function<T, T, T> {
	T operator()(const T& x, const T& y) const { return x + y; }
};
//以下适配器用来将某个二元仿函数转化为一元仿函数
template<class Operation>
class y_binder1st {
protected:
	Operation op;
	typename Operation::first_argument_type value;
public:
	typename Operation::result_type operator()(const typename Operation::second_argument_type& x) const {}
};
/**
 * 算术类仿函数
 * 加减乘除，模运算（余数），负数等
 */
template<class T>
struct y_minus :public y_binary_function<T, T, T> {
	T operator()(const T& x, const T& y) const { return x - y; }
};
template<class T>
struct y_multiplies :public y_binary_function<T, T, T> {
	T operator()(const T& x, const T& y) const { return x * y; }
};
template<class T>
struct y_divides :public y_binary_function<T, T, T> {
	T operator()(const T& x, const T& y) const { return x / y; }
};
template<class T>
struct y_modulus :public y_binary_function<T, T, T> {
	T operator()(const T& x, const T& y) const { return x % y; }
};
//证同元素（identity element）：数值A若与该元素做op运算，会得到A自己
//加法运算的证同元素为0；乘法的为1
template<class T>
inline T y_identity_element(y_plus<T>) { return T(0); }
template<class T>
inline T y_identity_element(y_multiplies<T>) { return T(1); }
/**
 * 关系运算类
 * 等于和不等于、大于和小于、大于等于和小于等于
 */
template<class T>
struct y_equal_to :public y_binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x == y; }
};
template<class T>
struct y_not_equal_to :public y_binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x != y; }
};
template<class T>
struct y_greater :public y_binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x > y; }
};
template<class T>
struct y_greater_equal :public y_binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x >= y; }
};
template<class T>
struct y_less :public y_binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x < y; }
};
template<class T>
struct y_less_equal :public y_binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x <= y; }
};
/**
 * 逻辑运算类
 * 逻辑与、或、非
 */
template<class T>
struct y_logical_and :public y_binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x && y; }
};
template<class T>
struct y_logical_or :public y_binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x || y; }
};
template<class T>
struct y_logical_not :public y_unary_function<T, bool> {
	bool operator()(const T& x, const T& y) const { return !x; }
};
/**
 * stl容器常用
 * identity
 * select
 * project
 */
template<class T>
struct y_identity :public y_unary_function<T, T> {
	//set中键值和实值相等，所以采用此函数
	const T& operator() (const T& x) const { return x; }
};
template<class Pair>
struct y_select1st :public y_unary_function<Pair, typename Pair::first_type> {
	//map中以pair的第一元素为键值，所以采用此函数
	const typename Pair::first_type& operator() (const Pair& x) const { return x.first; }
	//传回第二元素同理 return x.second;
};
template<class Arg1, class Arg2>
struct y_project1st :public y_binary_function<Arg1, Arg2, Arg1> {
	//忽略第二参数，传回第一参数，还没见过使用
	Arg1 operator() (const Arg1& x, const Arg2&) const { return x; }
};
template<class Arg1, class Arg2>
struct y_project2nd :public y_binary_function<Arg1, Arg2, Arg2> {
	//忽略第一参数，传回第二参数，还没见过使用
	Arg2 operator() (const Arg1&, const Arg2& y) const { return y; }
};


int main()
{
	std::cout << "Hello World!\n";
}

