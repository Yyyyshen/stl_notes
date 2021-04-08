#pragma once

/**
 * iterator adapters
 */

template<class Container>
class y_back_insert_iterator {
	//����������ֵ������Ϊ������β�˲������
protected:
	Container* container;
public:
	typedef output_iterator_tag iterator_category;//ע������
	typedef void value_type;
	typedef void difference_type;
	typedef void pointer;
	typedef void reference;

	explicit y_back_insert_iterator(Container& x) :container(x) {}
	y_back_insert_iterator<Container>& operator=(const typename Container::value_type& value) {
		container->push_back(value);//��ֵתΪ����push_back
		return *this;
	}
	//�����������رչ��ܣ�ֱ�ӷ����Լ�
	y_back_insert_iterator<Container>& operator*() { return *this; }
	y_back_insert_iterator<Container>& operator++() { return *this; }
	y_back_insert_iterator<Container>& operator++(int) { return *this; }
};
//������������������ʹ��back_insert_iterator
template<class Container>
inline y_back_insert_iterator<Container> y_back_inserter(Container& x) {
	return y_back_insert_iterator<Container>(x);
}
//��ͷ�˲����front_insert_iteratorͬ��ֻ�Ǹı���ת���õĺ���
template<class Container>
class y_front_insert_iterator {
	//����������ֵ������Ϊ������ͷ�˲������
protected:
	Container* container;
public:
	typedef output_iterator_tag iterator_category;//ע������
	typedef void value_type;
	typedef void difference_type;
	typedef void pointer;
	typedef void reference;

	explicit y_front_insert_iterator(Container& x) :container(x) {}
	y_front_insert_iterator<Container>& operator=(const typename Container::value_type& value) {
		container->push_front(value);//��ֵתΪ����push_front
		return *this;
	}
	//�����������رչ��ܣ�ֱ�ӷ����Լ�
	y_front_insert_iterator<Container>& operator*() { return *this; }
	y_front_insert_iterator<Container>& operator++() { return *this; }
	y_front_insert_iterator<Container>& operator++(int) { return *this; }
};
//������������������ʹ��front_insert_iterator
template<class Container>
inline y_front_insert_iterator<Container> y_front_inserter(Container& x) {
	return y_front_insert_iterator<Container>(x);
}
//��ָ��λ�ò�������������������
template<class Container>
class y_insert_iterator {
	//����������ֵ������Ϊ������ͷ�˲������
protected:
	Container* container;
	typename Container::iterator iter;
public:
	typedef output_iterator_tag iterator_category;//ע������
	typedef void value_type;
	typedef void difference_type;
	typedef void pointer;
	typedef void reference;

	explicit y_insert_iterator(Container& x, typename Container::iterator i) :container(x), iter(i) {}
	y_insert_iterator<Container>& operator=(const typename Container::value_type& value) {
		iter = container->insert(iter, value);//��ֵתΪ����insert
		++iter;
		return *this;
	}
	//�����������رչ��ܣ�ֱ�ӷ����Լ�
	y_insert_iterator<Container>& operator*() { return *this; }
	y_insert_iterator<Container>& operator++() { return *this; }
	y_insert_iterator<Container>& operator++(int) { return *this; }
};
//������������������ʹ��insert_iterator
template<class Container, class Iterator>
inline y_insert_iterator<Container> y_inserter(Container& x, Iterator i) {
	typedef typename Container::iterator iter;
	return y_insert_iterator<Container>(x, iter(i));
}

//reverse iterator ���������ƶ���Ϊ��ת��˫�����������������ṩ�˷�ת������
//����rbegin��end��ͬ���ǣ�ǰ��ָ�����һ��Ԫ�أ�����ָ�����һ��Ԫ�صĺ�һ��λ��
//begin��rend��ͬ���ǣ�ǰ��ָ���һ��Ԫ�أ�����ָ���һ��Ԫ�ص�ǰһ��Ԫ��
template<class Iterator>
class y_reverse_iterator {
protected:
	Iterator current;
public:
	typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
	typedef typename iterator_traits<Iterator>::value_type value_type;
	typedef typename iterator_traits<Iterator>::difference_type difference_type;
	typedef typename iterator_traits<Iterator>::pointer pointer;
	typedef typename iterator_traits<Iterator>::reference reference;

	typedef Iterator iterator_type;
	typedef y_reverse_iterator<Iterator> self;
public:
	y_reverse_iterator() {}
	explicit y_reverse_iterator(iterator_type x) : current(x) {}
	y_reverse_iterator(const self& x) :current(x.current) {}

	iterator_type base() const { return current; } //���ض�Ӧ�����������
	reference operator*() const {
		Iterator tmp = current;
		return *--tmp;
		//�����������ȡֵ�����ǽ���Ӧ���������������һ��ȡֵ
	}
	pointer operator->() const { return &(operator*()); }
	//ǰ�������
	self& operator++() {
		--current;
		return *this;
	}
	self operator++(int) {
		//����
		self temp = *this;
		--current;
		return temp;
	}
	//���˱�ǰ��
	self& operator--() {
		++current;
		return *this;
	}
	self operator--(int) {
		self temp = *this;
		++current;
		return temp;
	}
	//�Ӽ�����Ҳ��ת����
	self operator+(difference_type n) const {
		return self(current - n);
	}
	self& operator+=(difference_type n) {
		current -= n;
		return *this;
	}
	self operator-(difference_type n) const {
		return self(current + n);
	}
	self operator-=(difference_type n) {
		current += n;
		return *this;
	}
	//�±����������Ҫע����ǵ�һ��*��+������ñ����ڵ�operator*,operator+�����ڶ���*���ᣬ�жϷ��������ݴ�������Ͷ�����
	reference operator[](difference_type n) const { return *(*this + n); }
};


//stream iterator
//���������󶨵������������ϣ����Է��������ӵ�������������
//�ڵ�������++����ʱ���ᱻ�������õ������ڲ�ά����stream��Ա���Ӷ�ʵ���������
template<class T, class Distance = ptrdiff_t>
class y_istream_iterator {
	//������������󶨵ĵ�����
	friend bool operator==<>(const y_istream_iterator<T, Distance>& x, const y_istream_iterator<T, Distance>& y);//STL��ʹ�ú궨��__STL_NULL_TMPL_ARGSΪ<>����
protected:
	istream* stream;
	T value;
	bool end_marker;
	void read() {
		end_marker = (*stream) ? true : false;
		if (end_marker) *stream >> value;//�ܹؼ�������֮��stream״̬���ܻ�ı䣬Ҫ�����ж�end_marker������eof�����Ͳ���������ʱ������false״̬
		end_marker = (*stream) ? true : false;
	}
public:
	typedef input_iterator_tag iterator_category;
	typedef T value_type;
	typedef Distance difference_type;
	typedef const T* pointer;
	typedef const T& reference;//����Ϊinput iterator���ͣ�������const�ͳ�Ա�Ƚϱ���

	y_istream_iterator() :stream(&cin), end_marker(false) {}//Ĭ��ʹ��cin
	y_istream_iterator(istream& s) :stream(&s) { read(); }

	reference operator*() const { return value; }
	pointer operator->() const { return &(operator*()); }

	y_istream_iterator<T, Distance>& operator++() {
		read();
		return *this;
	}
	y_istream_iterator<T, Distance> operator++(int) {
		y_istream_iterator<T, Distance> temp = *this;
		read();
		return temp;
	}
};
template<class T>
class y_ostream_iterator {
	//���������󶨵�������
protected:
	ostream* stream;
	const char* string;
public:
	typedef output_iterator_tag iterator_category;
	typedef void value_type;
	typedef void difference_type;
	typedef void pointer;
	typedef void reference;

	y_ostream_iterator(ostream& s) :stream(&s), string(0) {}
	y_ostream_iterator(ostream& s, const char* c) :stream(&s), string(c) {}

	y_ostream_iterator<T>& operator=(const T& value) {
		*stream << value;
		if (string) *steam << string;
		return *this;
	}

	y_ostream_iterator<T>& operator*() { return *this; }
	y_ostream_iterator<T>& operator++() { return *this; }
	y_ostream_iterator<T>& operator++(int) { return *this; }
};