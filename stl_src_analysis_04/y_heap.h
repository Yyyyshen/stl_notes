#pragma once

/**
 * heap
 * 分为max-heap和min-heap
 * 每个节点都大于等于（或小于等于）左右子节点，也就是说根节点为最大值（或最小值）
 * 根节点的位置对应于array（或vector）的起头处
 */
void max_heap() {} //以下函数操作均以max-heap为例实现
/**
 * push_heap算法
 * 当一个新元素加入时，为了满足完全二叉树条件，需要先放在最下层作为叶节点
 * 对于vector就是放在end()处，继续判断新元素该在的位置（将该位置看为一个空洞，调整其索引进行比较，最后把结果填入坑中）
 * 进行上溯，将新节点于当前所在位置的父节点比较，如果比父节点大（对于max-heap），则互换位置，往复操作直到不需要互换或所处位置已经是根节点
 */
template<class RandomAccessIterator, class Distance, class T>
void __y_push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value) {
	Distance parent = (holeIndex - 1) / 2;//定位父节点位置
	while (holeIndex > topIndex /*还不是根节点*/ && *(first + parent) < value /*父节点小于新加入的值*/)
	{
		//将父节点值填入当前洞号
		*(first + holeIndex) = *(first + parent);
		//调整洞号
		holeIndex = parent;
		//计算新洞号的父节点，继续循环
		parent = (holeIndex - 1) / 2;
	}
	//调整到满足max_heap特性，给洞号元素赋值
	*(first + holeIndex) = value;
}
template<class RandomAccessIterator, class Distance, class T>
inline void __y_push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*) {
	__y_push_heap(first, Distance(last - first - 1), Distance(0), T(*(last - 1)));
}
template<class RandomAccessIterator>
inline void y_push_heap(RandomAccessIterator first, RandomAccessIterator last) {
	//此函数被调用时，两个迭代器参数应该为容器的头尾，并且新元素应该已经push_back，不满足的话执行结果不一定准确
	__y_push_heap_aux(first, last, distance_type(first), value_type(first));
}
/**
 * pop_heap
 * 最大值在根节点，pop操作应该是取走根节点
 * 而vector中是使用pop_back()性能高些，所以实际操作应该是把根节点移至vector最后，作为最下层的叶节点拿掉（或者看作根节点与最后一个叶子节点交换值，pop后调整树）
 * 并且，为了满足max-heap特性，需要执行一个下溯操作
 * 将根节点（最大值取走后，相当于一个空洞，空洞填入最后一个叶节点值）与子节点比较，交换较大的子节点，直到空洞下放到合适位置或叶节点为止
 */
template<class RandomAccessIterator, class Distance, class T>
void __y_adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value) {
	Distance topIndex = holeIndex;
	Distance secondChild = 2 * holeIndex + 2;//洞节点的右子节点
	while (secondChild < len) {
		//比较左右子节点值，然后用secondChild代表较大子节点
		if (*(first + secondChild) < *(first + (secondChild - 1)))
			secondChild--;
		//下溯，令较大子值为洞值，再下调洞号至较大子节点处
		*(first + holeIndex) = *(first + secondChild);
		holeIndex = secondChild;
		secondChild = 2 * (secondChild + 1);
	}
	if (secondChild == len)
	{
		//没有右子节点，则令左子值为洞值，同样操作
		*(first + holeIndex) = *(first + secondChild - 1);
		holeIndex = secondChild - 1;
	}
	//将调整值填入当前洞号，复用了push操作
	__y_push_heap(first, holeIndex, topIndex, value);//其实直接用*(first+holeIndex) = value;应该也可以，因为当前操作后已经满足了和heap特性
}
template<class RandomAccessIterator, class T, class Distance>
inline void __y_pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*) {
	*result = *first;//设定尾值为首值
	__y_adjust_heap(first, Distance(0), Distance(last - first), value);//下溯调整heap，value记录原叶子节点值，后续将填入洞号所在位置
}
template<class RandomAccessIterator, class T>
inline void __y_pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*) {
	__y_pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first));
}
template<class RandomAccessIterator>
inline void y_pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
	__y_pop_heap_aux(first, last, value_type(first));
}
/**
 * sort_heap
 * 基于pop_heap每次可获得heap中最大值，所以，持续pop操作（每次将范围缩小一位，因为pop操作首先会把最大值放到尾端）
 * 最终会得到一个递增序列
 */
template<class RandomAccessIterator>
void y_sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
	while (last - first > 1)
		y_pop_heap(first, last--);//每执行一次，操作范围缩小一格
}
/**
 * make_heap
 * 此算法用来将现有数据转化为heap
 */
template<class RandomAccessIterator, class T, class Distance>
inline void __y_make_heap(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*) {
	if (last - first < 2) return;//长度小于2，则没有必要进行排序
	Distance len = last - first;
	//确定第一个需要重排的子树头部，以parent标示（意义上应该是holeIndex）
	Distance parent = (len - 2) / 2;
	while (true)
	{
		__y_adjust_heap(first, parent, len, T(*(first + parent)));
		if (parent == 0) return;//走完根节点则结束
		parent--;
	}
}
template<class RandomAccessIterator>
inline void y_make_heap(RandomAccessIterator first, RandomAccessIterator last) {
	__y_make_heap(first, last, value_type(first), distance_type(first));
}