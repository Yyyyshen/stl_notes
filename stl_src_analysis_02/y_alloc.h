#pragma once

/**
 * 根据标准接口，完成一个功能简单的allocator
 */
#include <new>// for set_new_handler
#include <cstddef>// for ptrdiff_t,size_t
#include <cstdlib>// for exit()
#include <climits>// for UINT_MAX
#include <iostream>

namespace yshen
{
	//几个基本操作函数

	//分配空间
	template <class T>
	inline T* _allocate(ptrdiff_t /*不同系统下指针长度不同*/ size, T*) {
		//set_new_handler(0)主要是为了卸载目前的内存分配异常处理函数
		//这样一来一旦分配内存失败的话，C++就会强制性抛出std:bad_alloc异常，而不是跑到处理某个异常处理函数去处理
		//参考： http://www.cplusplus.com/reference/new/set_new_handler/
		std::set_new_handler(0);
		T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
		/*
		C++中的operator new与new operator
		operator new
		（1） 只分配所要求的空间，不调用相关对象的构造函数。当无法满足所要求分配的空间时，则
				->如果有new_handler，则调用new_handler，否则
				->如果没要求不抛出异常（以nothrow参数表达），则执行bad_alloc异常，否则
				->返回0
		（2） 可以被重载
		（3） 重载时，返回类型必须声明为void*
		（4） 重载时，第一个参数类型必须为表达要求分配空间的大小（字节），类型为size_t
		（5） 重载时，可以带其它参数
		new operator
		（1） 调用operator new分配足够的空间，并调用相关对象的构造函数
		（2） 不可以被重载
		相应地，operator delete与delete operator有相似的特性。
		*/
		if (tmp == 0)
		{
			//相当于用这一块代替了set_new_handler的异常处理
			std::cerr << "out of memory" << std::endl;
			exit(1);
		}
		return tmp;
	}
	//释放空间
	template <class T>
	inline void _deallocate(T* buffer) {
		::operator delete(buffer);
	}
	//创建对象
	template <class T1, class T2>
	inline void _construct(T1* p, const T2& value) {
		//参考：https://blog.csdn.net/u014209688/article/details/90047713
		new(p) T1(value);
		/*
		如果想在预分配的内存上创建对象，用缺省的new操作符是行不通的
		可以使用一个操作叫做placement new，允许你构造一个新对象到预分配的内存上
		本例操作就是在指针p所指向的内存空间创建一个T1类型的对象
		而对象的内容是从T2类型的对象转换过来的（调用了T1的构造函数，T1::T1(T2 value)）
		类似于realloc函数，在已有空间的基础上重新调整分配的空间
		这个操作把已有的空间当成一个缓冲区来使用，减少了分配空间所耗费的时间
		直接用new操作符分配内存的话，在堆中查找足够大的剩余空间速度是比较慢的
		当你有自己的内存缓冲区或者在你实现自己的内存分配策略的时候，placement new会很有用
		在STL中广泛使用了placement new来给容器分配内存
		需要注意的点：
		使用头文件#include <new >
		可以用placement new构造一个数组中的元素
		要析构一个用placement new分配的对象，需要手动调用析构函数（并不存在“placement delete”这种操作）。
		*/
	}
	//销毁对象
	template <class T>
	inline void _destroy(T* ptr) {
		ptr->~T();//手动调用析构函数
	}

	//空间配置器
	//类主体
	template <class T>
	class allocator {
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		//重绑定allocator
		template <class U>
		struct rebind {
			typedef allocator<U> other;
		};

		//封装函数
		pointer allocate(size_type n, const void* hint = 0) {
			return _allocate((difference_type)n, (pointer)0);
		}
		void deallocate(pointer p, size_type n) {
			_deallocate(p);
		}
		void construct(pointer p, const T& value) {
			_construct(p, value);
		}
		void destory(pointer p) {
			_destroy(p);
		}
		//地址及大小
		pointer address(reference x) {
			return (pointer)&x;
		}
		const_pointer const_address(const_reference x) {
			return (const_pointer)&x;
		}
		size_type max_size() const {
			return size_type(UINT_MAX / sizeof(T));
		}
	};

}//end of namespace yshen