#pragma once
/**
 * stack
 * FILO�Ƚ����
 */
template<class T, class Sequence = deque<T>>
class y_stack {
	friend bool operator==(const y_stack&, const y_stack&);
	friend bool operator<(const y_stack&, const y_stack&);
public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type size_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;
protected:
	Sequence c;//�ײ�������Ĭ��ʹ��deque����listҲ����
public:
	//���õײ�����ʵ�ֹ���
	bool empty() const { return c.empty(); }
	size_type size() const { return c.size(); }
	reference top() { return c.back(); }
	const_reference top() const { return c.back(); }
	void push(const value_type& x) { c.push_back(x); }
	void pop() { c.pop_back(); }//ĩ�˽�ĩ�˳�
	//����Ԫ��Ҫ�����Ƚ������ֻ��stack����Ԫ�زŻᱻȡ�ã�����Ҳ���ṩ������
};

template<class T, class Sequence>
bool operator==(const y_stack<T, Sequence>& x, const y_stack<T, Sequence>& y) {
	return x.c == y.c;
}
template<class T, class Sequence>
bool operator<(const y_stack<T, Sequence>& x, const y_stack<T, Sequence>& y) {
	return x.c < y.c;
}

/**
 * queue
 * FIFO�Ƚ��ȳ�
 */
template<class T, class Sequence = deque<T>>
class y_queue {
	friend bool operator==(const y_queue&, const y_queue&);
	friend bool operator<(const y_queue&, const y_queue&);
public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type size_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;
protected:
	Sequence c;//�ײ�������Ĭ��ʹ��deque����listҲ����
public:
	//���õײ�����ʵ�ֹ���
	bool empty() const { return c.empty(); }
	size_type size() const { return c.size(); }
	reference front() { return c.front(); }
	const_reference front() const { return c.front(); }
	reference back() { return c.back(); }
	const_reference back() const { return c.back(); }
	void push(const value_type& x) { c.push_back(x); }
	void pop() { c.pop_front(); }//ĩ�˽�ǰ�˳�
	//����Ԫ��Ҫ�����Ƚ��ȳ���ֻ����β���õ�������Ҳ���ṩ������
};

template<class T, class Sequence>
bool operator==(const y_queue<T, Sequence>& x, const y_queue<T, Sequence>& y) {
	return x.c == y.c;
}
template<class T, class Sequence>
bool operator<(const y_queue<T, Sequence>& x, const y_queue<T, Sequence>& y) {
	return x.c < y.c;
}