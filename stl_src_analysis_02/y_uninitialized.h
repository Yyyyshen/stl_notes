#pragma once
/**
 * �ڴ����������
 */
#if 0

namespace yshen {

	//copy��غ���
	template<class InputIterator, class ForwardIterator>
	ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type) {
		return copy(first, last, result);//����STL�㷨
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
	//�ú���ʹ�����ܽ��ڴ�����������Ĺ�����Ϊ����
	//�����Ϊ�����[result,result+(last-first)]��Χ��ÿ����������ָ��δ��ʼ������������copy constructor
	//�����뷶Χ[first,last)��Χ��ÿ���������һ������Ʒ���Ž������Χ
	//Ҳ���ǣ��������뷶Χ��ÿ��������i���ú�������construct(&*(result+(i-first)),*i)
	//������ȫ���乹�캯��ͨ��������������ɣ������ڴ�飬���԰�����Χ������Ԫ�أ�ʹ��uninitialized_copy���ڸ��ڴ�鹹��Ԫ��
	template<class InputIterator, class ForwardIterator>
	ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
		return __uninitialized_copy(first, last, result, value_type(result));
	}
	//����char*��wchar_t*���Բ�������Ч�ʵ�memmoveִ�и�����Ϊ������SGIΪ����������������ػ��汾
	inline char* uninitialized_copy(const char* first, const char* last, char* result) {
		memmove(result, first, last - first);
		return result + (last - first);
	}


	//fill��غ���
	template<class ForwardIterator, class Size, class T>
	inline void __uninitialized_fill_aux(ForwardIterator first, Size n, const T& x, __true_type) {
		file(first, n, x);
	}
	template<class ForwardIterator, class Size, class T>
	inline void __uninitialized_fill_aux(ForwardIterator first, Size n, const T& x, __false_type) {
		//��POD����
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


	//fill_n��غ���
	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __true_type) {
		return file_n(first, n, x);
	}
	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type) {
		//��POD����
		ForwardIterator cur = first;
		for (; n > 0; --n, ++cur)
			construct(&*cur, x);
		return cur;
	}
	template<class ForwardIterator, class Size, class T, class T1>
	inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*) {
		//����֤��������value_type��PODָ����Plain Old Data��Ҳ���Ǳ����ͱ��ͳ��C struct�ͱ�
		typedef typename __type_traits<T1>::is_POD_type is_POD;
		return __uninitialized_fill_n_aux(first, n, x, is_POD());
	}
	//Ϊָ����Χ[first,first+n)������Ԫ���趨��ͬ�ĳ�ֵ
	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
		return __uninitialized_fill_n(first, n, x, value_type(first));
	}

}//end of namespace yshen

#endif

