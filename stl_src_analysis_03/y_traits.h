#pragma once

/**
 * traits
 * �ͱ���ȡ
 */

template<class I>
struct y_iterator_traits {
	typedef typename I::iterator_category	iterator_category;//����������

	/*
	�����ƶ����ԺͲ�������Ϊ����
	Input Iterator:��ָ����ֻ��
	Output Iterator:ֻд
	Forward Iterator:����д�����㷨������replace()���ڴ��ֵ�����������ж�д
	Bidirectional Iterator:˫���ƶ�
	Random Access Iterator:������ʣ���������ָ����������������p+n,p-n,p[n],p1-p2,p1<p2;��ǰ����֧��++�������ּ���--��
	*/


	typedef typename I::value_type			value_type;//��������ָ�����ͱ�
	typedef typename I::difference_type		difference_type;//��������֮��ľ��룬Ҳ�ɱ�ʾ�����������
	typedef typename I::pointer				pointer;//��Ӧ��reference��������ָ�����ַ
	typedef typename I::reference			reference;//��Ӧ��pointer��������ָ��������
};

//һЩӦ��
//�������
template<class I, class T>
typename iterator_traits<I>::difference_type
y_count(I first, I last, const T& value) {
	typename iterator_traits<I>::difference_type n = 0;
	for (; first != last; ++first)
		if (*first == value)
			++n;
	return n;
}
//��ͬ���͵�����
template<class InputIterator, class Distance>
void y_advance_II(InputIterator& i, Distance n) { while (n--) ++i; }
template<class BidirectionalIterator, class Distance>
void y_advance_BI(BidirectionalIterator& i, Distance n) {
	if (n >= 0)
		while (n--) ++i;
	else
		while (n++) --i;
}
template<class RandomAccessIterator, class Distance>
void y_advance_RAI(RandomAccessIterator& i, Distance n) { i += n; }
//�ж�����
template<class InputIterator, class Distance>
void y_advance(InputIterator& i, Distance n) {
	//ִ���ھ���ʹ���ĸ��汾
	if (is_random_access_iterator(i))
		y_advance_RAI(i, n);
	else if (is_bidirectional_iterator(i))
		y_advance_BI(i, n);
	else
		y_advance_II(i, n);
}
//���淽��Ч�ʲ��ߣ�����ʹ��һЩ����Ϊ���
struct y_input_iterator_tag {};
struct y_output_iterator_tag {};
struct y_forward_iterator_tag : public y_input_iterator_tag {};
struct y_bidirectional_iterator_tag : public y_forward_iterator_tag {};
struct y_random_access_iterator_tag : public y_bidirectional_iterator_tag {};
//����
template<class InputIterator, class Distance>
inline void __y_advance(InputIterator& i, Distance n, y_input_iterator_tag) { while (n--) ++i; }
template<class ForwardIterator, class Distance>
inline void __y_advance(ForwardIterator& i, Distance n, y_forward_iterator_tag) { __y_advance(i, n, y_input_iterator_tag()); /* ����ת�� */ }
template<class BidirectionalIterator, class Distance>
void __y_advance(BidirectionalIterator& i, Distance n, y_bidirectional_iterator_tag) {
	if (n >= 0)
		while (n--) ++i;
	else
		while (n++) --i;
}
template<class RandomAccessIterator, class Distance>
void __y_advance(RandomAccessIterator& i, Distance n, y_random_access_iterator_tag) { i += n; }
//�ϲ���ƽӿ�
template<class InputIterator/*STL�淶������ͽ���������*/, class Distance>
inline void advance(InputIterator& i, Distance n)
{
	__y_advance(i, n, y_iterator_traits<InputIterator>::iterator_category());//ʹ������ʱ����
}

//���ԭ��ָ���pointer-to-constƫ�ػ�
template<class T>
struct y_iterator_traits<T*> {
	typedef y_random_access_iterator_tag	iterator_category;//ԭ��ָ����һ��������ʵ�����
	typedef T								value_type;
	typedef ptrdiff_t						difference_type;
	typedef T* pointer;
	typedef T& reference;
};
template<class T>
struct y_iterator_traits<const T*> {
	typedef y_random_access_iterator_tag	iterator_category;
	typedef T								value_type;
	typedef ptrdiff_t						difference_type;
	typedef const T* pointer;
	typedef const T& reference;
};

/**
 * ������
 */
template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Referance = T&>
struct y_iterator {
	typedef Category		iterator_category;
	typedef T				value_type;
	typedef Distance		difference_type;
	typedef Pointer			pointer;
	typedef Referance		reference;
};
//�����������ʱ����ʽ���
template<class Item>
struct y_list_iter : public std::iterator<std::forward_iterator_tag, Item> {};

//��ȡ���ͱ�ĺ�����װ
template<class Iterator>
inline typename y_iterator_traits<Iterator>::iterator_category
y_iterator_category(const Iterator&) {
	typedef typename y_iterator_traits<Iterator>::iterator_category category;
	return category();
}
template<class Iterator>
inline typename y_iterator_traits<Iterator>::difference_type*
y_distance_type(const Iterator&) {
	return static_cast<typename y_iterator_traits<Iterator>::difference_type*>(0);
}
template<class Iterator>
inline typename y_iterator_traits<Iterator>::value_type*
y_value_type(const Iterator&) {
	return static_cast<typename y_iterator_traits<Iterator>::value_type*>(0);
}



//һЩ������ֵ��ֵ���﷨����
void test_something() {
	//ʹ��*�������ǣ���ֵ����ֵ��������ֵ������
	int* pi = new int(5);
	const int* pci = new int(9);//pointer-to-constָ���ֵΪconst��ptr������const��ͬд��int const * pci;
	int* const cpi = new int(6);//const ptr��const�����ұߵ�����
	*pi = 7;
	//*pci = 1; //*pci��ȡһ����ֵ��������ֵ
	pci = pi;//pciָ�뱾����Ը�ֵ
	//cpi = pi; //const ptr
	cout << *pi << " " << *pci << " " << *cpi << endl;
}