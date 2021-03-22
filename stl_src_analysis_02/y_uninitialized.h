#pragma once
/**
 * 内存基本处理工具
 */
#if 0

namespace yshen {

	//copy相关函数
	template<class InputIterator, class ForwardIterator>
	ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type) {
		return copy(first, last, result);//交给STL算法
	}
	template<class InputIterator, class ForwardIterator>
	ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type) {
		ForwardIterator cur = result;
		for (; first != last; ++first, ++cur)
			construct(&*cur, *first);
		return cur;
	}
	template<class InputIterator, class ForwardIterator, class T>
	ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*) {
		typedef typename __type_traits<T>::is_POD_type is_POD;
		return __uninitialized_copy_aux(first, last, result, is_POD());
	}
	//该函数使我们能将内存的配置与对象的构造行为分离
	//如果作为输出的[result,result+(last-first)]范围内每个迭代器都指向未初始化区域，则会调用copy constructor
	//给输入范围[first,last)范围内每个对象产生一个复制品，放进输出范围
	//也就是，对于输入范围内每个迭代器i，该函数调用construct(&*(result+(i-first)),*i)
	//容器的全区间构造函数通常以两个步骤完成：配置内存块，足以包含范围内所有元素；使用uninitialized_copy，在该内存块构造元素
	template<class InputIterator, class ForwardIterator>
	ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
		return __uninitialized_copy(first, last, result, value_type(result));
	}
	//对于char*和wchar_t*可以采用最有效率的memmove执行复制行为，所以SGI为这两个类型设计了特化版本
	inline char* uninitialized_copy(const char* first, const char* last, char* result) {
		memmove(result, first, last - first);
		return result + (last - first);
	}


	//fill相关函数
	template<class ForwardIterator, class Size, class T>
	inline void __uninitialized_fill_aux(ForwardIterator first, Size n, const T& x, __true_type) {
		file(first, n, x);
	}
	template<class ForwardIterator, class Size, class T>
	inline void __uninitialized_fill_aux(ForwardIterator first, Size n, const T& x, __false_type) {
		//非POD类型
		ForwardIterator cur = first;
		for (; n > 0; --n, ++cur)
			construct(&*cur, x);
	}
	template<class ForwardIterator, class Size, class T, class T1>
	inline void __uninitialized_fill(ForwardIterator first, Size n, const T& x, T1*) {
		typedef typename __type_traits<T1>::is_POD_type is_POD;
		__uninitialized_fill_aux(first, n, x, is_POD());
	}
	template<class ForwardIterator, class T>
	void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x) {
		__uninitialized_fill(first, last, x, value_type(first));
	}


	//fill_n相关函数
	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __true_type) {
		return file_n(first, n, x);
	}
	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type) {
		//非POD类型
		ForwardIterator cur = first;
		for (; n > 0; --n, ++cur)
			construct(&*cur, x);
		return cur;
	}
	template<class ForwardIterator, class Size, class T, class T1>
	inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*) {
		//先验证迭代器的value_type，POD指的是Plain Old Data，也就是标量型别或传统的C struct型别
		typedef typename __type_traits<T1>::is_POD_type is_POD;
		return __uninitialized_fill_n_aux(first, n, x, is_POD());
	}
	//为指定范围[first,first+n)内所有元素设定相同的初值
	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
		return __uninitialized_fill_n(first, n, x, value_type(first));
	}

}//end of namespace yshen

#endif

