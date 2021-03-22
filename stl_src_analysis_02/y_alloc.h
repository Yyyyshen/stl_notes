#pragma once

/**
 * ���ݱ�׼�ӿڣ����һ�����ܼ򵥵�allocator
 */
#include <new>// for set_new_handler
#include <cstddef>// for ptrdiff_t,size_t
#include <cstdlib>// for exit()
#include <climits>// for UINT_MAX
#include <iostream>

namespace yshen
{
	//����������������

	//����ռ�
	template <class T>
	inline T* _allocate(ptrdiff_t /*��ͬϵͳ��ָ�볤�Ȳ�ͬ*/ size, T*) {
		//set_new_handler(0)��Ҫ��Ϊ��ж��Ŀǰ���ڴ�����쳣������
		//����һ��һ�������ڴ�ʧ�ܵĻ���C++�ͻ�ǿ�����׳�std:bad_alloc�쳣���������ܵ�����ĳ���쳣������ȥ����
		//�ο��� http://www.cplusplus.com/reference/new/set_new_handler/
		std::set_new_handler(0);
		T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
		/*
		C++�е�operator new��new operator
		operator new
		��1�� ֻ������Ҫ��Ŀռ䣬��������ض���Ĺ��캯�������޷�������Ҫ�����Ŀռ�ʱ����
				->�����new_handler�������new_handler������
				->���ûҪ���׳��쳣����nothrow����������ִ��bad_alloc�쳣������
				->����0
		��2�� ���Ա�����
		��3�� ����ʱ���������ͱ�������Ϊvoid*
		��4�� ����ʱ����һ���������ͱ���Ϊ���Ҫ�����ռ�Ĵ�С���ֽڣ�������Ϊsize_t
		��5�� ����ʱ�����Դ���������
		new operator
		��1�� ����operator new�����㹻�Ŀռ䣬��������ض���Ĺ��캯��
		��2�� �����Ա�����
		��Ӧ�أ�operator delete��delete operator�����Ƶ����ԡ�
		*/
		if (tmp == 0)
		{
			//�൱������һ�������set_new_handler���쳣����
			std::cerr << "out of memory" << std::endl;
			exit(1);
		}
		return tmp;
	}
	//�ͷſռ�
	template <class T>
	inline void _deallocate(T* buffer) {
		::operator delete(buffer);
	}
	//��������
	template <class T1, class T2>
	inline void _construct(T1* p, const T2& value) {
		//�ο���https://blog.csdn.net/u014209688/article/details/90047713
		new(p) T1(value);
		/*
		�������Ԥ������ڴ��ϴ���������ȱʡ��new���������в�ͨ��
		����ʹ��һ����������placement new�������㹹��һ���¶���Ԥ������ڴ���
		��������������ָ��p��ָ����ڴ�ռ䴴��һ��T1���͵Ķ���
		������������Ǵ�T2���͵Ķ���ת�������ģ�������T1�Ĺ��캯����T1::T1(T2 value)��
		������realloc�����������пռ�Ļ��������µ�������Ŀռ�
		������������еĿռ䵱��һ����������ʹ�ã������˷���ռ����ķѵ�ʱ��
		ֱ����new�����������ڴ�Ļ����ڶ��в����㹻���ʣ��ռ��ٶ��ǱȽ�����
		�������Լ����ڴ滺������������ʵ���Լ����ڴ������Ե�ʱ��placement new�������
		��STL�й㷺ʹ����placement new�������������ڴ�
		��Ҫע��ĵ㣺
		ʹ��ͷ�ļ�#include <new >
		������placement new����һ�������е�Ԫ��
		Ҫ����һ����placement new����Ķ�����Ҫ�ֶ����������������������ڡ�placement delete�����ֲ�������
		*/
	}
	//���ٶ���
	template <class T>
	inline void _destroy(T* ptr) {
		ptr->~T();//�ֶ�������������
	}

	//�ռ�������
	//������
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

		//�ذ�allocator
		template <class U>
		struct rebind {
			typedef allocator<U> other;
		};

		//��װ����
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
		//��ַ����С
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