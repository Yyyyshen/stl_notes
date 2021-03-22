// stl_src_analysis_02.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

/**
 * 空间配置器
 *
 * STL操作的数据都存放在容器内，就需要分配空间
 *
 * 简单自己实现一个
 */
#include "y_alloc.h"
#include <vector>
using namespace std;
void test_y_alloc() {
	//int ia[5] = { 0,1,2,3,4 };
	//unsigned int i;
	//vector<int, yshen::allocator<int>> iv(ia, ia + 5);
	//for (i = 0; i < iv.size(); i++)
	//{
	//	cout << iv[i] << " ";
	//}
	//cout << endl;

	//简单的实现并不能直接使用，上面代码块只简单示范用法
}
/**
 * SGI STL使用了专属的sub-allocation，有层次配置能力、效率优越
 * （其实也提供了标准配置器接口，做了一层隐藏）
 * 与标准规范不同，名称使用alloc而非allocator，并且不接受参数
 * 示例：vector<int,std::alloc> iv;
 * 未符合标准也并不影响什么，日常中很少会自己实现
 * SGI STL中vector声明：
 * template <class T, class Alloc = alloc>
 * class vector{...};
 *
 * SGI中标准空间配置器std::allocator，不建议使用，因为效率不佳，只对于内存分配和释放进行一次封装
 * 而一般的内存分配应该有两步，::operator new分配内存，再调用构造；删除同理，析构后释放内存
 * 所以将两个操作阶段区分开（就像上面自己实现的例子）
 *
 * SGI中内存分配释放，std::alloc
 * 设计上：
 * 向system heap申请空间
 * 考虑多线程
 * 考虑内存不足时的应变措施
 * 考虑过多小型区块造成内存碎片问题
 *
 * SIG设计了双层级配置器
 * 第一级使用了C的malloc()和free()，模拟set_new_handler处理内存不足情况
 * 第二级视情况采用不同策略，维护16个free lists，负责16种小型区块的次配置能力，使用了memory pool，内存不足时转用第一级配置器
 * 不管使用哪一级，都会再封装一个接口使配置器符合STL规格，如下
 */
template <class T, class Alloc>
class y_simple_alloc {
public:
	static T* allocate(size_t n) { return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T)); }
	static T* allocate(void) { return (T*)Alloc::allocate(sizeof(T)); }
	static void deallocate(T* p, size_t n) { if (0 != n)Alloc::deallocate(p, n * sizeof(T)); }
	static void deallocate(T* p) { Alloc::deallocate(p, sizeof(T)); }
};
/**
 * 详解两级配置器
 * 见下面头文件中实现
 */
#include "y_malloc_1.h" 
#include "y_malloc_2.h"

int main()
{
	//test_y_alloc();
}

