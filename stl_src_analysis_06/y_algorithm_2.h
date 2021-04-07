#pragma once

/**
 * �����㷨����2
 */

template<class ForwardIterator, class T>
inline ForwardIterator y_lower_bound(ForwardIterator first, ForwardIterator last, const T& value) {
	//���ֲ��ҵ�һ�֣���������������Ѱ��Ԫ�أ��ҵ��򷵻�ָ���һ��Ԫ�صĵ�������û�ҵ��򷵻�ָ���һ����С��value��Ԫ��
	return __y_lower_bound(first, last, value, distance_type(first), iterator_category(first));//���ݵ��������ַ�����
}
template<class ForwardIterator, class T, class Distance>
ForwardIterator __y_lower_bound(ForwardIterator first, ForwardIterator last, const T& value, Distance*, forward_iterator_tag) {
	Distance len = 0;
	distance(first, last, len);
	Distance half;
	ForwardIterator middle;
	while (len > 0)
	{
		half = len >> 1;// len / 2
		middle = first;
		advance(middle, half);//��middleָ���м�λ��
		if (*middle < value)
		{
			first = middle;//firstָ��middle��һ��λ��
			++first;
			len = len - half - 1;//����len��������
		}
		else
			len = half;
	}
	return first;
}

template<class ForwardIterator, class T>
inline ForwardIterator y_upper_bound(ForwardIterator first, ForwardIterator last, const T& value) {
	//��������������Ѱ��Ԫ�أ���lower_bound��ͬ���ǣ����Ѱ�ҵ�ֵ���ڣ��򷵻�ָ��value��һ��λ�õĵ�������������ָ��value����
	return __y_upper_bound(first, last, value, distance_type(first), iterator_category(first));
}
template<class ForwardIterator, class T, class Distance>
ForwardIterator __y_upper_bound(ForwardIterator first, ForwardIterator last, const T& value, Distance*, forward_iterator_tag) {
	Distance len = 0;
	distance(first, last, len);
	Distance half;
	ForwardIterator middle;
	while (len > 0)
	{
		half = len >> 1;// len / 2
		middle = first;
		advance(middle, half);//��middleָ���м�λ��
		if (value < *middle)
			len = half;
		else
		{
			first = middle;//firstָ��middle��һ��λ��
			++first;
			len = len - half - 1;//����len��������
		}
	}
	return first;
}

template<class ForwardIterator, class T>
bool y_binary_search(ForwardIterator first, ForwardIterator last, const T& value) {
	//���ֲ��ҵ�һ�֣���lower_bound/upper_bound��ͬ���ǣ�ֻ�����Ƿ��ҵ������ṩ������Ϣ
	ForwardIterator i = y_lower_bound(first, last, value);
	return i != last && !(value < *i);//������Զ����ж����򣬺�벿���ж��滻Ϊ!comp(value,*i)
}

template<class BidirectionalIterator>
bool y_next_permutation(BidirectionalIterator first, BidirectionalIterator last) {
	//ȡ��������Ԫ�ص���һ��������ϣ����磬abc�������������Ϊ��abc,acb,bac,bca,cab,cba
	if (first == last) return false;//������
	BidirectionalIterator i = first;
	++i;
	if (i == last) return false;//ֻ��һ��Ԫ��
	i = last;
	--i;//ָ�����һ��Ԫ��
	for (;;) {
		BidirectionalIterator ii = i;
		--i;//��ii��i��ʾһ������Ԫ��
		if (*i < *ii) {
			BidirectionalIterator j = last;
			while (!(*i < *--j));//�Ӻ���ǰ�ң�ֱ��������*i���Ԫ��
			iter_swap(i, j);//����i,j
			reverse(ii, last);//��ii֮���Ԫ��ȫ����������
			return true;
		}
		if (i == first) {
			reverse(first, last);//���е���ǰ�棬ȫ����������
			return false;
		}
	}
	//prev_permutationΪȡ��������Ԫ�ص�ǰһ���������
}

template<class RandomAccessIterator>
inline void y_random_shuffle(RandomAccessIterator first, RandomAccessIterator last) {
	//��������Ԫ�ش����������
	__y_random_shuffle(first, last, distance_type(first));
}
template<class RandomAccessIterator, class Distance>
void __y_random_shuffle(RandomAccessIterator first, RandomAccessIterator last, Distance*) {
	if (first == last) return;
	for (RandomAccessIterator i = first + 1; i != last; ++i)
		iter_swap(i, first + Distance(rand() % ((i - first) + 1)));//�������ʽ���Զ���
}

template<class RandomAccessIterator>
void y_partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last) {
	//����һ��middle����������������middle-first����СԪ���Ե���˳����������first,middle���䣬����last-middle��Ԫ������middle,last���䣬����֤˳��
	__y_partial_sort(first, middle, last, vallue_type(first));
}
template<class RandomAccessIterator, class T>
void __y_partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, T*) {
	make_heap(first, middle);//�Ȱ�first,middle֮��Ԫ�ذ�heap��������
	for (RandomAccessIterator i = middle; i < last; ++i)//i<last���ֲ���ֻ��������������͵�����
		if (*i < *first)//������Ԫ����first�Ƚϣ���С��Ų��ǰ�벿�֣�firstԪ���Ƶ���벿��
			__pop_heap(first, middle, i, T(*i), distance_type(first));
	sort_heap(first, middle);
}

void y_sort() {
	//���򣬽�������Ԫ�ش�С�������ţ���������������͵�������������ϵ�������ײ��ú�������Բ���Ҫ������㷨
	//STL��sort�㷨����������ʱ���ÿ������򣬷ֶεݹ�
	//�ֶκ�������Сʱ������Insertion sort������ݹ��ι������Heap sort
}
//�ȿ�Insertion sort
template<class RandomAccessIterator>
void __y_insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
	//˫��ѭ����ʽ���򣬸��Ӷ�N^2��������Сʱ�ȵݹ���������ĸ���С
	if (first == last) return;
	for (RandomAccessIterator i = first + 1; i != last; ++i)//��ѭ�������������У�������һ��������
		__y_linear_insert(first, i, value_type(first));//��ѭ�����������䣬����������ÿ����ת�Է�ת
}
template<class RandomAccessIterator, class T>
void __y_linear_inser(RandomAccessIterator first, RandomAccessIterator last, T*) {
	T value = *last;//��¼βԪ��
	if (value < *first) {
		//βԪ�ر�ͷԪ�ػ�С��ֱ�������������ƣ�����βԪ�طŵ�ͷ���������ж�������ÿ�β���ʱ��ͷԪ��һ������СԪ�أ�
		copy_backward(first, last, last + 1);
		*first = value;
	}
	else//β��С��ͷ
		__y_unguarded_linear_insert(last, value);
}
template<class RandomAccessIterator, class T>
void __y_unguarded_linear_insert(RandomAccessIterator last, T value) {
	RandomAccessIterator next = last;
	--next;
	//������ѭ����ֱ����������ת��
	while (value < *next) {
		*last = *next;
		last = next;
		--next;
	}
	*last = value;
}
//��������ʱ����������ĸ��ӶȾͺܸ��ˣ�����ʹ�ÿ�������ƽ�ָ��Ӷ�NlogN���ʱN^2
//��Ҫ˼��Ϊ���ַ������������������ṩһ��������ֵ��������
template<class T>
inline const T& __y_median(const T& a, const T& b, const T& c) {
	if (a < b)
		if (b < c)
			return b;
		else if (a < c)
			return c;
		else
			return a;
	else if (a < c)
		return a;
	else if (b < c)
		return c;
	else
		return b;
}
//�ָ�������طָ���Ҷε�һ��λ��
template<class RandomAccessIterator, class T>
RandomAccessIterator __y_unguarded_partition(RandomAccessIterator first, RandomAccessIterator last, T pivot) {
	while (true) {
		while (*first < pivot) ++first;
		--last;
		while (pivot < *last) --last;
		if (!(first < last)) return first;
		iter_swap(first, last);
		++first;
	}
}
//SGI STL sort
template<class RandomAccessIterator>
inline void y_sort(RandomAccessIterator first, RandomAccessIterator last) {
	if (first != last)
	{
		__y_introsort_loop(first, last, value_type(first), __y_lg(last - first) * 2);
		__y_final_insertion_sort(first, last);
	}
}
//���Ʒָ�񻯣��ҳ�2^k<=n�����ֵk������Ԫ����40���������ָ�5*2=10��
template<class Size>
inline Size __y_lg(Size n) {
	Size k;
	for (k = 0; n > 1; n >>= 1)++k;
	return k;
}
//��ֵ
const int __y_stl_threshold = 16;
//introsort
template<class RandomAccessIterator, class T, class Size>
void __y_introsort_loop(RandomAccessIterator first, RandomAccessIterator last, T*, Size depth_limit) {
	while (last - first > __y_stl_threshold) {
		if (depth_limit == 0) {
			partial_sort(first, last, last);//�ָ�̫�࣬����heapsort
			return;
		}
		--depth_limit;
		RandomAccessIterator cut = __y_unguarded_partition(first, last, T(__y_median(*first, *(first + (last - first) / 2), *(last - 1))));
		//�Ұ�εݹ�
		__y_introsort_loop(cut, last, value_type(first), depth_limit);
		last = cut;
		//����lastλ�ú����ѭ���������εݹ�sort
	}
}
//introsort�������������ж�������У�ÿ�������ж���һ���̶ȵ������ٽ���final
template<class RandomAccessIterator>
void __y_final_insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
	if (last - first > __y_stl_threshold) {
		__y_insertion_sort(first, first + __y_stl_threshold);//������ֵ��insertion sort����ǰ��16����
		__y_unguarded_insertion_sort(first + __y_stl_threshold, last);//ʣ�������м�������
	}
	else
		__y_insertion_sort(first, last);//С����ֵ������insertion sort����һ��
}
template<class RandomAccessIterator>
void __y_unguarded_insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
	__y_unguarded_insertion_sort_aux(first, last, value_type(first));
}
template<class RandomAccessIterator, class T>
void __y_unguarded_insertion_sort_aux(RandomAccessIterator first, RandomAccessIterator last, T*) {
	for (RandomAccessIterator i = first; i != last; ++i)
		__y_unguarded_linear_insert(i, T(*i));
}