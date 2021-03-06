// stl_src_analysis_05.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

/**
 * 关联容器
 * set
 *  multiset
 * map
 *  multimap
 * 底层机制以RB-tree完成，也是独立容器，但不对外开放
 * 另外，还有哈希表以及衍生容器
 * hash_table
 *  hash_set
 *  hash_map
 *  hash_multiset
 *  hash_multimap
 */
using namespace std;
/**
 * 关联容器每个元素都有一个键值key和实值value
 * 当元素插入时，内部结构根据键大小以某种规则将元素放于合适位置
 * 没有头尾，所以也没有push_back,push_front,pop_back,pop_front,begin,end这样的操作
 * 一般来说，内部结构为一个平衡二叉树，有多种类型：AVL_tree，RB-tree，AA-tree
 *
 * 所以，先了解树（在算法练习项目中也有了一定的基础）
 * BST——二叉搜索树，任何节点键值一定大于左子树每个节点键值，并小于右子树每个节点键值，可以很快找到最大最小值（二分思想）
 * 增加时，从根节点开始，判断插入值与每个节点大小，根据规则插入正确位置
 * 删除时，如果节点只有一个子节点，直接将子节点连接至父节点；如果有两个，以右子树中最小值取代之（因为右子树一定比当前值大，所以用其中最小值来作为新的父节点）
 * BBST——平衡二叉搜索树
 * AVL——由于插入值顺序不同或值不平均，树很容易不平衡，AVL努力保证树的平衡，要求任何节点左右子树高度相差最多为1，若不满足则会旋转树来调整
 * RBT——红黑树，是一种平衡二叉搜索树，并满足规则：
 * (1)每个节点不是黑色就是红色
 * (2)根节点为黑色
 * (3)节点为红其子节点必须为黑
 * (4)任一节点至NULL(尾)的任何路径所含黑节点数相同（每个叶子节点都视为黑色的空节点）
 * 新增节点时：
 * 根据规则4，新节点为红（设置黑色会影响很多路径计算，所以设置为红色对于树平衡影响最小）
 * 根据规则3，父子节点不可能同时为红，则新增节点的父节点应为黑；
 * 若不满足上述条件，则需要调整颜色、旋转树形（采取什么方式依情况而定）
 *
 * 调整红黑树的一种方法：首先，为了避免父子节点同时为红色，可以从上至下沿着到新增节点的路径寻找
 * 当发现某节点两子节点都为红，则将该节点改为红，其两子节点改为黑，从上至下的修正颜色
 * 修正后若插入点的父节点为红，则旋转并修改颜色，使插入点的父节点为黑色
 * 这样调整过后，插入节点的操作就会变得简单，或是可以直接插入不影响树规则、或是插入后只需要进行一次单旋
 */
#include "y_rb_tree.h"
void test_rb_tree() {

}
/**
 * set
 * 所有元素根据键值自动排序，不同于map拥有实值和键值，set元素的键值就是实值，反之亦然，并且，不允许两个元素有相同键值
 * 由于这个特性，不能通过迭代器修改set元素值，因为元素值就是键值，关系到set排序规则，迭代器选用了底层红黑树中的const_iterator，杜绝写入
 * 开放的很多操作接口在红黑树中都提供了，基本就是一层封装
 */
#include "y_set.h"
void test_set() {

}
/**
 * map
 */
#include "y_map.h"
void test_map() {
	//除了元素使用pair，以及可以改变实值，与set相差不大
}
/**
 * multiset
 * 特性和用法与set完全相同，唯一差别是允许键值重复
 */
#include "y_multiset.h"
void test_multiset() {

}
/**
 * multimap
 * 同理multiset
 */
#include "y_multimap.h"
void test_multimap() {

}

/**
 * hashtable
 * 利用一个映射函数hash func，将元素映射为一个索引，以类似字典的形式来存储数据
 * 其中，这个散列函数不可能完全将不同元素映射到不同位置，还是可能出现相同索引的
 * 这就是所谓的碰撞，解决办法有线性探测、二次探测、开链等（算法课中了解过）
 */
#include "y_hashtable.h"
void test_hashtable() {

}
/**
 * hash_set
 * 除了底层使用hashtable作为容器之外，使用与set相同
 * 需注意的是由于红黑树可以自动排序，hash_set没有此功能
 */
#include "y_hashset.h"
void test_hashset() {

}
/**
 * hash_map
 * 同理与hash_set，换了底层容器
 */
#include "y_hashmap.h"
void test_hashmap() {

}
/**
 * hash_multiset和hash_multimap
 * 除了允许键值相同之外，与hashset和hashmap相同
 */
#include "y_hashmultiset_and_map.h"
void test_hashmultiset_and_map() {
	//后面随便看一眼就好，都是重复的东西
}

int main()
{
	std::cout << "Hello World!\n";
}
