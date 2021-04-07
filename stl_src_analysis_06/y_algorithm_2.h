#pragma once

/**
 * 其他算法补充2
 */

template<class ForwardIterator, class T>
inline ForwardIterator y_lower_bound(ForwardIterator first, ForwardIterator last, const T& value) {
	//二分查找的一种，在已排序区间内寻找元素，找到则返回指向第一个元素的迭代器，没找到则返回指向第一个不小于value的元素
	return __y_lower_bound(first, last, value, distance_type(first), iterator_category(first));//根据迭代器做分发处理
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
		advance(middle, half);//令middle指向中间位置
		if (*middle < value)
		{
			first = middle;//first指向middle下一个位置
			++first;
			len = len - half - 1;//调整len继续查找
		}
		else
			len = half;
	}
	return first;
}

template<class ForwardIterator, class T>
inline ForwardIterator y_upper_bound(ForwardIterator first, ForwardIterator last, const T& value) {
	//在已排序区间内寻找元素，与lower_bound不同的是，如果寻找的值存在，则返回指向value下一个位置的迭代器，而并非指向value本身
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
		advance(middle, half);//令middle指向中间位置
		if (value < *middle)
			len = half;
		else
		{
			first = middle;//first指向middle下一个位置
			++first;
			len = len - half - 1;//调整len继续查找
		}
	}
	return first;
}

template<class ForwardIterator, class T>
bool y_binary_search(ForwardIterator first, ForwardIterator last, const T& value) {
	//二分查找的一种，与lower_bound/upper_bound不同的是，只返回是否找到，不提供额外信息
	ForwardIterator i = y_lower_bound(first, last, value);
	return i != last && !(value < *i);//如果是自定义判定规则，后半部分判断替换为!comp(value,*i)
}

template<class BidirectionalIterator>
bool y_next_permutation(BidirectionalIterator first, BidirectionalIterator last) {
	//取得区间内元素的下一个排列组合；例如，abc的六种排列组合为：abc,acb,bac,bca,cab,cba
	if (first == last) return false;//空区间
	BidirectionalIterator i = first;
	++i;
	if (i == last) return false;//只有一个元素
	i = last;
	--i;//指向最后一个元素
	for (;;) {
		BidirectionalIterator ii = i;
		--i;//用ii和i表示一组相邻元素
		if (*i < *ii) {
			BidirectionalIterator j = last;
			while (!(*i < *--j));//从后往前找，直到遇到比*i大的元素
			iter_swap(i, j);//交换i,j
			reverse(ii, last);//将ii之后的元素全部逆向重排
			return true;
		}
		if (i == first) {
			reverse(first, last);//进行到最前面，全部逆向重排
			return false;
		}
	}
	//prev_permutation为取得区间内元素的前一个排列组合
}

template<class RandomAccessIterator>
inline void y_random_shuffle(RandomAccessIterator first, RandomAccessIterator last) {
	//将区间内元素次序随机重排
	__y_random_shuffle(first, last, distance_type(first));
}
template<class RandomAccessIterator, class Distance>
void __y_random_shuffle(RandomAccessIterator first, RandomAccessIterator last, Distance*) {
	if (first == last) return;
	for (RandomAccessIterator i = first + 1; i != last; ++i)
		iter_swap(i, first + Distance(rand() % ((i - first) + 1)));//随机数方式可自定义
}

template<class RandomAccessIterator>
void y_partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last) {
	//接受一个middle迭代器，是序列中middle-first个最小元素以递增顺序排序，置于first,middle区间，其余last-middle个元素置于middle,last区间，不保证顺序
	__y_partial_sort(first, middle, last, vallue_type(first));
}
template<class RandomAccessIterator, class T>
void __y_partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, T*) {
	make_heap(first, middle);//先把first,middle之间元素按heap规则排序
	for (RandomAccessIterator i = middle; i < last; ++i)//i<last这种操作只适用于随机访问型迭代器
		if (*i < *first)//将后面元素与first比较，将小的挪到前半部分，first元素移到后半部分
			__pop_heap(first, middle, i, T(*i), distance_type(first));
	sort_heap(first, middle);
}

void y_sort() {
	//排序，将区间内元素从小到大重排，适用于随机访问型迭代器容器；关系型容器底层用红黑树所以不需要用这个算法
	//STL中sort算法，数据量大时采用快速排序，分段递归
	//分段后数据量小时，改用Insertion sort；如果递归层次过深，改用Heap sort
}
//先看Insertion sort
template<class RandomAccessIterator>
void __y_insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
	//双层循环形式排序，复杂度N^2，数据量小时比递归操作带来的负荷小
	if (first == last) return;
	for (RandomAccessIterator i = first + 1; i != last; ++i)//外循环遍历整个序列，决定出一个子区间
		__y_linear_insert(first, i, value_type(first));//内循环遍历子区间，将子区间内每个逆转对反转
}
template<class RandomAccessIterator, class T>
void __y_linear_inser(RandomAccessIterator first, RandomAccessIterator last, T*) {
	T value = *last;//记录尾元素
	if (value < *first) {
		//尾元素比头元素还小，直接整个区间右移，并将尾元素放到头部（这样判断是由于每次操作时，头元素一定是最小元素）
		copy_backward(first, last, last + 1);
		*first = value;
	}
	else//尾不小于头
		__y_unguarded_linear_insert(last, value);
}
template<class RandomAccessIterator, class T>
void __y_unguarded_linear_insert(RandomAccessIterator last, T value) {
	RandomAccessIterator next = last;
	--next;
	//遍历内循环，直到不存在逆转对
	while (value < *next) {
		*last = *next;
		last = next;
		--next;
	}
	*last = value;
}
//数据量大时，上面排序的复杂度就很高了，可以使用快速排序，平局复杂度NlogN，最坏时N^2
//主要思想为二分法，分治排序，所以先提供一个三点中值决定函数
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
//分割函数，返回分割后右段第一个位置
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
//控制分割恶化，找出2^k<=n的最大值k；例如元素有40个，则最多分割5*2=10层
template<class Size>
inline Size __y_lg(Size n) {
	Size k;
	for (k = 0; n > 1; n >>= 1)++k;
	return k;
}
//阈值
const int __y_stl_threshold = 16;
//introsort
template<class RandomAccessIterator, class T, class Size>
void __y_introsort_loop(RandomAccessIterator first, RandomAccessIterator last, T*, Size depth_limit) {
	while (last - first > __y_stl_threshold) {
		if (depth_limit == 0) {
			partial_sort(first, last, last);//分割太多，改用heapsort
			return;
		}
		--depth_limit;
		RandomAccessIterator cut = __y_unguarded_partition(first, last, T(__y_median(*first, *(first + (last - first) / 2), *(last - 1))));
		//右半段递归
		__y_introsort_loop(cut, last, value_type(first), depth_limit);
		last = cut;
		//调整last位置后继续循环，对左半段递归sort
	}
}
//introsort结束，区间内有多个子序列，每个子序列都有一定程度的排序，再进入final
template<class RandomAccessIterator>
void __y_final_insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
	if (last - first > __y_stl_threshold) {
		__y_insertion_sort(first, first + __y_stl_threshold);//大于阈值，insertion sort处理前面16个，
		__y_unguarded_insertion_sort(first + __y_stl_threshold, last);//剩余子序列继续处理
	}
	else
		__y_insertion_sort(first, last);//小于阈值，调用insertion sort处理一次
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