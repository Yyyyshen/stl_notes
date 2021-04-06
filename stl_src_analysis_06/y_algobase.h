#pragma once

/**
 * 基本算法
 */

template<class T>
struct display {
	void operator()(const T& x) const { cout << x << ' '; }//仿函数用于输出
};

//equal，比较区间内元素
template<class InputIterator1, class InputIterator2>
inline bool y_equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
	for (; first1 != last1; ++first1, ++first2) //如果序列1范围更大， 会有问题
		if (*first1 != *first2)
			return false;
	return true;
}
//fill，填写元素
template<class ForwardIterator, class T>
void y_fill(ForwardIterator first, ForwardIterator last, const T& value) {
	for (; first != last; ++first)
		*first = value;
}
template<class OutputIterator, class Size, class T>
OutputIterator y_fill_n(OutputIterator first, Size n, const T& value) {
	for (; n > 0; --n, ++first)//如果n超过了迭代器范围，会有问题，可以使用一个inserter()产生具有插入而非覆写功能的迭代器
		*first = value;
	return first;
}
//iter_swap，交换所指元素
template<class ForwardIterator1, class ForwardIterator2, class T>
inline void __y_iter_swwap(ForwardIterator1 a, ForwardIterator2 b, T*) {
	T temp = *a;
	*a = *b;
	*b = temp;
}
template<class ForwardIterator1, class ForwardIterator2>
inline void y_iter_swwap(ForwardIterator1 a, ForwardIterator2 b) {
	__y_iter_swap(a, b, value_type(a));
}
//lexicographical_compare，比较元素
template <class InputIterator1, class InputIterator2, class Compare>
bool y_lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp) {
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (comp(*first1, *first2))//默认是 < 
			return true;
		if (comp(*first2, *first1))
			return false;
	}
	return first1 == last1 && first2 != last2;//进行到这里，如果第一序列到达结尾而第二序列还有剩余，那么第一序列更小
}
//特化版本使用原生指针，比较函数使用c标准函数memcmp效率也更高
inline bool y_lexicographical_compare(const unsigned char* first1, const unsigned char* last1, const unsigned char* first2, const unsigned char* last2) {
	const size_t len1 = last1 - first1;
	const size_t len2 = last2 - first2;
	const int result = memcmp(first1, first2, min(len1, len2));
	return result != 0 ? result < 0 : len1 < len2;
}
//max（min同理）
template<class T/* ,class Compare */>
inline const T& y_max(const T& a, const T& b/* ,Compare comp */) {
	return a < b ? b : a;
	//return comp(b, a) ? b : a;//或由comp决定大小比较标准
}
//mismatch，平行比较两个序列，之处第一个不匹配点，返回一对迭代器，分别指向两序列中不匹配点
template<class InputIterator1, class InputIterator2>
pair<InputIterator1, InputIterator2> y_mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
	while (first1 != last1 && *first1 == *first2) {
		//while(first1!=last1&&binary_pred(*first1,*first2)){
		++first1;
		++first2;
	}
	return pair<InputIterator1, InputIterator2>(first1, first2);
}
//swap，交换值
template<class T>
inline void y_swap(T& a, T& b) {
	T temp = a;
	a = b;
	b = temp;
}

/**
 * copy
 * 拷贝操作
 * 使用C标准函数能节约很多时间，所以STL中使用了大量的重载、型别特性、偏特化等加强效率
 * 
 * （copy_backward 为逆行拷贝，原理相同，方向不同）
 */
template<class InputIterator, class OutputIterator> struct __y_copy_dispatch;
//完全泛化版本
template<class InputIterator, class OutputIterator>
inline OutputIterator y_copy(InputIterator first, InputIterator last, OutputIterator result) {
	return __y_copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
}
//特殊版本重载，字符串（宽字符同理）
inline char* y_copy(const char* first, const char* last, char* result) {
	memmove(result, first, last - first);
	return result + (last - first);
}
//完全泛化的分发函数
template<class InputIterator, class OutputIterator>
struct __y_copy_dispatch {
	OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
		return __y_copy(first, last, result, iterator_category(first));
	}
};
//用于判断指针所指对象是否具备一些条件
struct __y_true_type {};
struct __y_false_type {};
template<class T> struct __y_type_traits {};
//偏特化指针版本（const T* 同理）
template<class T>
struct __y_copy_dispatch<T*, T*> {
	T* operator()(T* first, T* last, T* result) {
		typedef typename __y_type_traits<T>::has_trivial_assignment_operator t;
		return __y_copy_t(first, last, result, t());
	}
};
//根据不同迭代器，调用不同版本__copy
template<class InputIterator, class OutputIterator>
inline OutputIterator __y_copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag) {
	for (; first != last; ++result, ++first)
		*result = *first;//以迭代器等同与否决定循环是否继续，速度相对慢
	return result;
}
//随机访问型迭代器版本
template<class RandomAccessIterator, class OutputIterator>
inline OutputIterator __y_copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag) {
	//再封装，以便复用
	return __y_copy_d(first, last, result, distance_type(first));
}
template<class RandomAccessIterator, class OutputIterator, class Distance>
inline OutputIterator __y_copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*) {
	//用算好的n来决定循环次数，速度比判断迭代器是否相同要快一些
	for (Distance n = last - first; n > 0; --n, ++result, ++first)
		*result = *first;
	return result;
}
//对于偏特化原生指针版本，可以直接用C标准函数，适用于指针所指对象具备trivial assignment operator
template<class T>
inline T* __y_copy_t(const T* first, const T* last, T* result, __y_true_type) {
	memmove(result, first, sizeof(T) * (last - first));
	return result + (last - first);
}
//所指元素具备non-trivial assignment operator，而原生指针是一种随机访问型迭代器，可以复用__copy_d
template<class T>
inline T* __y_copy_t(const T* first, const T* last, T* result, __y_false_type) {
	return __y_copy_d(first, last, result, (ptrdiff_t*)0);
}