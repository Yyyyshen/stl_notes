#pragma once

/**
 * ������������
 */

typedef bool __rb_tree_color_type;
const __rb_tree_color_type __rb_tree_red = false;
const __rb_tree_color_type __rb_tree_black = true;
//�ڵ����
struct __rb_tree_node_base {
	typedef __rb_tree_color_type color_type;
	typedef __rb_tree_node_base* base_ptr;
	//��Ա
	color_type color;
	base_ptr parent;
	base_ptr left;
	base_ptr right;
	//�����/Сֵ
	static base_ptr minimum(base_ptr x) {
		while (x->left != 0) x = x->left;
		return x;
	}
	static base_ptr maximum(base_ptr x) {
		while (x->right != 0) x = x->right;
		return x;
	}
};
template<class Value>
struct __rb_tree_node :public __rb_tree_node_base {
	typedef __rb_tree_node<Value>* link_type;
	Value value_field;
};
//���������
struct __rb_tree_iterator_base {
	typedef __rb_tree_node_base::base_ptr base_ptr;
	typedef bidirectional_iterator_tag iterator_category;
	typedef ptrdiff_t difference_type;

	base_ptr node;//��������������

	void increment() { //��ʵ��������operator++
		if (node->right != 0)
		{
			node = node->right;//��������ӽڵ㣬���������������ߵ�ͷ������һ��Ԫ��
			while (node->left != 0)
				node = node->left;
		}
		else {
			base_ptr y = node->parent;//���û�����ӽڵ㣬�����Ҹ��ڵ�
			while (node == y->right)
			{
				node = y;//�����ǰ�ڵ��Ǹ��ڵ�����ӽڵ㣬��һֱ����
				y = y->parent;
			}
			//��Ҫ�жϵ�ǰ���ӽڵ㲻���ڴ�ʱ���ڵ㣬��Ϊ�˴���һ������������Ѿ����������ڵ㣬�����ڵ������ӽڵ�
			if (node->right != y)
				node = y;
		}
	}
	void decrement() {//operator--
		if (node->color == __rb_tree_red && node->parent->parent == node)
			node = node->right;//��ǰ�Ǻ�ڵ㣬���游�ڵ�Ϊ�Լ���������nodeΪheader��һ���������ڵ㣬Ҳ��end()����header���ӽڵ�Ϊmostright��Ҳ������������max�ڵ�
		else if (node->left != 0)
		{
			base_ptr y = node->left;//�����ӽڵ㣬��������������ߵ�ͷ����ǰһ��Ԫ��
			while (y->right != 0)
				y = y->right;
			node = y;
		}
		else
		{
			//�����ӽڵ㣬�����Ҹ��ڵ�
			base_ptr y = node->parent;
			while (node == y->left)
			{
				node = y;//����Ǹ��ڵ�����ӽڵ㣬��һֱ���ϣ�ֱ����ǰ�ڵ㲻�����ӽڵ�
				y = y->parent;
			}
			node = y;//��󣬸��ڵ���ǲ������
		}
	}
};
template<class Value, class Ref, class Ptr>
struct __rb_tree_iterator :public __rb_tree_iterator_base {
	//���Ͷ���
	typedef Value value_type;
	typedef Ref reference;
	typedef Ptr pointer;
	typedef __rb_tree_iterator<Value, Value&, Value*> iterator;
	typedef __rb_tree_iterator<Value, const Value&, const Value*> const_iterator;
	typedef __rb_tree_iterator<Value, Ref, Ptr> self;
	typedef __rb_tree_node<Value>* link_type;
	//����
	__rb_tree_iterator() {}
	__rb_tree_iterator(link_type x) { node = x; }
	__rb_tree_iterator(const iterator& it) { node = it.node; }
	//������
	reference operator*() const { return link_type(node)->value_field; }
	pointer operator->() const { return &(operator*()); }
	self& operator++() { increment(); return *this; }//ǰ��
	self operator++(int) {//����
		self temp = *this;
		increment();
		return temp;
	}
	self& operator--() { decrement(); return *this; }
	self operator--(int) {
		self temp = *this;
		decrement();
		return temp;
	}
};
inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root);//ȫ�ֺ���������ƽ������
inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root);//ȫ�ֺ�������x����
inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root);//ȫ�ֺ�������x����
//��������ݽṹ
//
//	begin()				end()
//	mostleft			�L
//		 -------header------ mostright
//		|		 ����		|
//		|		root()		| ��root��header��Ϊ���ڵ㣬����һ��ʵ�ּ��ɣ�
//		|	�L			�K  |
//		left			right
//
template <class T, class Alloc>
class y_simple_alloc {
public:
	static T* allocate(size_t n) { return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T)); }
	static T* allocate(void) { return (T*)Alloc::allocate(sizeof(T)); }
	static void deallocate(T* p, size_t n) { if (0 != n)Alloc::deallocate(p, n * sizeof(T)); }
	static void deallocate(T* p) { Alloc::deallocate(p, sizeof(T)); }
};
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
class rb_tree {
protected:
	typedef void* void_pointer;
	typedef __rb_tree_node_base* base_ptr;
	typedef __rb_tree_node<Value> rb_tree_node;
	typedef y_simple_alloc<rb_tree_node, Alloc> rb_tree_node_allocator;
	typedef __rb_tree_color_type color_type;
public:
	typedef Key key_value;
	typedef Value value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef rb_tree_node* link_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
protected:
	link_type get_node() { return rb_tree_node_allocator::allocate(); }
	void put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }
	link_type create_node(const value_type& x) {
		link_type temp = get_node();//���ÿռ�
		try {
			construct(&temp->value_field, x);//��������
		}
		catch (...) {
			put_node(temp);
		}
		return temp;
	}
	link_type clone_node(link_type x) {
		link_type temp = create_node(x->value_field);
		//����һ���ڵ㣬����ֵ��������ɫ
		temp->color = x->color;
		temp->left = 0;
		temp->right = 0;
		return temp;
	}
	void destroy_node(link_type p) {
		destroy(&p->value_field);//��������
		put_node(p);//�ͷſռ�
	}
protected:
	size_type node_count;//�ڵ�����
	link_type header;//ʵ�ּ���
	Compare key_compare;//�����Ƚϼ�ֵ��С��func obj
	//header���
	link_type& root() const { return (link_type&)header->parent; }//header��root��Ϊ���ڵ�
	link_type& leftmost() const { return (link_type&)header->left; }//header��ָ������Сֵ
	link_type& rightmost() const { return (link_type&)header->right; }//header��ָ�������ֵ
	//ȡx�ĳ�Ա
	static link_type left(link_type x) { return (link_type&)(x->left); }
	static link_type right(link_type x) { return (link_type&)(x->right); }
	static link_type parent(link_type x) { return (link_type&)(x->parent); }
	static reference value(link_type x) { return x->value_field; }
	static const Key& key(link_type x) { return KeyOfValue()(value(x)); }
	static color_type& color(link_type x) { return (color_type&)(x->color); }
	//base�汾
	static link_type left(base_ptr x) { return (link_type&)(x->left); }
	static link_type right(base_ptr x) { return (link_type&)(x->right); }
	static link_type parent(base_ptr x) { return (link_type&)(x->parent); }
	static reference value(base_ptr x) { return link_type(x)->value_field; }
	static const Key& key(base_ptr x) { return KeyOfValue()(value(link_type(x))); }
	static color_type& color(base_ptr x) { return (color_type&)(link_type(x)->color); }
	//��ֵ������node�е�ʵ��
	static link_type minimum(link_type x) { return (link_type)__rb_tree_node_base::minimum(x); }
	static link_type maximum(link_type x) { return (link_type)__rb_tree_node_base::maximum(x); }
public:
	typedef __rb_tree_iterator<value_type, reference, pointer> iterator;//����������
private:
	iterator __insert(base_ptr x, base_ptr y, const value_type& v);
	link_type __copy(link_type x, link_type p);
	void __erase(link_type x);
	void init() {
		header = get_node();//����һ���ڵ�ռ䣬headerָ����
		color(header) = __rb_tree_red;//headerĬ�϶���Ϊ��ɫ������header��root
		//��ʼ����rootΪ�գ�headerָ��������Сֵ��Ϊheader�Լ�
		root() = 0;
		leftmost() = header;
		rightmost() = header;
	}
public:
	rb_tree(const Compare& comp = Compare()) :node_count(0), key_compare(comp) { init(); }
	~rb_tree() { clear(); put_node(header); }
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& operator=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x);
public:
	Compare key_comp() const { return key_compare; }
	iterator begin() { return leftmost(); }
	iterator end() { return header; }
	bool empty() const { return node_count == 0; }
	size_type size() const { return node_count; }
	size_type max_size() const { return size_type(-1); }
	void clear();
public:
	pair<iterator, bool> insert_unique(const value_type& x);//��ֵΨһ
	iterator insert_equal(const value_type& x);//��ֵ���ظ�
	iterator find(const Key& k);//Ѱ���Ƿ��Ҽ�ֵΪk�Ľڵ�
	//...

};
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type& v) {
	link_type y = header;
	link_type x = root();
	while (x != 0)
	{
		y = x;
		x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);//x������������x�󣬷�֮����
	}
	return __insert(x, y, v);//xΪ�²���㣬yΪ����㸸�ڵ�
}
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& v) {
	link_type y = header;
	link_type x = root();
	//ͬ���Ӹ���ʼ��Ѱ�Һ��ʲ����
	bool comp = true;
	while (x != 0) {
		y = x;
		comp = key_compare(KeyOfValue()(v), key(x));
		x = comp ? left(x) : right(x);
	}
	//�뿪ѭ����xΪҶ�ڵ㣬yΪ�丸�ڵ�
	iterator j = iterator(y);
	if (comp)//���뿪whileѭ��ʱ��compΪ�棨����Ӧ��������ࣩ
		if (j == begin())//������㸸�ڵ�Ϊ����ڵ�
			return pair<iterator, bool>(__insert(x, y, v), true);
		else
			--j;//�������󣬵���j
	if (key_compare(key(j.node), KeyOfValue()(v)))//�����Ҳ�
		return pair<iterator, bool>(__insert(x, y, v), true);
	//���ܽ��е��ˣ���˵�����ظ�
	return pair<iterator, bool>(j, false);
}
//�����Ĳ������ʵ��
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(base_ptr x_, base_ptr y_, const value_type& v) {
	//x_Ϊ����㣬y_Ϊ�����ĸ��ڵ㣬vΪ��ֵ
	link_type x = (link_type)x_;
	link_type y = (link_type)y_;
	link_type z;
	if (y == header || x != 0 || key_compare(KeyOfValue()(v), key(y)))
	{
		z = create_node(v);//�����½ڵ�
		left(y) = z;//��y����headerʱ��leftmost() = z;
		if (y == header)
		{
			root() = z;
			rightmost() = z;
		}
		else if (y == leftmost()) //yΪ����ڵ�
			leftmost() = z;
	}
	else
	{
		z = create_node(v);
		right(y) = z;//���½ڵ�y�����ӽڵ�
		if (y == rightmost())
			rightmost() = z;//��y�������ҽڵ㣬�ı�rightmost()ָ��
	}
	//�����½ڵ�����
	parent(z) = y;
	left(z) = 0;
	right(z) = 0;
	//����ƽ����������������ɫ��Ҳ�ڴ˷����н���
	__rb_tree_rebalance(z, header->parent);//����Ϊ�����ڵ��root�ڵ�
	++node_count;//�ۼӽڵ����
	return iterator(z);//���������ڵ�ĵ�����
}
//�������������ת����ɫ
inline void
__rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
	x->color = __rb_tree_red;//�½ڵ�����Ϊ��

	while (x != root && x->parent->color == __rb_tree_red)//�����ڵ�Ϊ�죨xΪ���ڵ�����Ҫ������ֻ�ı���ɫ���ɣ�
	{
		if (x->parent == x->parent->parent->left)//���ڵ�Ϊ�游�ڵ����ӽڵ�
		{
			__rb_tree_node_base* y = x->parent->parent->right;//�õ������ڵ�
			if (y && y->color == __rb_tree_red)//�����ڵ������ҲΪ��ɫ��������ǰ��˵�ķ�ʽ�������游�ڵ�Ϊ��ɫ���������ڵ�Ͳ����ڵ���
			{
				x->parent->color = __rb_tree_black;
				y->color = __rb_tree_black;
				x->parent->parent->color = __rb_tree_red;
				x = x->parent->parent;//���ݣ�������ɫ
			}
			else//û�в����ڵ�򲮸��ڵ�Ϊ�ڣ������㶼�Ǻ������
			{
				if (x == x->parent->right)//�½ڵ�Ϊ���ڵ�����ӽڵ㣬�ڲ���룬����תһ��
				{
					x = x->parent;
					__rb_tree_rotate_left(x, root);//��x���ڵ�����
				}
				//�����룬�ı���ɫ������һ��
				x->parent->color = __rb_tree_black;
				x->parent->parent->color = __rb_tree_red;
				__rb_tree_rotate_right(x->parent->parent, root);//��x�游����
			}
		}
		else //���ڵ�Ϊ�游�ڵ����ӽڵ�
		{
			__rb_tree_node_base* y = x->parent->parent->left;//�õ������ڵ�
			if (y && y->color)//�����ڵ������ҲΪ��ɫ��������ǰ��˵�ķ�ʽ�������游�ڵ�Ϊ��ɫ���������ڵ�Ͳ����ڵ���
			{
				x->parent->color = __rb_tree_black;
				y->color = __rb_tree_black;
				x->parent->parent->color = __rb_tree_red;
				x = x->parent->parent;//�������ݼ����ɫ
			}
			else//û�в����ڵ�򲮸��ڵ�Ϊ�ڣ������㶼�Ǻ����������Ҫ��ת
			{
				if (x == x->parent->left)//�½ڵ�Ϊ���ڵ�����ӽڵ㣬˵�����ڲ���룬����תһ��
				{
					x = x->parent;
					__rb_tree_rotate_right(x, root);//��x���ڵ�����
				}
				//λ����࣬�ı���ɫ������һ��
				x->parent->color = __rb_tree_black;
				x->parent->parent->color = __rb_tree_red;
				__rb_tree_rotate_left(x->parent->parent, root);//����ת��x�游�ڵ�����
			}

		}
	}

	root->color = __rb_tree_black;//���ڵ�һ��Ϊ��
}
//��������
inline void
__rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
	//xΪ��ת��
	__rb_tree_node_base* y = x->right;//�õ�x���ӽڵ�
	x->right = y->left;
	if (y->left != 0)
		y->left->parent = x;
	y->parent = x->parent;//��x�ĸ��ڵ��Ϊy�ĸ��ڵ�
	//��y��ȫ����x��λ��
	if (x == root) //xΪ���ڵ�
		root = y;
	else if (x == x->parent->left)//xΪ���ڵ�����ӽڵ�
		x->parent->left = y;
	else //xΪ���ڵ�����ӽڵ�
		x->parent->right = y;
	//����xΪy�����ӽڵ�
	y->left = x;
	x->parent = y;
}
//��������
inline void
__rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
	//xΪ��ת��
	__rb_tree_node_base* y = x->left;//�õ�x���ӽڵ�
	x->left = y->right;
	if (y->right != 0)
		y->right->parent = x;//�����ڣ��������丸�ڵ�Ϊx
	y->parent = x->parent;//��x���ڵ��Ϊy���ڵ�
	//��y��ȫ����x����x���丸�ڵ�Ĺ�ϵ��ȫ����
	if (x == root)//xΪ���ڵ�
		root = y;
	else if (x == x->parent->right)//xΪ���ڵ����ӽڵ�
		x->parent->right = y;
	else//xΪ���ڵ����ӽڵ�
		x->parent->left = y;
	//����xΪy�����ӽڵ�
	y->right = x;
	x->parent = y;
}
//���������Ƿ��м�ֵΪk�Ľڵ�
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k) {
	link_type y = header;
	link_type x = root();//��root��ʼѰ��
	while (x != 0)
	{
		//��key_compare��Ϊ�Ƚ�׼����Ϊfunc obj
		if (!key_compare(key(x), k))
			y = x, x = left(x);//��ʾk�ȵ�ǰλ��keyС������Ѱ��
		else
			x = right(x);//��ʾx�ȵ�ǰλ��key������Ѱ��

		iterator j = iterator(y);
		return (j == end() || key_compare(k, key(j.node))) ? end() : j;
	}
}