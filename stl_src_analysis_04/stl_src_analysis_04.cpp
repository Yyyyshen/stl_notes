// stl_src_analysis_04.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

/**
 * 序列式容器
 * array(c++内建)
 * vector
 *  heap(算法形式呈现)
 *   priority-queue
 * list
 * slist
 * deque
 *  stack (基于deque
 *  queue  属于adapter)
 *
 * 序列式容器元素都ordered，但未必sorted
 */

using namespace std;

/**
 * vector
 * 与array相似，差别在于空间可变性
 * 重新配置空间的操作为：申请新空间、数据移动、释放旧空间
 * 这个过程性能成本较高，所以不可能是每次新增元素时扩容
 * 需要一个空间配置策略
 * 参考SGI STL版本源码进行了解
 */
#include "y_vector.h"
#include <vector>
void test_vector() {
	vector<int> iv(2, 9);
	cout << "size:" << iv.size() << endl;//2
	cout << "capacity:" << iv.capacity() << endl;//2
	iv.push_back(1); //size3，cap4
	iv.clear();
	//size0,cap4
}
/**
 * list
 * 相比vector是连续线性空间，list会复杂一些
 * 每次插入和删除一个元素，就配置或释放一个元素空间，没有空间浪费，并且时间效率都是O(1)
 */
#include "y_list.h"
#include<list>
void test_list() {

}
/**
 * deque
 * 双向开口连续线性空间
 * 可在头尾两端做插入删除（vector技术上也实现了头部操作，但效率很差）
 * 与vector另一个差异在于，没有容量概念，因为是动态地以分段连续空间组合而成，随时可以增加一段新空间并链接
 * 可随机访问但迭代器并不是普通指针，复杂度高一些；所以，如果要排序deque可以先复制到vector进行排序再复制回来
 */
#include "y_deque.h"
#include <deque>
void test_deque() {
	//确实复杂很多，见y_deque.h中实现
}
/**
 * stack与queue
 * 基于deque的数据结构，不属于容器，是适配器
 */
#include "y_stack_and_queue.h"
void test_start_and_queue() {

}
/**
 * heap
 * 不属于容器，是prioity queue的助手
 * prioity queue允许使用者以任意次序将任何元素推入容器，取出时一定时从优先权最高（数值最高）的元素开始取
 * binary max heap有这样的特性，可以作为priority queue底层机制
 *
 * 分析：
 * 如果用list作为底层机制，元素插入操作可以保持O(1)，但要找到最大值，却要遍历扫描，或者先排序一次，但反过来插入操作又变得复杂，所以不太合适
 * 如果用二叉搜索树/红黑树作为底层机制，插入操作和最大值操作都时O(logN)，但有些小题大做，有些复杂
 * 基于完全二叉树的binary heap可以比较适中的完成这个需求
 * 由complete binary tree特性，可以利用array存储所有节点，将array#0元素保留，当树中某个节点位于arr[i]，其左节点必位于arr[2i]，右节点位于arr[2i+1]
 * 这种用arr简单表示出tree的方式为隐式表述法
 * 再考虑到，heap需要动态改变大小，所以用vector代替array
 * 最后，vec+heap算法来实现
 */
#include "y_heap.h"
#include <algorithm>
void test_heap() {
	int ia[9] = { 0,1,2,3,4,8,9,3,5 };
	vector<int> ivec(ia, ia + 9);//若直接以原生arr作为容器，可以使用make、sort、pop操作，但不能push，因为无法动态改变大小
	make_heap(ivec.begin(), ivec.end());
	for (int i = 0; i < ivec.size(); ++i)
		cout << ivec[i] << " ";
	cout << endl;//将vec转为了堆形式
	ivec.push_back(7);
	push_heap(ivec.begin(), ivec.end());
	for (int i = 0; i < ivec.size(); ++i)
		cout << ivec[i] << " ";
	cout << endl;//向后添加了一个元素，并调用调整堆算法
	pop_heap(ivec.begin(), ivec.end());
	cout << ivec.back() << endl;//此时只是将顶部元素放到结尾，需要手动去掉
	ivec.pop_back();
	for (int i = 0; i < ivec.size(); ++i)
		cout << ivec[i] << " ";
	cout << endl;
	sort_heap(ivec.begin(), ivec.end());
	for (int i = 0; i < ivec.size(); ++i)
		cout << ivec[i] << " ";
	cout << endl;//得到一个排序好的递增序列
}
/**
 * priority_queue
 * 在上一节heap中提过，缺省条件下利用一个max-heap实现，而map-heap是基于容器vector表现的完全二叉树
 * 满足权值高的先出（也就是使用pop_heap）
 */
#include <queue>
void test_priority_queue() {
	//见y_heap.h，写在一起了
	int ia[9] = { 0,1,2,3,4,8,9,3,5 };
	priority_queue<int> ipq(ia, ia + 9);
	while (!ipq.empty())
	{
		cout << ipq.top() << " ";
		ipq.pop();
	}
	cout << endl;
}
/**
 * slist
 * 单链表，并不在标准规格内，但可以作为技巧学习的例子
 * 与list的主要差别在于，前者为单向迭代器，后者list为双向迭代器
 */
#include "y_slist.h"
void test_slist() {
	
}


int main()
{
	test_heap();
}
