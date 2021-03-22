#pragma once
/**
 * SGI 第一级配置器
 * 
 * 使用malloc、realloc以及free，增加一些oom处理
 */

#if 0
#	include <new>
#	define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#	include <iostream>
#	define __THROW_BAD_ALLOC std::cerr<<"out of memory"<<std::endl; exit(1)
#endif

namespace yshen {
	template <int inst>
	class __malloc_alloc_template {
	private:
		//函数指针，所代表函数用来处理内存不足情况
		static void* oom_malloc(size_t);
		static void* oom_realloc(void*, size_t);
		static void (*__malloc_alloc_oom_handler)();

	public:
		//空间分配释放，直接malloc和free以及realloc
		static void* allocate(size_t n) {
			void result = malloc(n);
			if (0 == result) result = oom_malloc(n); //分配失败则调用oom处理函数
			return result;
		}
		static void deallocate(void* p, size_t/* n */) {
			free(p);
		}
		static void* reallocate(void* p, size_t/* old_sz */, size_t new_sz) {
			void* result = realloc(p, new_sz);
			if (0 == result) result = oom_realloc(p, new_sz);
			return result;
		}
		//仿c++的set_new_handler()，指定一个函数指针
		static void (*set_molloc_handler(void(*f)()))(){
			void (*old)() = __malloc_alloc_oom_handler;
			__malloc_alloc_oom_handler = f;
			return(old);
		}
	};

	//oom处理函数
	template<int inst>
	void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;
	template<int inst>
	void* __malloc_alloc_template<inst>::oom_malloc(size_t n) {
		void (*my_malloc_handler)();
		void* result;
		//不断尝试释放、分配
		for (;;)
		{
			my_malloc_handler = __malloc_alloc_oom_handler;
			//若未设定“内存不足处理例程”，抛出异常，结束应用
			if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
			(*my_malloc_handler)();//调用处理例程，企图释放内存
			result = malloc(n);//再次尝试分配
			if (result) return (result);
		}
	}
	template<int inst>
	void* __malloc_alloc_template<inst>::oom_realloc(void* p, size_t n) {
		void (*my_malloc_handler)();
		void* result;
		for (;;)
		{
			//类似malloc
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
			(*my_malloc_handler)();
			result = realloc(p, n);
			if (result) return(result);
		}
	}
	//直接将参数inst指定为0
	typedef __malloc_alloc_template<0> malloc_alloc;

}//end of namespace yshen