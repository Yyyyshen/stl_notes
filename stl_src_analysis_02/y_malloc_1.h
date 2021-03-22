#pragma once
/**
 * SGI ��һ��������
 * 
 * ʹ��malloc��realloc�Լ�free������һЩoom����
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
		//����ָ�룬�����������������ڴ治�����
		static void* oom_malloc(size_t);
		static void* oom_realloc(void*, size_t);
		static void (*__malloc_alloc_oom_handler)();

	public:
		//�ռ�����ͷţ�ֱ��malloc��free�Լ�realloc
		static void* allocate(size_t n) {
			void result = malloc(n);
			if (0 == result) result = oom_malloc(n); //����ʧ�������oom������
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
		//��c++��set_new_handler()��ָ��һ������ָ��
		static void (*set_molloc_handler(void(*f)()))(){
			void (*old)() = __malloc_alloc_oom_handler;
			__malloc_alloc_oom_handler = f;
			return(old);
		}
	};

	//oom������
	template<int inst>
	void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;
	template<int inst>
	void* __malloc_alloc_template<inst>::oom_malloc(size_t n) {
		void (*my_malloc_handler)();
		void* result;
		//���ϳ����ͷš�����
		for (;;)
		{
			my_malloc_handler = __malloc_alloc_oom_handler;
			//��δ�趨���ڴ治�㴦�����̡����׳��쳣������Ӧ��
			if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
			(*my_malloc_handler)();//���ô������̣���ͼ�ͷ��ڴ�
			result = malloc(n);//�ٴγ��Է���
			if (result) return (result);
		}
	}
	template<int inst>
	void* __malloc_alloc_template<inst>::oom_realloc(void* p, size_t n) {
		void (*my_malloc_handler)();
		void* result;
		for (;;)
		{
			//����malloc
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
			(*my_malloc_handler)();
			result = realloc(p, n);
			if (result) return(result);
		}
	}
	//ֱ�ӽ�����instָ��Ϊ0
	typedef __malloc_alloc_template<0> malloc_alloc;

}//end of namespace yshen