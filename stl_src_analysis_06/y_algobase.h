#pragma once

/**
 * �����㷨
 */

template<class T>
struct display {
	void operator()(const T& x) const { cout << x << ' '; }//�º����������
};

//equal���Ƚ�������Ԫ��
template<class InputIterator1, class InputIterator2>
inline bool y_equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
	for (; first1 != last1; ++first1, ++first2) //�������1��Χ���� ��������
		if (*first1 != *first2)
			return false;
	return true;
}
//fill����дԪ��
template<class ForwardIterator, class T>
void y_fill(ForwardIterator first, ForwardIterator last, const T& value) {
	for (; first != last; ++first)
		*first = value;
}
template<class OutputIterator, class Size, class T>
OutputIterator y_fill_n(OutputIterator first, Size n, const T& value) {
	for (; n > 0; --n, ++first)//���n�����˵�������Χ���������⣬����ʹ��һ��inserter()�������в�����Ǹ�д���ܵĵ�����
		*first = value;
	return first;
}
//iter_swap��������ָԪ��
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
//lexicographical_compare���Ƚ�Ԫ��
template <class InputIterator1, class InputIterator2, class Compare>
bool y_lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp) {
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (comp(*first1, *first2))//Ĭ���� < 
			return true;
		if (comp(*first2, *first1))
			return false;
	}
	return first1 == last1 && first2 != last2;//���е���������һ���е����β���ڶ����л���ʣ�࣬��ô��һ���и�С
}
//�ػ��汾ʹ��ԭ��ָ�룬�ȽϺ���ʹ��c��׼����memcmpЧ��Ҳ����
inline bool y_lexicographical_compare(const unsigned char* first1, const unsigned char* last1, const unsigned char* first2, const unsigned char* last2) {
	const size_t len1 = last1 - first1;
	const size_t len2 = last2 - first2;
	const int result = memcmp(first1, first2, min(len1, len2));
	return result != 0 ? result < 0 : len1 < len2;
}
//max��minͬ��
template<class T/* ,class Compare */>
inline const T& y_max(const T& a, const T& b/* ,Compare comp */) {
	return a < b ? b : a;
	//return comp(b, a) ? b : a;//����comp������С�Ƚϱ�׼
}
//mismatch��ƽ�бȽ��������У�֮����һ����ƥ��㣬����һ�Ե��������ֱ�ָ���������в�ƥ���
template<class InputIterator1, class InputIterator2>
pair<InputIterator1, InputIterator2> y_mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
	while (first1 != last1 && *first1 == *first2) {
		//while(first1!=last1&&binary_pred(*first1,*first2)){
		++first1;
		++first2;
	}
	return pair<InputIterator1, InputIterator2>(first1, first2);
}
//swap������ֵ
template<class T>
inline void y_swap(T& a, T& b) {
	T temp = a;
	a = b;
	b = temp;
}

/**
 * copy
 * ��������
 * ʹ��C��׼�����ܽ�Լ�ܶ�ʱ�䣬����STL��ʹ���˴��������ء��ͱ����ԡ�ƫ�ػ��ȼ�ǿЧ��
 * 
 * ��copy_backward Ϊ���п�����ԭ����ͬ������ͬ��
 */
template<class InputIterator, class OutputIterator> struct __y_copy_dispatch;
//��ȫ�����汾
template<class InputIterator, class OutputIterator>
inline OutputIterator y_copy(InputIterator first, InputIterator last, OutputIterator result) {
	return __y_copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
}
//����汾���أ��ַ��������ַ�ͬ��
inline char* y_copy(const char* first, const char* last, char* result) {
	memmove(result, first, last - first);
	return result + (last - first);
}
//��ȫ�����ķַ�����
template<class InputIterator, class OutputIterator>
struct __y_copy_dispatch {
	OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
		return __y_copy(first, last, result, iterator_category(first));
	}
};
//�����ж�ָ����ָ�����Ƿ�߱�һЩ����
struct __y_true_type {};
struct __y_false_type {};
template<class T> struct __y_type_traits {};
//ƫ�ػ�ָ��汾��const T* ͬ��
template<class T>
struct __y_copy_dispatch<T*, T*> {
	T* operator()(T* first, T* last, T* result) {
		typedef typename __y_type_traits<T>::has_trivial_assignment_operator t;
		return __y_copy_t(first, last, result, t());
	}
};
//���ݲ�ͬ�����������ò�ͬ�汾__copy
template<class InputIterator, class OutputIterator>
inline OutputIterator __y_copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag) {
	for (; first != last; ++result, ++first)
		*result = *first;//�Ե�������ͬ������ѭ���Ƿ�������ٶ������
	return result;
}
//��������͵������汾
template<class RandomAccessIterator, class OutputIterator>
inline OutputIterator __y_copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag) {
	//�ٷ�װ���Ա㸴��
	return __y_copy_d(first, last, result, distance_type(first));
}
template<class RandomAccessIterator, class OutputIterator, class Distance>
inline OutputIterator __y_copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*) {
	//����õ�n������ѭ���������ٶȱ��жϵ������Ƿ���ͬҪ��һЩ
	for (Distance n = last - first; n > 0; --n, ++result, ++first)
		*result = *first;
	return result;
}
//����ƫ�ػ�ԭ��ָ��汾������ֱ����C��׼������������ָ����ָ����߱�trivial assignment operator
template<class T>
inline T* __y_copy_t(const T* first, const T* last, T* result, __y_true_type) {
	memmove(result, first, sizeof(T) * (last - first));
	return result + (last - first);
}
//��ָԪ�ؾ߱�non-trivial assignment operator����ԭ��ָ����һ����������͵����������Ը���__copy_d
template<class T>
inline T* __y_copy_t(const T* first, const T* last, T* result, __y_false_type) {
	return __y_copy_d(first, last, result, (ptrdiff_t*)0);
}