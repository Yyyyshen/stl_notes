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

int main()
{
	
}
