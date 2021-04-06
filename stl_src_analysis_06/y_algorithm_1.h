#pragma once

/**
 * �����㷨����1
 */

template<class ForwardIterator>
ForwardIterator y_adjacent_find(ForwardIterator first, ForwardIterator last) {
	//���������ظ�Ԫ��
	if (first == last) return last;
	ForwardIterator next = first;
	while (++next != last) {
		if (*first == *next) return first;//���滻Ϊ�Զ���ĺ�������
		first = next;
	}
	return last;
}

template<class InputIterator, class T>
typename iterator_traits<InputIterator>::difference_type
y_count(InputIterator first, InputIterator last, const T& value) {
	//�ҳ���������Ԫ�ظ���
	typename iterator_traits<InputIterator>::difference_type n = 0;
	for (; first != last; ++first)
		if (*first == value) //count_if�汾��ָ�������ж�����
			++n;
	return n;
}

template<class InputIterator, class T>
InputIterator y_find(InputIterator first, InputIterator last, const T& value) {
	//Ѱ�ҵ�һ������������Ԫ��λ�ã�find_if��ָ���ж�����
	while (first != last && *first != value) ++first;
	return first;
}

template<class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1
y_find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2) {
	//������1�У���������2�����һ�γ��ֵ㣬û�ҵ��򷵻�last1
	typedef typename iterator_traits<ForwardIterator1>::iterator_category category1;
	typedef typename iterator_traits<ForwardIterator2>::iterator_category category2;
	//���ݲ�ͬ���ͱ𣬵��ò�ͬ�²㺯��
	return __y_find_end(first1, last1, first2, last2, category1(), category2());
}
//���������
template<class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1
__y_find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, forward_iterator_tag, forward_iterator_tag) {
	if (first2 == last2)
		return last1;
	else {
		ForwardIterator1 result = last1;
		while (1) {
			ForwardIterator1 new_result = search(first1, last1, first2, last2);
			if (new_result == last1)
				return result;
			else {
				result = new_result;
				first1 = new_result;
				++first1;
			}
		}
	}
}
//˫��������ͣ����������
template<class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator1
__y_find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1, BidirectionalIterator2 first2, BidirectionalIterator2 last2
	, bidirectional_iterator_tag, bidirectional_iterator_tag) {
	//���ڲ����������ֵ�λ�ã����Է�����һ�����ֵ�λ�þͻ��һЩ
	typedef reverse_iterator<BidirectionalIterator1> reviter1;
	typedef reverse_iterator<BidirectionalIterator2> reviter2;
	reviter1 rlast1(first1);
	reviter2 rlast2(first2);
	reviter1 rresult = search(reviter1(last1), rlast1, reviter2(last2), rlast2);
	if (rresult == rlast1)
		return last1;
	else {
		BidirectionalIterator1 result = rresult.base();//ת�ط����������
		advance(result, -distance(first2, last2));//�����ص������е���ͷ��
		return result;
	}
}

template<class InputIterator, class ForwardIterator>
InputIterator y_find_first_of(InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2) {
	//������2ĳЩԪ��Ϊ����Ŀ�꣬Ѱ��������1�ڵ�һ�γ��ֵ�λ�ã�������synesthesiaѰ�ҵ�һ��Ԫ����ĸ�������ָ���ڶ�����Ϊaeiou�����շ���e����λ��
	for (; first1 != last1; ++first1)
		for (ForwardIterator iter = first2; iter != last2; ++iter)
			if (*first1 == *iter)
				return first1;
	return last1;
}

template<class InputIterator, class Function>
Function y_for_each(InputIterator first, InputIterator last, Function f) {
	//������������������f����������������Ԫ��
	for (; first != last; ++first)
		f(*first);
	return f;
}

template<class ForwardIterator, class Generator>
void y_generate(ForwardIterator first, ForwardIterator last, Generator gen) {
	//���º����������������䣬generate_nΪָ����first��ʼ����n��Ԫ�����˲���
	for (; first != last; ++first)
		*first = gen();
}

template<class InputIterator1, class InputIterator2>
bool y_includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
	//�ж�����2�Ƿ񺭸�������1�����������б��붼�����򼯺�
	while (first1 != last1 && first2 != last2)
		if (*first2 < *first1)
			return false;
		else if (*first1 < *first2)
			++first1;
		else
			++first1, ++first2;
	return first2 == last2;
}

template<class ForwardIterator>
ForwardIterator y_max_element(ForwardIterator first, ForwardIterator last) {
	//�ҳ����������Ԫ�أ����ص�����
	if (first == last) return first;
	ForwardIterator result = first;
	while (++first != last)
		if (*result < *first) result = first;//min_elementͬ��
	return result;
}

template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator y_merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	//������������ļ��ϣ��ϲ�������һ�οռ䣬���ý��Ҳ��sorted����
	while (first1 != last1 && first2 != last2) {
		if (*first2 < *first1) {
			*result = *first2;
			++first2;
		}
		else {
			*result = *first1;
			++first1;
		}
		++result;
	}
	//��������ͬ��ǰ�����ѽ�С�İ�˳��ŵ�������������ʣ��Ԫ�ؿ�����Ŀ�Ķ�
	return copy(first2, last2, copy(first1, last1, result));
}

template<class BidirectionalIterator, class Predicate>
BidirectionalIterator y_partition(BidirectionalIterator first, BidirectionalIterator last, Predicate pred) {
	//��������Ԫ���������У�������pred�ж�Ϊtrue��Ԫ�ػᱻ����ǰ�Σ�����֤Ԫ�ص�ԭʼ���λ��
	while (true) {
		while (true)
			if (first == last)
				return first;
			else if (pred(*first))
				++first;
			else
				break;
		--last;
		while (true)
			if (first == last)
				return first;
			else if (!pred(*last))
				--last;
			else
				break;
		iter_swap(first, last);
		++first;
	}
}

template<class ForwardIterator, class T>
ForwardIterator y_remove(ForwardIterator first, ForwardIterator last, const T& value) {
	//�Ƴ�������ɾ������û���Ƴ���Ԫ�ذ�˳���first��ֵ������ֵΪ������������Ԫ�ص���һ��λ��
	//���磺{0,1,0,2,0,3,0,4}ִ��remove()�Ƴ�0ֵԪ�أ�ִ�н��Ϊ{1,2,3,4,0,3,0,4}������ֵΪָ������λ�õĵ�����
	//���ԣ��˷���������erase����ʹ�ã�eraseÿ��ɾ��Ԫ�ض�����������������Ч�ʻ�Ƚϵͣ���remove����֮��ֱ��ɾ������λ��֮������ݼ��ɣ�Ч�ʸ�һЩ

	first = find(first, last, value);//�ҳ���һ�����Ԫ�أ������remove_if�������滻Ϊfind_if����ָ���ж�����
	ForwardIterator next = first;
	//����remove_copy()�����¾������ص����ʽ����Ƴ�����
	return first == last ? first : y_remove_copy(++next, last, first, value);//�����remove_if�������滻Ϊremove_copy_if����ָ���ж�����
}

template<class InputIterator, class OutputIterator, class T>
OutputIterator y_remove_coopy(InputIterator first, InputIterator last, OutputIterator result, const T& value) {
	//�Ƴ�Ԫ�ص���ɾ����������Ƶ�һ����result��ʾ��ʼλ�õ��������ϣ����������Ժ�ԭ�����ص�
	for (; first != last; ++first)
		if (*first != value) { //�����remove_copy_if�������滻Ϊ!pred(*first)
			*result = *first;
			++result;
		}
	return result;
}

template<class ForwardIterator, class T>
void y_replace(ForwardIterator first, ForwardIterator last, const T& old_value, const T& new_value) {
	//�滻������ֵ
	for (; first != last; ++first)
		if (*first == old_value) *first = new_value;
	//replace_if�Ļ������жϱ�׼�滻Ϊ�Զ������α����
}

template<class InputIterator, class OutputIterator, class T>
OutputIterator y_replace_copy(InputIterator first, InputIterator last, OutputIterator result, const T& old_value, const T& new_value) {
	//�滻������ֵ����������Ƶ�result��ָλ��
	for (; first != last; ++first, ++result)
		*result = *first == old_value ? new_value : *first;//replace_copy_ifͬ��
	return result;
}

template<class BidirectionalIterator>
inline void y_reverse(BidirectionalIterator first, BidirectionalIterator last) {
	//��ת������Ԫ�أ���Բ�ͬ�ĵ��������ͣ�����һ���ַ�
	__y_reverse(first, last, iterator_catefory(first));
}
//˫�������
template<class BidirectionalIterator>
void __y_reverse(BidirectionalIterator first, BidirectionalIterator last, bidirectional_iterator_tag) {
	while (true)
		if (first == last || first == --last)
			return;
		else
			iter_swap(first++, last);
}
//������ʵ�����
template<class RandomAccessIterator>
void __y_reverse(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
	while (first < last) iter_swap(first++, --last);//ͷβ��������������ʵ��࣬��ֻ����������͵�����������first<last�������ж�
}
//��ת���е�������Ƶ�ָ�������У�˫���������ֱ�Ӵ�β����ǰ��һ�˼���
template<class BidirectionalIterator, class OutputIterator>
OutputIterator y_reverse_copy(BidirectionalIterator first, BidirectionalIterator last, OutputIterator result) {
	while (first != last) {
		--last;
		*result = *last;
		++result;
	}
	return result;
}

template<class ForwardIterator>
void y_rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last) {
	//��ת��first,middle��middle,lastԪ�ػ�������������swap_ranges���������䳤�ȿ��Բ�ͬ
	if (first == middle || middle == last) return;
	__y_rotate(first, middle, last, distance_type(first), iterator_category(first));//ͬ��Ϊ�˲�ͬ���������͵�Ч�����˷ַ�
}
template<class ForwardIterator, class Distance>
void __y_rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last, Distance*, forward_iterator_tag) {
	for (ForwardIterator i = middle;;) {
		iter_swap(first, i);
		++first;
		++i;
		if (first == middle) {
			if (i == last) return;
			middle = i;
		}
		else if (i == last)
			i = middle;
	}
}
template<class BidirectionalIterator, class Distance>
void __y_rotate(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, Distance*, bidirectional_iterator_tag) {
	reverse(first, middle);
	reverse(middle, last);
	reverse(first, last);
}
template<class RandomAccessIterator, class Distance>
void __y_rotate(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Distance*, random_access_iterator_tag) {
	Distance n = __y_gcd(last - first, middle - first);//�˵������������ֻ��������������͵�������ȡȫ����ǰ�˳��ȵ��������
	while (n--)
		__y_rotate_cycle(first, last, first + n, middle - first, value_type(first));
}
template<class EuclideanRingElement>
EuclideanRingElement __y_gcd(EuclideanRingElement m, EuclideanRingElement n) {
	while (n != 0) {
		EuclideanRingElement t = m % n;
		m = n;
		n = t;
	}
	return m;
}
template<class RandomAccessIterator, class Distance, class T>
void __y_rotate_cycle(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator initial, Distance shift, T*) {
	T value = *initial;
	RandomAccessIterator ptr1 = initial;
	RandomAccessIterator ptr2 = ptr1 + shift;
	while (ptr2 != initial) {
		*ptr1 = *ptr2;
		ptr1 = ptr2;
		if (last - ptr2 > shift)
			ptr2 += shift;
		else
			ptr2 = first + (shift - (last - ptr2));
	}
	*ptr1 = value;
}
//rotate_copy����ת������Ƶ���λ�ã�����Ҫԭ�����������ݣ��ͼ򵥵���copy��������
template<class ForwardIterator, class OutputIterator>
OutputIterator y_rotate_copy(ForwardIterator first, ForwardIterator middle, ForwardIterator last, OutputIterator result) {
	return copy(first, middle, copy(middle, last, result));
}

void y_search() {
	//������1�в�������2��һ�γ��ֵ�λ��
}

template<class InputIterator, class OutputIterator, class UnaryOperation>
OutputIterator y_transform(InputIterator first, InputIterator last, OutputIterator result, UnaryOperation op) {
	//��������Ԫ�ؼ����зº��������������������ָ������λ��
	for (; first != last; ++first, ++result)
		*result = op(*first);
	return result;
}

void y_unique() {
	//�Ƴ��ظ�Ԫ�أ�����adjacent_find�ҳ������ظ�Ԫ�����
}