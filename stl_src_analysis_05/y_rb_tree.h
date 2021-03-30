#pragma once

/**
 * 红黑树基本设计
 */

typedef bool __rb_tree_color_type;
const __rb_tree_color_type __rb_tree_red = false;
const __rb_tree_color_type __rb_tree_black = true;
//节点设计
struct __rb_tree_node_base {
	typedef __rb_tree_color_type color_type;
	typedef __rb_tree_node_base* base_ptr;
	//成员
	color_type color;
	base_ptr parent;
	base_ptr left;
	base_ptr right;
	//求最大/小值
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
//迭代器设计
struct __rb_tree_iterator_base {
	typedef __rb_tree_node_base::base_ptr base_ptr;
	typedef bidirectional_iterator_tag iterator_category;
	typedef ptrdiff_t difference_type;

	base_ptr node;//与容器产生关联

	void increment() { //其实就是用于operator++
		if (node->right != 0)
		{
			node = node->right;//如果有右子节点，则右子树的沿左走到头就是下一个元素
			while (node->left != 0)
				node = node->left;
		}
		else {
			base_ptr y = node->parent;//如果没有右子节点，向上找父节点
			while (node == y->right)
			{
				node = y;//如果当前节点是父节点的右子节点，就一直上溯
				y = y->parent;
			}
			//需要判断当前右子节点不等于此时父节点，是为了处理一个特殊情况；已经搜索到根节点，而根节点无右子节点
			if (node->right != y)
				node = y;
		}
	}
	void decrement() {//operator--
		if (node->color == __rb_tree_red && node->parent->parent == node)
			node = node->right;//当前是红节点，且祖父节点为自己，发生于node为header（一个特殊的虚节点，也是end()），header右子节点为mostright，也就是整棵树的max节点
		else if (node->left != 0)
		{
			base_ptr y = node->left;//有左子节点，则从左子树沿右走到头就是前一个元素
			while (y->right != 0)
				y = y->right;
			node = y;
		}
		else
		{
			//无左子节点，往上找父节点
			base_ptr y = node->parent;
			while (node == y->left)
			{
				node = y;//如果是父节点的左子节点，就一直往上，直到当前节点不是左子节点
				y = y->parent;
			}
			node = y;//最后，父节点就是操作结果
		}
	}
};
template<class Value, class Ref, class Ptr>
struct __rb_tree_iterator :public __rb_tree_iterator_base {
	//类型定义
	typedef Value value_type;
	typedef Ref reference;
	typedef Ptr pointer;
	typedef __rb_tree_iterator<Value, Value&, Value*> iterator;
	typedef __rb_tree_iterator<Value, const Value&, const Value*> const_iterator;
	typedef __rb_tree_iterator<Value, Ref, Ptr> self;
	typedef __rb_tree_node<Value>* link_type;
	//构造
	__rb_tree_iterator() {}
	__rb_tree_iterator(link_type x) { node = x; }
	__rb_tree_iterator(const iterator& it) { node = it.node; }
	//操作符
	reference operator*() const { return link_type(node)->value_field; }
	pointer operator->() const { return &(operator*()); }
	self& operator++() { increment(); return *this; }//前置
	self operator++(int) {//后置
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
inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root);//全局函数，重新平衡红黑树
inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root);//全局函数，以x左旋
inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root);//全局函数，以x右旋
//红黑树数据结构
//
//	begin()				end()
//	mostleft			↙
//		 -------header------ mostright
//		|		 ↑↓		|
//		|		root()		| （root和header互为父节点，这是一个实现技巧）
//		|	↙			↘  |
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
		link_type temp = get_node();//配置空间
		try {
			construct(&temp->value_field, x);//构造内容
		}
		catch (...) {
			put_node(temp);
		}
		return temp;
	}
	link_type clone_node(link_type x) {
		link_type temp = create_node(x->value_field);
		//复制一个节点，除了值，还有颜色
		temp->color = x->color;
		temp->left = 0;
		temp->right = 0;
		return temp;
	}
	void destroy_node(link_type p) {
		destroy(&p->value_field);//析构内容
		put_node(p);//释放空间
	}
protected:
	size_type node_count;//节点数量
	link_type header;//实现技巧
	Compare key_compare;//用来比较键值大小的func obj
	//header相关
	link_type& root() const { return (link_type&)header->parent; }//header和root互为父节点
	link_type& leftmost() const { return (link_type&)header->left; }//header左指树中最小值
	link_type& rightmost() const { return (link_type&)header->right; }//header右指树的最大值
	//取x的成员
	static link_type left(link_type x) { return (link_type&)(x->left); }
	static link_type right(link_type x) { return (link_type&)(x->right); }
	static link_type parent(link_type x) { return (link_type&)(x->parent); }
	static reference value(link_type x) { return x->value_field; }
	static const Key& key(link_type x) { return KeyOfValue()(value(x)); }
	static color_type& color(link_type x) { return (color_type&)(x->color); }
	//base版本
	static link_type left(base_ptr x) { return (link_type&)(x->left); }
	static link_type right(base_ptr x) { return (link_type&)(x->right); }
	static link_type parent(base_ptr x) { return (link_type&)(x->parent); }
	static reference value(base_ptr x) { return link_type(x)->value_field; }
	static const Key& key(base_ptr x) { return KeyOfValue()(value(link_type(x))); }
	static color_type& color(base_ptr x) { return (color_type&)(link_type(x)->color); }
	//求极值，复用node中的实现
	static link_type minimum(link_type x) { return (link_type)__rb_tree_node_base::minimum(x); }
	static link_type maximum(link_type x) { return (link_type)__rb_tree_node_base::maximum(x); }
public:
	typedef __rb_tree_iterator<value_type, reference, pointer> iterator;//迭代器出场
private:
	iterator __insert(base_ptr x, base_ptr y, const value_type& v);
	link_type __copy(link_type x, link_type p);
	void __erase(link_type x);
	void init() {
		header = get_node();//产生一个节点空间，header指向它
		color(header) = __rb_tree_red;//header默认定义为红色，区分header与root
		//初始化后，root为空，header指向的最大最小值都为header自己
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
	pair<iterator, bool> insert_unique(const value_type& x);//键值唯一
	iterator insert_equal(const value_type& x);//键值可重复
	iterator find(const Key& k);//寻找是否右键值为k的节点
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
		x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);//x更大则插入点在x左，反之往右
	}
	return __insert(x, y, v);//x为新插入点，y为插入点父节点
}
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& v) {
	link_type y = header;
	link_type x = root();
	//同样从根开始，寻找合适插入点
	bool comp = true;
	while (x != 0) {
		y = x;
		comp = key_compare(KeyOfValue()(v), key(x));
		x = comp ? left(x) : right(x);
	}
	//离开循环后，x为叶节点，y为其父节点
	iterator j = iterator(y);
	if (comp)//若离开while循环时，comp为真（代表应插入于左侧）
		if (j == begin())//若插入点父节点为最左节点
			return pair<iterator, bool>(__insert(x, y, v), true);
		else
			--j;//不是最左，调整j
	if (key_compare(key(j.node), KeyOfValue()(v)))//插入右侧
		return pair<iterator, bool>(__insert(x, y, v), true);
	//若能进行到此，则说明键重复
	return pair<iterator, bool>(j, false);
}
//真正的插入操作实现
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(base_ptr x_, base_ptr y_, const value_type& v) {
	//x_为插入点，y_为插入点的父节点，v为新值
	link_type x = (link_type)x_;
	link_type y = (link_type)y_;
	link_type z;
	if (y == header || x != 0 || key_compare(KeyOfValue()(v), key(y)))
	{
		z = create_node(v);//产生新节点
		left(y) = z;//当y就是header时，leftmost() = z;
		if (y == header)
		{
			root() = z;
			rightmost() = z;
		}
		else if (y == leftmost()) //y为最左节点
			leftmost() = z;
	}
	else
	{
		z = create_node(v);
		right(y) = z;//令新节点y的右子节点
		if (y == rightmost())
			rightmost() = z;//若y就是最右节点，改变rightmost()指向
	}
	//设置新节点属性
	parent(z) = y;
	left(z) = 0;
	right(z) = 0;
	//重新平衡树，包括调整颜色，也在此方法中进行
	__rb_tree_rebalance(z, header->parent);//参数为新增节点和root节点
	++node_count;//累加节点计数
	return iterator(z);//返回新增节点的迭代器
}
//调整红黑树，旋转及调色
inline void
__rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
	x->color = __rb_tree_red;//新节点设置为红

	while (x != root && x->parent->color == __rb_tree_red)//当父节点为红（x为根节点则不需要调整，只改变颜色即可）
	{
		if (x->parent == x->parent->parent->left)//父节点为祖父节点左子节点
		{
			__rb_tree_node_base* y = x->parent->parent->right;//用到伯父节点
			if (y && y->color == __rb_tree_red)//伯父节点存在且也为红色，则利用前面说的方式，更改祖父节点为红色，并将父节点和伯父节点变黑
			{
				x->parent->color = __rb_tree_black;
				y->color = __rb_tree_black;
				x->parent->parent->color = __rb_tree_red;
				x = x->parent->parent;//上溯，调整颜色
			}
			else//没有伯父节点或伯父节点为黑，不满足都是红的条件
			{
				if (x == x->parent->right)//新节点为父节点的右子节点，内侧插入，先旋转一次
				{
					x = x->parent;
					__rb_tree_rotate_left(x, root);//以x父节点左旋
				}
				//外侧插入，改变颜色，右旋一次
				x->parent->color = __rb_tree_black;
				x->parent->parent->color = __rb_tree_red;
				__rb_tree_rotate_right(x->parent->parent, root);//以x祖父右旋
			}
		}
		else //父节点为祖父节点右子节点
		{
			__rb_tree_node_base* y = x->parent->parent->left;//用到伯父节点
			if (y && y->color)//伯父节点存在且也为红色，则利用前面说的方式，更改祖父节点为红色，并将父节点和伯父节点变黑
			{
				x->parent->color = __rb_tree_black;
				y->color = __rb_tree_black;
				x->parent->parent->color = __rb_tree_red;
				x = x->parent->parent;//继续上溯检查颜色
			}
			else//没有伯父节点或伯父节点为黑，不满足都是红的条件，需要旋转
			{
				if (x == x->parent->left)//新节点为父节点的左子节点，说明是内测插入，先旋转一次
				{
					x = x->parent;
					__rb_tree_rotate_right(x, root);//以x父节点右旋
				}
				//位于外侧，改变颜色，左旋一次
				x->parent->color = __rb_tree_black;
				x->parent->parent->color = __rb_tree_red;
				__rb_tree_rotate_left(x->parent->parent, root);//以旋转后x祖父节点左旋
			}

		}
	}

	root->color = __rb_tree_black;//根节点一定为黑
}
//左旋函数
inline void
__rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
	//x为旋转点
	__rb_tree_node_base* y = x->right;//用到x右子节点
	x->right = y->left;
	if (y->left != 0)
		y->left->parent = x;
	y->parent = x->parent;//令x的父节点成为y的父节点
	//令y完全顶替x的位置
	if (x == root) //x为根节点
		root = y;
	else if (x == x->parent->left)//x为父节点的左子节点
		x->parent->left = y;
	else //x为父节点的右子节点
		x->parent->right = y;
	//调整x为y的左子节点
	y->left = x;
	x->parent = y;
}
//右旋函数
inline void
__rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
	//x为旋转点
	__rb_tree_node_base* y = x->left;//用到x左子节点
	x->left = y->right;
	if (y->right != 0)
		y->right->parent = x;//若存在，则设置其父节点为x
	y->parent = x->parent;//令x父节点成为y父节点
	//令y完全顶替x，将x对其父节点的关系完全接收
	if (x == root)//x为根节点
		root = y;
	else if (x == x->parent->right)//x为父节点右子节点
		x->parent->right = y;
	else//x为父节点左子节点
		x->parent->left = y;
	//调整x为y的右子节点
	y->right = x;
	x->parent = y;
}
//搜索树中是否含有键值为k的节点
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k) {
	link_type y = header;
	link_type x = root();//从root开始寻找
	while (x != 0)
	{
		//以key_compare作为比较准则，其为func obj
		if (!key_compare(key(x), k))
			y = x, x = left(x);//表示k比当前位置key小，向左寻找
		else
			x = right(x);//表示x比当前位置key大，向右寻找

		iterator j = iterator(y);
		return (j == end() || key_compare(k, key(j.node))) ? end() : j;
	}
}