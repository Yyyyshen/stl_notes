#pragma once

/**
 * traits
 * 型别萃取
 */

template<class I>
struct y_iterator_traits {
	typedef typename I::iterator_category	iterator_category;//迭代器分类

	/*
	根据移动特性和操作，分为五类
	Input Iterator:所指对象只读
	Output Iterator:只写
	Forward Iterator:允许写入型算法（例如replace()）在此种迭代器区间进行读写
	Bidirectional Iterator:双向移动
	Random Access Iterator:随机访问，涵盖所有指针算数能力，包括p+n,p-n,p[n],p1-p2,p1<p2;（前三种支持++，第四种加上--）
	*/


	typedef typename I::value_type			value_type;//迭代器所指对象型别
	typedef typename I::difference_type		difference_type;//两迭代器之间的距离，也可表示容器最大容量
	typedef typename I::pointer				pointer;//对应于reference，代表所指对象地址
	typedef typename I::reference			reference;//对应于pointer，代表所指对象内容
};

//一些应用
//计算个数
template<class I, class T>
typename iterator_traits<I>::difference_type
y_count(I first, I last, const T& value) {
	typename iterator_traits<I>::difference_type n = 0;
	for (; first != last; ++first)
		if (*first == value)
			++n;
	return n;
}
//不同类型迭代器
template<class InputIterator, class Distance>
void y_advance_II(InputIterator& i, Distance n) { while (n--) ++i; }
template<class BidirectionalIterator, class Distance>
void y_advance_BI(BidirectionalIterator& i, Distance n) {
	if (n >= 0)
		while (n--) ++i;
	else
		while (n++) --i;
}
template<class RandomAccessIterator, class Distance>
void y_advance_RAI(RandomAccessIterator& i, Distance n) { i += n; }
//判断类型
template<class InputIterator, class Distance>
void y_advance(InputIterator& i, Distance n) {
	//执行期决定使用哪个版本
	if (is_random_access_iterator(i))
		y_advance_RAI(i, n);
	else if (is_bidirectional_iterator(i))
		y_advance_BI(i, n);
	else
		y_advance_II(i, n);
}
//上面方法效率不高，可以使用一些类作为标记
struct y_input_iterator_tag {};
struct y_output_iterator_tag {};
struct y_forward_iterator_tag : public y_input_iterator_tag {};
struct y_bidirectional_iterator_tag : public y_forward_iterator_tag {};
struct y_random_access_iterator_tag : public y_bidirectional_iterator_tag {};
//重载
template<class InputIterator, class Distance>
inline void __y_advance(InputIterator& i, Distance n, y_input_iterator_tag) { while (n--) ++i; }
template<class ForwardIterator, class Distance>
inline void __y_advance(ForwardIterator& i, Distance n, y_forward_iterator_tag) { __y_advance(i, n, y_input_iterator_tag()); /* 单纯转发 */ }
template<class BidirectionalIterator, class Distance>
void __y_advance(BidirectionalIterator& i, Distance n, y_bidirectional_iterator_tag) {
	if (n >= 0)
		while (n--) ++i;
	else
		while (n++) --i;
}
template<class RandomAccessIterator, class Distance>
void __y_advance(RandomAccessIterator& i, Distance n, y_random_access_iterator_tag) { i += n; }
//上层控制接口
template<class InputIterator/*STL规范，以最低阶类型命名*/, class Distance>
inline void advance(InputIterator& i, Distance n)
{
	__y_advance(i, n, y_iterator_traits<InputIterator>::iterator_category());//使用了临时对象
}

//针对原生指针和pointer-to-const偏特化
template<class T>
struct y_iterator_traits<T*> {
	typedef y_random_access_iterator_tag	iterator_category;//原生指针是一种随机访问迭代器
	typedef T								value_type;
	typedef ptrdiff_t						difference_type;
	typedef T* pointer;
	typedef T& reference;
};
template<class T>
struct y_iterator_traits<const T*> {
	typedef y_random_access_iterator_tag	iterator_category;
	typedef T								value_type;
	typedef ptrdiff_t						difference_type;
	typedef const T* pointer;
	typedef const T& reference;
};

/**
 * 迭代器
 */
template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Referance = T&>
struct y_iterator {
	typedef Category		iterator_category;
	typedef T				value_type;
	typedef Distance		difference_type;
	typedef Pointer			pointer;
	typedef Referance		reference;
};
//具体针对容器时的正式规格
template<class Item>
struct y_list_iter : public std::iterator<std::forward_iterator_tag, Item> {};

//获取各型别的函数封装
template<class Iterator>
inline typename y_iterator_traits<Iterator>::iterator_category
y_iterator_category(const Iterator&) {
	typedef typename y_iterator_traits<Iterator>::iterator_category category;
	return category();
}
template<class Iterator>
inline typename y_iterator_traits<Iterator>::difference_type*
y_distance_type(const Iterator&) {
	return static_cast<typename y_iterator_traits<Iterator>::difference_type*>(0);
}
template<class Iterator>
inline typename y_iterator_traits<Iterator>::value_type*
y_value_type(const Iterator&) {
	return static_cast<typename y_iterator_traits<Iterator>::value_type*>(0);
}



//一些关于左值右值的语法测试
void test_something() {
	//使用*解引用是，左值允许赋值操作，右值不允许
	int* pi = new int(5);
	const int* pci = new int(9);//pointer-to-const指向的值为const，ptr本身不是const；同写法int const * pci;
	int* const cpi = new int(6);//const ptr，const修饰右边的内容
	*pi = 7;
	//*pci = 1; //*pci提取一个右值，不允许赋值
	pci = pi;//pci指针本身可以赋值
	//cpi = pi; //const ptr
	cout << *pi << " " << *pci << " " << *cpi << endl;
}