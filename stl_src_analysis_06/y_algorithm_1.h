#pragma once

/**
 * 其他算法补充1
 */

template<class ForwardIterator>
ForwardIterator y_adjacent_find(ForwardIterator first, ForwardIterator last) {
	//查找相邻重复元素
	if (first == last) return last;
	ForwardIterator next = first;
	while (++next != last) {
		if (*first == *next) return first;//或替换为自定义的函数对象
		first = next;
	}
	return last;
}

template<class InputIterator, class T>
typename iterator_traits<InputIterator>::difference_type
y_count(InputIterator first, InputIterator last, const T& value) {
	//找出符合条件元素个数
	typename iterator_traits<InputIterator>::difference_type n = 0;
	for (; first != last; ++first)
		if (*first == value) //count_if版本可指定具体判定条件
			++n;
	return n;
}

template<class InputIterator, class T>
InputIterator y_find(InputIterator first, InputIterator last, const T& value) {
	//寻找第一个符合条件的元素位置，find_if可指定判断条件
	while (first != last && *first != value) ++first;
	return first;
}

template<class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1
y_find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2) {
	//在序列1中，查找序列2的最后一次出现点，没找到则返回last1
	typedef typename iterator_traits<ForwardIterator1>::iterator_category category1;
	typedef typename iterator_traits<ForwardIterator2>::iterator_category category2;
	//根据不同的型别，调用不同下层函数
	return __y_find_end(first1, last1, first2, last2, category1(), category2());
}
//单项查找型
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
//双向迭代器型，可逆向查找
template<class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator1
__y_find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1, BidirectionalIterator2 first2, BidirectionalIterator2 last2
	, bidirectional_iterator_tag, bidirectional_iterator_tag) {
	//由于查找是最后出现的位置，所以反向查第一个出现的位置就会快一些
	typedef reverse_iterator<BidirectionalIterator1> reviter1;
	typedef reverse_iterator<BidirectionalIterator2> reviter2;
	reviter1 rlast1(first1);
	reviter2 rlast2(first2);
	reviter1 rresult = search(reviter1(last1), rlast1, reviter2(last2), rlast2);
	if (rresult == rlast1)
		return last1;
	else {
		BidirectionalIterator1 result = rresult.base();//转回非逆向迭代器
		advance(result, -distance(first2, last2));//调整回到子序列的起头处
		return result;
	}
}

template<class InputIterator, class ForwardIterator>
InputIterator y_find_first_of(InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2) {
	//以区间2某些元素为查找目标，寻找在区间1内第一次出现的位置；例如在synesthesia寻找第一个元音字母，则可以指定第二序列为aeiou，最终返回e所在位置
	for (; first1 != last1; ++first1)
		for (ForwardIterator iter = first2; iter != last2; ++iter)
			if (*first1 == *iter)
				return first1;
	return last1;
}

template<class InputIterator, class Function>
Function y_for_each(InputIterator first, InputIterator last, Function f) {
	//遍历迭代器，将函数f作用于区间内所有元素
	for (; first != last; ++first)
		f(*first);
	return f;
}

template<class ForwardIterator, class Generator>
void y_generate(ForwardIterator first, ForwardIterator last, Generator gen) {
	//将仿函数运算结果填入区间，generate_n为指定从first开始到第n个元素做此操作
	for (; first != last; ++first)
		*first = gen();
}

template<class InputIterator1, class InputIterator2>
bool y_includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
	//判断序列2是否涵盖于序列1，其中两序列必须都是有序集合
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
	//找出序列中最大元素，返回迭代器
	if (first == last) return first;
	ForwardIterator result = first;
	while (++first != last)
		if (*result < *first) result = first;//min_element同理
	return result;
}

template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator y_merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	//两个经过排序的集合，合并置于另一段空间，所得结果也是sorted序列
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
	//两个序列同步前进，把较小的按顺序放到新容器，最后把剩余元素拷贝到目的段
	return copy(first2, last2, copy(first1, last1, result));
}

template<class BidirectionalIterator, class Predicate>
BidirectionalIterator y_partition(BidirectionalIterator first, BidirectionalIterator last, Predicate pred) {
	//将区间内元素重新排列，被运算pred判定为true的元素会被放在前段，不保证元素的原始相对位置
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
	//移除，但不删除；将没有移除的元素按顺序从first赋值，返回值为重新整理后最后元素的下一个位置
	//例如：{0,1,0,2,0,3,0,4}执行remove()移除0值元素，执行结果为{1,2,3,4,0,3,0,4}，返回值为指向第五个位置的迭代器
	//所以，此方法经常于erase搭配使用，erase每次删除元素都会重新整理容器，效率会比较低，而remove整理之后，直接删掉返回位置之后的数据即可，效率高一些

	first = find(first, last, value);//找出第一个相等元素；如果是remove_if，这里替换为find_if，可指定判定条件
	ForwardIterator next = first;
	//利用remove_copy()允许新旧容器重叠性质进行移除操作
	return first == last ? first : y_remove_copy(++next, last, first, value);//如果是remove_if，这里替换为remove_copy_if，可指定判定条件
}

template<class InputIterator, class OutputIterator, class T>
OutputIterator y_remove_coopy(InputIterator first, InputIterator last, OutputIterator result, const T& value) {
	//移除元素但不删除，结果复制到一个以result标示起始位置的容器身上，新容器可以和原容器重叠
	for (; first != last; ++first)
		if (*first != value) { //如果是remove_copy_if，这里替换为!pred(*first)
			*result = *first;
			++result;
		}
	return result;
}

template<class ForwardIterator, class T>
void y_replace(ForwardIterator first, ForwardIterator last, const T& old_value, const T& new_value) {
	//替换区间内值
	for (; first != last; ++first)
		if (*first == old_value) *first = new_value;
	//replace_if的话这里判断标准替换为自定义操作伪函数
}

template<class InputIterator, class OutputIterator, class T>
OutputIterator y_replace_copy(InputIterator first, InputIterator last, OutputIterator result, const T& old_value, const T& new_value) {
	//替换区间内值，但结果复制到result所指位置
	for (; first != last; ++first, ++result)
		*result = *first == old_value ? new_value : *first;//replace_copy_if同理
	return result;
}

template<class BidirectionalIterator>
inline void y_reverse(BidirectionalIterator first, BidirectionalIterator last) {
	//反转容器中元素，针对不同的迭代器类型，做了一个分发
	__y_reverse(first, last, iterator_catefory(first));
}
//双向迭代器
template<class BidirectionalIterator>
void __y_reverse(BidirectionalIterator first, BidirectionalIterator last, bidirectional_iterator_tag) {
	while (true)
		if (first == last || first == --last)
			return;
		else
			iter_swap(first++, last);
}
//随机访问迭代器
template<class RandomAccessIterator>
void __y_reverse(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
	while (first < last) iter_swap(first++, --last);//头尾互换，跟上面其实差不多，但只有随机访问型迭代器才能做first<last这样的判断
}
//反转序列但结果复制到指定容器中，双向迭代器，直接从尾部往前走一趟即可
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
	//旋转将first,middle和middle,last元素互换，功能类似swap_ranges但两个区间长度可以不同
	if (first == middle || middle == last) return;
	__y_rotate(first, middle, last, distance_type(first), iterator_category(first));//同样为了不同迭代器类型的效率做了分发
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
	Distance n = __y_gcd(last - first, middle - first);//此迭代器相减操作只适用于随机访问型迭代器，取全长和前端长度的最大公因子
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
//rotate_copy将反转结果复制到新位置，不需要原容器调整内容，就简单的用copy操作即可
template<class ForwardIterator, class OutputIterator>
OutputIterator y_rotate_copy(ForwardIterator first, ForwardIterator middle, ForwardIterator last, OutputIterator result) {
	return copy(first, middle, copy(middle, last, result));
}

void y_search() {
	//在序列1中查找序列2第一次出现的位置
}

template<class InputIterator, class OutputIterator, class UnaryOperation>
OutputIterator y_transform(InputIterator first, InputIterator last, OutputIterator result, UnaryOperation op) {
	//将序列中元素及逆行仿函数操作，并将结果放入指定容器位置
	for (; first != last; ++first, ++result)
		*result = op(*first);
	return result;
}

void y_unique() {
	//移除重复元素，先用adjacent_find找出相邻重复元素起点
}