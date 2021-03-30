#pragma once

/**
 * set
 * �Ժ����Ϊ�ײ����
 */

template<class Key, class Compare, class Alloc>
class y_set {
public:
	typedef Key key_type;
	typedef Key value_type;
	//keyֵ���Ǽ�ֵ����ʵֵ���Ƚ��㷨Ҳͬ��Ӧ��������
	typedef Compare key_compare;
	typedef Compare value_compare;
private:
	template<class T>
	struct identity :public unary_function<T, T> {
		const T& operator()(const T& x) const { return x; }
	};//�º����������½����˽�
	typedef rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;
	rep_type t;//�ײ���ú����
public:
	//����const���ͣ���Ϊ��ֵ��ʵֵ��ͬ���ԣ������������޸�
	typedef typename rep_type::const_pointer pointer;
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::const_reference reference;
	typedef typename rep_type::const_reference const_reference;
	typedef typename rep_type::const_iterator iterator;
	typedef typename rep_type::const_iterator const_iterator;
	typedef typename rep_type::const_reverse_iterator reverse_iterator;
	typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
	typedef typename rep_type::size_type size_type;
	typedef typename rep_type::difference_type difference_type;
	//����
	y_set() : t(Compare()) {}
	explicit y_set(const Compare& comp) : t(comp) {}
	template<class InputIterator>
	y_set(InputIterator first, InputIterator last) : t(Compare()) { t.insert_unique(first, last); }//��ֵ��������ͬ��multiset������
	template<class InputIterator>
	y_set(InputIterator first, InputIterator last, const Compare& comp) : t(comp) { t.insert_unique(first, last); }
	y_set(const y_set<Key, Compare, Alloc>& x) :t(x.t) {}
	//��ֵ
	y_set<Key, Compare, Alloc>& operator=(const y_set<Key, Compare, Alloc>& x) { t = x.t; return *this; }
	//�����������󲿷�ֱ�ӻ��ں��������
	key_compare key_comp() const { return t.key_comp(); }
	value_compare value_comp() const { return t.key_comp(); }
	iterator begin() const { return t.begin(); }
	iterator end() const { return t.end(); }
	reverse_iterator rbegin() const { return t.rbegin(); }
	reverse_iterator rend() const { return t.rend(); }
	bool empty() const { return t.empty(); }
	size_type size() const { return t.size(); }
	size_type max_size() const { return t.max_size(); }
	void swap(y_set<Key, Compare, Alloc>& x) { t.swap(x.t); }
	typedef pair<iterator, bool> pair_iterator_bool;
	pair_iterator_bool insert(const value_type& x) {
		pair<typename rep_type::iterator, bool> p = t.insert_unique(x);
		return pair_iterator_bool(p.first, p.second);
	}
	iterator insert(iterator position, const value_type& x) {
		typedef typename rep_type::iterator rep_iterator;
		return t.insert_unique((rep_iterator&)position, x);
	}
	void erase(iterator position) {
		typedef typename rep_type::iterator rep_iterator;
		t.erase((rep_iterator&)position);
	}
	size_type erase(const key_type& x) {
		return t.erase(x);
	}
	void erase(iterator first, iterator last) {
		typedef typename rep_type::iterator rep_iterator;
		t.erase((rep_iterator&)first, (rep_iterator&)last);
	}
	void clear() { t.clear(); }
	iterator find(const key_type& x) const { return t.find(x); }
	size_type count(const key_type& x) const { return t.count(x); }
	//...
	friend bool operator==(const y_set&, const y_set&);
	friend bool operator<(const y_set&, const y_set&);
};

template<class Key, class Compare, class Alloc>
inline bool operator==(const y_set<Key, Compare, Alloc>& x, y_set<Key, Compare, Alloc>& y) {
	return x.t == y.t;
}

template<class Key, class Compare, class Alloc>
inline bool operator<(const y_set<Key, Compare, Alloc>& x, y_set<Key, Compare, Alloc>& y) {
	return x.t < y.t;
}