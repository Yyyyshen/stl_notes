#pragma once

/**
 * heap
 * ��Ϊmax-heap��min-heap
 * ÿ���ڵ㶼���ڵ��ڣ���С�ڵ��ڣ������ӽڵ㣬Ҳ����˵���ڵ�Ϊ���ֵ������Сֵ��
 * ���ڵ��λ�ö�Ӧ��array����vector������ͷ��
 */
void max_heap() {} //���º�����������max-heapΪ��ʵ��
/**
 * push_heap�㷨
 * ��һ����Ԫ�ؼ���ʱ��Ϊ��������ȫ��������������Ҫ�ȷ������²���ΪҶ�ڵ�
 * ����vector���Ƿ���end()���������ж���Ԫ�ظ��ڵ�λ�ã�����λ�ÿ�Ϊһ���ն����������������бȽϣ����ѽ��������У�
 * �������ݣ����½ڵ��ڵ�ǰ����λ�õĸ��ڵ�Ƚϣ�����ȸ��ڵ�󣨶���max-heap�����򻥻�λ�ã���������ֱ������Ҫ����������λ���Ѿ��Ǹ��ڵ�
 */
template<class RandomAccessIterator, class Distance, class T>
void __y_push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value) {
	Distance parent = (holeIndex - 1) / 2;//��λ���ڵ�λ��
	while (holeIndex > topIndex /*�����Ǹ��ڵ�*/ && *(first + parent) < value /*���ڵ�С���¼����ֵ*/)
	{
		//�����ڵ�ֵ���뵱ǰ����
		*(first + holeIndex) = *(first + parent);
		//��������
		holeIndex = parent;
		//�����¶��ŵĸ��ڵ㣬����ѭ��
		parent = (holeIndex - 1) / 2;
	}
	//����������max_heap���ԣ�������Ԫ�ظ�ֵ
	*(first + holeIndex) = value;
}
template<class RandomAccessIterator, class Distance, class T>
inline void __y_push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*) {
	__y_push_heap(first, Distance(last - first - 1), Distance(0), T(*(last - 1)));
}
template<class RandomAccessIterator>
inline void y_push_heap(RandomAccessIterator first, RandomAccessIterator last) {
	//�˺���������ʱ����������������Ӧ��Ϊ������ͷβ��������Ԫ��Ӧ���Ѿ�push_back��������Ļ�ִ�н����һ��׼ȷ
	__y_push_heap_aux(first, last, distance_type(first), value_type(first));
}
/**
 * pop_heap
 * ���ֵ�ڸ��ڵ㣬pop����Ӧ����ȡ�߸��ڵ�
 * ��vector����ʹ��pop_back()���ܸ�Щ������ʵ�ʲ���Ӧ���ǰѸ��ڵ�����vector�����Ϊ���²��Ҷ�ڵ��õ������߿������ڵ������һ��Ҷ�ӽڵ㽻��ֵ��pop���������
 * ���ң�Ϊ������max-heap���ԣ���Ҫִ��һ�����ݲ���
 * �����ڵ㣨���ֵȡ�ߺ��൱��һ���ն����ն��������һ��Ҷ�ڵ�ֵ�����ӽڵ�Ƚϣ������ϴ���ӽڵ㣬ֱ���ն��·ŵ�����λ�û�Ҷ�ڵ�Ϊֹ
 */
template<class RandomAccessIterator, class Distance, class T>
void __y_adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value) {
	Distance topIndex = holeIndex;
	Distance secondChild = 2 * holeIndex + 2;//���ڵ�����ӽڵ�
	while (secondChild < len) {
		//�Ƚ������ӽڵ�ֵ��Ȼ����secondChild����ϴ��ӽڵ�
		if (*(first + secondChild) < *(first + (secondChild - 1)))
			secondChild--;
		//���ݣ���ϴ���ֵΪ��ֵ�����µ��������ϴ��ӽڵ㴦
		*(first + holeIndex) = *(first + secondChild);
		holeIndex = secondChild;
		secondChild = 2 * (secondChild + 1);
	}
	if (secondChild == len)
	{
		//û�����ӽڵ㣬��������ֵΪ��ֵ��ͬ������
		*(first + holeIndex) = *(first + secondChild - 1);
		holeIndex = secondChild - 1;
	}
	//������ֵ���뵱ǰ���ţ�������push����
	__y_push_heap(first, holeIndex, topIndex, value);//��ʵֱ����*(first+holeIndex) = value;Ӧ��Ҳ���ԣ���Ϊ��ǰ�������Ѿ������˺�heap����
}
template<class RandomAccessIterator, class T, class Distance>
inline void __y_pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*) {
	*result = *first;//�趨βֵΪ��ֵ
	__y_adjust_heap(first, Distance(0), Distance(last - first), value);//���ݵ���heap��value��¼ԭҶ�ӽڵ�ֵ�����������붴������λ��
}
template<class RandomAccessIterator, class T>
inline void __y_pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*) {
	__y_pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first));
}
template<class RandomAccessIterator>
inline void y_pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
	__y_pop_heap_aux(first, last, value_type(first));
}
/**
 * sort_heap
 * ����pop_heapÿ�οɻ��heap�����ֵ�����ԣ�����pop������ÿ�ν���Χ��Сһλ����Ϊpop�������Ȼ�����ֵ�ŵ�β�ˣ�
 * ���ջ�õ�һ����������
 */
template<class RandomAccessIterator>
void y_sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
	while (last - first > 1)
		y_pop_heap(first, last--);//ÿִ��һ�Σ�������Χ��Сһ��
}
/**
 * make_heap
 * ���㷨��������������ת��Ϊheap
 */
template<class RandomAccessIterator, class T, class Distance>
inline void __y_make_heap(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*) {
	if (last - first < 2) return;//����С��2����û�б�Ҫ��������
	Distance len = last - first;
	//ȷ����һ����Ҫ���ŵ�����ͷ������parent��ʾ��������Ӧ����holeIndex��
	Distance parent = (len - 2) / 2;
	while (true)
	{
		__y_adjust_heap(first, parent, len, T(*(first + parent)));
		if (parent == 0) return;//������ڵ������
		parent--;
	}
}
template<class RandomAccessIterator>
inline void y_make_heap(RandomAccessIterator first, RandomAccessIterator last) {
	__y_make_heap(first, last, value_type(first), distance_type(first));
}