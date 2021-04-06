#pragma once

/**
 * 数值算法
 */

template<class InputIterator, class T>
T y_accumulate(InputIterator first, InputIterator last, T init) {
	for (; first != last; ++first)
		init = init + *first;//普通版本，每个元素累加
	return init;
}
//带操作函数版本
template<class InputIterator, class T, class BinaryOperation>
T y_accumulate(InputIterator first, InputIterator last, T init, BinaryOperation binary_op) {
	for (; first != last; ++first)
		init = binary_op(init, *first);//二元操作不必满足交换律和结合律
	return init;
}
//adjacent_difference
template<class InputIterator, class OutputIterator, class T>
OutputIterator __y_adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, T*) {
	T value = *first;
	while (++first != last) {
		T temp = *first;
		*++result = temp - value;//使用二元操作函数对象的话，替换这里即可，不赘述
		value = temp;
	}
	return ++result;
}
template<class InputIterator, class OutputIterator>
OutputIterator y_adjacent_difference(InputIterator first, InputIterator last, OutputIterator result) {
	if (first == last) return result;
	*result = *first;
	return __y_adjacent_difference(first, last, result, value_type(first));
}
//inner_product，求内积，提供外界仿函数取代原操作版本
template<class InputIterator1, class InputIterator2, class T, class BinaryOperation1, class BinaryOperation2>
T y_inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init, BinaryOperation1 binary_op1, BinaryOperation2 binary_op2) {
	for (; first1 != last1; ++first1, ++first2)
		init = binary_op1(init, binary_op2(*first1, *first2));
	return init;
}
//partial_sum
template<class InputIterator, class OutputIterator, class T>
OutputIterator __y_partial_sum(InputIterator first, InputIterator last, OutputIterator result, T*) {
	T value = *first;
	while (++first != last)
	{
		value = value + *first;
		*++result = value;
	}
	return ++result;
}
template<class InputIterator, class OutputIterator>
OutputIterator y_partial_sum(InputIterator first, InputIterator last, OutputIterator result) {
	if (first == last) return result;
	*result = *first;
	return __y_partial_sum(first, last, result, value_type(first));
}
//求幂，不在stl标准内
template<class T, class Integer, class MonoidOperation>
T y_power(T x, Integer n, MonoidOperation op) {//op操作必须满足结合律，不需要满足交换律
	if (n == 0)
		//return identity_element(op);//取出“证同元素”
		return 0;
	else {
		while ((n & 1) == 0) {
			n >>= 1;
			x = op(x, x);
		}
		T result = x;
		n >>= 1;
		while (n != 0) {
			x = op(x, x);
			if ((n & 1) != 0)
				result = op(result, x);
			n >>= 1;
		}
		return result;
	}
}
template<class T, class Integer>
inline T y_power(T x, Integer n) {
	return y_power(x, n, multiplies<T>());//默认指定运算形式为乘法
}
//itoa，改变某区间内容，是一种质变算法mutating algorithm
template<class ForwardIterator, class T>
void y_iota(ForwardIterator first, ForwardIterator last, T value) {
	while (first != last) *first++ = value++;
}