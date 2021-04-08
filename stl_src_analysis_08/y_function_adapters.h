#pragma once

/**
 * function adapters
 *
 * 容器以class templates完成，算法以function templates完成
 * 仿函数时一种将operator()重载的class template，迭代器则是将operator++等指针常用操作重载的class template
 * 适配器则是应用于容器和迭代器的class template
 * 如果应用于仿函数身上，就是function adapters
 *
 * container adapters内藏一个container member
 * iterator adapter内藏一个iterator member
 * 每个function adapter则是内藏了一个member object，类型为可适配的仿函数（adaptable functor）
 */


template<class Operation>
class y_binder1st : public unary_function<typename Operation::second_argument_type, typename Operation::result_type> {
protected:
	Operation op;
	typename Operation::first_argument_type value;
public:
	y_binder1st(const Operation& x, const typename Operation::first_argument_type& y) :op(x), value(y) {}//将表达式和第一参数记录
	typename Operation::result_type operator()(const typename Operation::second_argument_type& x) const {
		return op(value, x);//将value绑定为第一个参数
	}
};
//辅助函数
template<class Operation, class T>
inline y_binder1st<Operation> y_bind1st(const Operation& op, const T& x) {
	typedef typename Operation::first_argument_type arg1_type;
	return y_binder1st<Operation>(op, arg1_type(x));
}
template<class Operation>
class y_binder2nd : public unary_function<typename Operation::first_argument_type, typename Operation::result_type> {
protected:
	Operation op;
	typename Operation::second_argument_type value;
public:
	y_binder2nd(const Operation& x, const typename Operation::second_argument_type& y) :op(x), value(y) {}
	typename Operation::result_type operator()(const typename Operation::first_argument_type& x) const {
		return op(x, value);//将value绑定为op的第二个参数
	}
	//count_if(iv.begin(),iv.end(),bind2nd(less<int>(),12)); //搭配使用例子
	//bind2nd(...)会产生一个binder2nd<Operation>(...)对象并传给count_if作为pred参数0
};
//辅助函数
template<class Operation, class T>
inline y_binder2nd<Operation> y_bind2nd(const Operation& op, const T& x) {
	typedef typename Operation::second_argument_type arg2_type;
	return y_binder2nd<Operation>(op, arg2_type(x));
}


template<class Predicate>//缩写为pred，代表会返回bool值的表达式
class y_unary_negate :public unary_function<typename Predicate::argument_type, bool> {
	//表示某个Adaptable Predicate的逻辑负值（一元操作
protected:
	Predicate pred;
public:
	explicit y_unary_negate(const Predicate& x) :pred(x) {}
	bool operator()(const typename Predicate::argument_type& x) const {
		return !pred(x);//将pred运算结果取否定
	}
};
//辅助函数，方便使用上述class template
template<class Predicate>
inline y_unary_negate<Predicate> y_not1(const Predicate& pred) {
	return y_unary_negate<Predicate>(pred);
}
template<class Predicate>//缩写为pred，代表会返回bool值的表达式
class y_binary_negate :public binary_function<typename Predicate::first_argument_type, typename Predicate::second_argument_type, bool> {
	//表示某个Adaptable Binary Predicate的逻辑负值（二元操作
protected:
	Predicate pred;
public:
	explicit y_binary_negate(const Predicate& x) :pred(x) {}
	bool operator()(const typename Predicate::first_argument_type& x, const typename Predicate::second_argument_type& y) const {
		return !pred(x, y);//将pred运算结果取否定
	}
};
//辅助函数，方便使用上述class template
template<class Predicate>
inline y_binary_negate<Predicate> y_not2(const Predicate& pred) {
	return y_binary_negate<Predicate>(pred);
}


//已知两个Adaptable Unary Functions f(),g() 以下适配器用来产生一个h()
//h(x) = f(g(x))
template<class Operation1, class Operation2>
class y_unary_compose : public unary_function<typename Operation2::argument_type, typename Operation1::result_type> {
protected:
	Operation1 op1;
	Operation2 op2;
public:
	y_unary_compose(const Operation1& x, const Operation2& y) :op1(x), op2(y) {}
	typename Operation1::result_type operator()(const typename Operation2::argument_type& x) const {
		return op1(op2(x));
	}
};
//辅助函数，方便使用组合函数
template<class Operation1, class Operation2>
inline y_unary_compose<Operation1, Operation2> y_compose1(const Operation1& op1, const Operation2& op2) {
	return y_unary_compose<Operation1, Operation2>(op1, op2);
}
//已知一个二元函数f和两个一元函数g1，g2
//以下适配器产生一个h函数，h(x) = f(g1(x),g2(x))
template<class Operation1, class Operation2, class Operation3>
class y_binary_compose : public unary_function<typename Operation2::argument_type, typename Operation1::result_type> {
protected:
	Operation1 op1;
	Operation2 op2;
	Operation3 op3;
public:
	y_binary_compose(const Operation1& x, const Operation2& y, const Operation3& z) :op1(x), op2(y), op3(x) {}
	typename Operation1::result_type operator()(const typename Operation2::argument_type& x) const {
		return op1(op2(x), op3(x));
	}
};
//辅助函数，方便使用二元组合函数
template<class Operation1, class Operation2, class Operation3>
inline y_binary_compose<Operation1, Operation2, Operation3> y_compose2(const Operation1& op1, const Operation2& op2, const Operation3& op3) {
	return y_binary_compose<Operation1, Operation2, Operation3>(op1, op2, op3);
}


//用于函数指针ptr_func，能够让我们将一般函数作为仿函数使用
template<class Arg, class Result>
class y_pointer_to_unary_function : public unary_function<Arg, Result> {
	//将一元函数指针封装，仿函数被调用时，就调用该函数指针
protected:
	Result(*ptr)(Arg);//内部成员，一个函数指针
public:
	y_pointer_to_unary_function() {}
	explicit y_pointer_to_unary_function(Result(*x)(Arg)) :ptr(x) {}
	Result operator()(Arg x) const { return ptr(x); }
};
//辅助函数
template<class Arg, class Result>
inline y_pointer_to_unary_function<Arg, Result> y_ptr_fun(Result(*x)(Arg)) {
	return y_pointer_to_unary_function<Arg, Result>(x);
}
//对于二元函数
template<class Arg1, class Arg2, class Result>
class y_pointer_to_binary_function :public binary_function<Arg1, Arg2, Result> {
protected:
	Result(*ptr)(Arg1, Arg2);
public:
	y_pointer_to_binary_function() {}
	explicit y_pointer_to_binary_function(Result(*x)(Arg1, Arg2)) :ptr(x) {}
	Result operator()(Arg1 x, Arg2 y) const { return ptr(x, y); }
};
//辅助函数
template<class Arg1, class Arg2, class Result>
inline y_pointer_to_binary_function<Arg1, Arg2, Result> y_ptr_fun(Result(*x)(Arg1, Arg2)) {
	return y_pointer_to_binary_function<Arg1, Arg2, Result>(x);
}


//用于成员函数指针，mem_fun,mem_fun_ref，能够让我们将成员函数作为仿函数使用
//还可以将泛型与多态接轨，使用虚函数作为仿函数
//示例
class Shape {
public:
	virtual void display() = 0;
};
class Rect :public Shape {
public:
	virtual void display() { cout << "Rect "; }
};
class Circle :public Shape {
public:
	virtual void display() { cout << "Circle "; }
};
void test_mem_fun() {
	vector<Shape*> vec_shape;
	vec_shape.push_back(new Rect);
	vec_shape.push_back(new Circle);
	//普通遍历输出
	for (int i = 0; i < vec_shape.size(); ++i)
		vec_shape[i]->display();
	cout << endl;
	//使用适配器
	for_each(vec_shape.begin(), vec_shape.end(), mem_fun(&Shape::display));//将成员函数地址传递
	cout << endl;
}
//成员函数参数是有复杂性的，很多情况：
//无参和有一个参数、通过指针调用和通过引用调用、const成员函数和non-const成员函数
//三种情况可组合8种情况
template<class S, class T>
class y_mem_fun_t :public unary_function<T*, S> {//无参、指针调用、non-const
private:
	S(T::* f)();//记录成员函数指针
public:
	explicit y_mem_fun_t(S(T::* pf)()) :f(pf) {}//记录成员函数
	S operator()(T* p) const { return (p->*f)(); }//转调用
};
template<class S, class T>
class y_const_mem_fun_t :public unary_function<const T*, S> {//无参、指针调用、const
private:
	S(T::* f)() const;//记录成员函数指针
public:
	explicit y_const_mem_fun_t(S(T::* pf)() const) :f(pf) {}//记录成员函数
	S operator()(const T* p) const { return (p->*f)(); }//转调用
};
template<class S, class T>
class y_mem_fun_ref_t :public unary_function<T, S> {//无参、引用调用、non-const
private:
	S(T::* f)();
public:
	explicit y_mem_fun_ref_t(S(T::* pf)()) :f(pf) {}
	S operator()(T& r) const { return (r.*f)(); }
};
template<class S, class T>
class y_const_mem_fun_ref_t :public unary_function<T, S> {//无参、引用调用、const
private:
	S(T::* f)() const;
public:
	explicit y_const_mem_fun_ref_t(S(T::* pf)() const) :f(pf) {}
	S operator()(const T& r) const { return (r.*f)(); }
};
template<class S, class T, class A>
class y_mem_fun1_t :public binary_function<T*, A, S> {//一参、指针调用、non-const
private:
	S(T::* f)(A);
public:
	explicit y_mem_fun1_t(S(T::* pf)(A)) :f(pf) {}
	S operator()(T* p, A x) const { return (p->*f)(x); }
};
template<class S, class T, class A>
class y_const_mem_fun1_t :public binary_function<const T*, A, S> {//一参、指针调用、const
private:
	S(T::* f)(A) const;
public:
	explicit y_const_mem_fun1_t(S(T::* pf)(A) const) :f(pf) {}
	S operator()(T* p, A x) const { return (p->*f)(x); }
};
template<class S, class T, class A>
class y_mem_fun1_ref_t :public binary_function<T, A, S> {//一参、引用调用、non-const
private:
	S(T::* f)(A);
public:
	explicit y_mem_fun1_ref_t(S(T::* pf)(A)) :f(pf) {}
	S operator()(T& r, A x) const { return (r.*f)(x); }
};
template<class S, class T, class A>
class y_const_mem_fun1_ref_t :public binary_function<T, A, S> {//一参、引用调用、const
private:
	S(T::* f)(A) const;
public:
	explicit y_const_mem_fun1_ref_t(S(T::* pf)(A) const) :f(pf) {}
	S operator()(T& r, A x) const { return (r.*f)(x); }
};
//辅助函数，按规则使用上述class template
template<class S, class T>
inline y_mem_fun_t<S, T> y_mem_fun(S(T::* f)()) {
	return y_mem_fun_t<S, T>(f);
}
template<class S, class T>
inline y_const_mem_fun_t<S, T> y_mem_fun(S(T::* f)() const) {
	return y_const_mem_fun_t<S, T>(f);
}
template<class S, class T>
inline y_mem_fun_ref_t<S, T> y_mem_fun_ref(S(T::* f)()) {
	return y_mem_fun_ref_t<S, T>(f);
}
template<class S, class T>
inline y_const_mem_fun_ref_t<S, T> y_mem_fun_ref(S(T::* f)() const) {
	return y_const_mem_fun_ref_t<S, T>(f);
}
template<class S, class T, class A>
inline y_mem_fun1_t<S, T, A> y_mem_fun1(S(T::* f)(A)) {
	return y_mem_fun1_t<S, T, A>(f);
}
template<class S, class T, class A>
inline y_const_mem_fun1_t<S, T, A> y_mem_fun1(S(T::* f)(A) const) {
	return y_const_mem_fun1_t<S, T, A>(f);
}
template<class S, class T, class A>
inline y_mem_fun1_ref_t<S, T, A> y_mem_fun1_ref(S(T::* f)(A)) {
	return y_mem_fun1_ref_t<S, T, A>(f);
}
template<class S, class T, class A>
inline y_const_mem_fun1_ref_t<S, T, A> y_mem_fun1_ref(S(T::* f)(A) const) {
	return y_const_mem_fun1_ref_t<S, T, A>(f);
}