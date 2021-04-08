#pragma once

/**
 * function adapters
 *
 * ������class templates��ɣ��㷨��function templates���
 * �º���ʱһ�ֽ�operator()���ص�class template�����������ǽ�operator++��ָ�볣�ò������ص�class template
 * ����������Ӧ���������͵�������class template
 * ���Ӧ���ڷº������ϣ�����function adapters
 *
 * container adapters�ڲ�һ��container member
 * iterator adapter�ڲ�һ��iterator member
 * ÿ��function adapter�����ڲ���һ��member object������Ϊ������ķº�����adaptable functor��
 */


template<class Operation>
class y_binder1st : public unary_function<typename Operation::second_argument_type, typename Operation::result_type> {
protected:
	Operation op;
	typename Operation::first_argument_type value;
public:
	y_binder1st(const Operation& x, const typename Operation::first_argument_type& y) :op(x), value(y) {}//�����ʽ�͵�һ������¼
	typename Operation::result_type operator()(const typename Operation::second_argument_type& x) const {
		return op(value, x);//��value��Ϊ��һ������
	}
};
//��������
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
		return op(x, value);//��value��Ϊop�ĵڶ�������
	}
	//count_if(iv.begin(),iv.end(),bind2nd(less<int>(),12)); //����ʹ������
	//bind2nd(...)�����һ��binder2nd<Operation>(...)���󲢴���count_if��Ϊpred����0
};
//��������
template<class Operation, class T>
inline y_binder2nd<Operation> y_bind2nd(const Operation& op, const T& x) {
	typedef typename Operation::second_argument_type arg2_type;
	return y_binder2nd<Operation>(op, arg2_type(x));
}


template<class Predicate>//��дΪpred������᷵��boolֵ�ı��ʽ
class y_unary_negate :public unary_function<typename Predicate::argument_type, bool> {
	//��ʾĳ��Adaptable Predicate���߼���ֵ��һԪ����
protected:
	Predicate pred;
public:
	explicit y_unary_negate(const Predicate& x) :pred(x) {}
	bool operator()(const typename Predicate::argument_type& x) const {
		return !pred(x);//��pred������ȡ��
	}
};
//��������������ʹ������class template
template<class Predicate>
inline y_unary_negate<Predicate> y_not1(const Predicate& pred) {
	return y_unary_negate<Predicate>(pred);
}
template<class Predicate>//��дΪpred������᷵��boolֵ�ı��ʽ
class y_binary_negate :public binary_function<typename Predicate::first_argument_type, typename Predicate::second_argument_type, bool> {
	//��ʾĳ��Adaptable Binary Predicate���߼���ֵ����Ԫ����
protected:
	Predicate pred;
public:
	explicit y_binary_negate(const Predicate& x) :pred(x) {}
	bool operator()(const typename Predicate::first_argument_type& x, const typename Predicate::second_argument_type& y) const {
		return !pred(x, y);//��pred������ȡ��
	}
};
//��������������ʹ������class template
template<class Predicate>
inline y_binary_negate<Predicate> y_not2(const Predicate& pred) {
	return y_binary_negate<Predicate>(pred);
}


//��֪����Adaptable Unary Functions f(),g() ������������������һ��h()
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
//��������������ʹ����Ϻ���
template<class Operation1, class Operation2>
inline y_unary_compose<Operation1, Operation2> y_compose1(const Operation1& op1, const Operation2& op2) {
	return y_unary_compose<Operation1, Operation2>(op1, op2);
}
//��֪һ����Ԫ����f������һԪ����g1��g2
//��������������һ��h������h(x) = f(g1(x),g2(x))
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
//��������������ʹ�ö�Ԫ��Ϻ���
template<class Operation1, class Operation2, class Operation3>
inline y_binary_compose<Operation1, Operation2, Operation3> y_compose2(const Operation1& op1, const Operation2& op2, const Operation3& op3) {
	return y_binary_compose<Operation1, Operation2, Operation3>(op1, op2, op3);
}


//���ں���ָ��ptr_func���ܹ������ǽ�һ�㺯����Ϊ�º���ʹ��
template<class Arg, class Result>
class y_pointer_to_unary_function : public unary_function<Arg, Result> {
	//��һԪ����ָ���װ���º���������ʱ���͵��øú���ָ��
protected:
	Result(*ptr)(Arg);//�ڲ���Ա��һ������ָ��
public:
	y_pointer_to_unary_function() {}
	explicit y_pointer_to_unary_function(Result(*x)(Arg)) :ptr(x) {}
	Result operator()(Arg x) const { return ptr(x); }
};
//��������
template<class Arg, class Result>
inline y_pointer_to_unary_function<Arg, Result> y_ptr_fun(Result(*x)(Arg)) {
	return y_pointer_to_unary_function<Arg, Result>(x);
}
//���ڶ�Ԫ����
template<class Arg1, class Arg2, class Result>
class y_pointer_to_binary_function :public binary_function<Arg1, Arg2, Result> {
protected:
	Result(*ptr)(Arg1, Arg2);
public:
	y_pointer_to_binary_function() {}
	explicit y_pointer_to_binary_function(Result(*x)(Arg1, Arg2)) :ptr(x) {}
	Result operator()(Arg1 x, Arg2 y) const { return ptr(x, y); }
};
//��������
template<class Arg1, class Arg2, class Result>
inline y_pointer_to_binary_function<Arg1, Arg2, Result> y_ptr_fun(Result(*x)(Arg1, Arg2)) {
	return y_pointer_to_binary_function<Arg1, Arg2, Result>(x);
}


//���ڳ�Ա����ָ�룬mem_fun,mem_fun_ref���ܹ������ǽ���Ա������Ϊ�º���ʹ��
//�����Խ��������̬�ӹ죬ʹ���麯����Ϊ�º���
//ʾ��
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
	//��ͨ�������
	for (int i = 0; i < vec_shape.size(); ++i)
		vec_shape[i]->display();
	cout << endl;
	//ʹ��������
	for_each(vec_shape.begin(), vec_shape.end(), mem_fun(&Shape::display));//����Ա������ַ����
	cout << endl;
}
//��Ա�����������и����Եģ��ܶ������
//�޲κ���һ��������ͨ��ָ����ú�ͨ�����õ��á�const��Ա������non-const��Ա����
//������������8�����
template<class S, class T>
class y_mem_fun_t :public unary_function<T*, S> {//�޲Ρ�ָ����á�non-const
private:
	S(T::* f)();//��¼��Ա����ָ��
public:
	explicit y_mem_fun_t(S(T::* pf)()) :f(pf) {}//��¼��Ա����
	S operator()(T* p) const { return (p->*f)(); }//ת����
};
template<class S, class T>
class y_const_mem_fun_t :public unary_function<const T*, S> {//�޲Ρ�ָ����á�const
private:
	S(T::* f)() const;//��¼��Ա����ָ��
public:
	explicit y_const_mem_fun_t(S(T::* pf)() const) :f(pf) {}//��¼��Ա����
	S operator()(const T* p) const { return (p->*f)(); }//ת����
};
template<class S, class T>
class y_mem_fun_ref_t :public unary_function<T, S> {//�޲Ρ����õ��á�non-const
private:
	S(T::* f)();
public:
	explicit y_mem_fun_ref_t(S(T::* pf)()) :f(pf) {}
	S operator()(T& r) const { return (r.*f)(); }
};
template<class S, class T>
class y_const_mem_fun_ref_t :public unary_function<T, S> {//�޲Ρ����õ��á�const
private:
	S(T::* f)() const;
public:
	explicit y_const_mem_fun_ref_t(S(T::* pf)() const) :f(pf) {}
	S operator()(const T& r) const { return (r.*f)(); }
};
template<class S, class T, class A>
class y_mem_fun1_t :public binary_function<T*, A, S> {//һ�Ρ�ָ����á�non-const
private:
	S(T::* f)(A);
public:
	explicit y_mem_fun1_t(S(T::* pf)(A)) :f(pf) {}
	S operator()(T* p, A x) const { return (p->*f)(x); }
};
template<class S, class T, class A>
class y_const_mem_fun1_t :public binary_function<const T*, A, S> {//һ�Ρ�ָ����á�const
private:
	S(T::* f)(A) const;
public:
	explicit y_const_mem_fun1_t(S(T::* pf)(A) const) :f(pf) {}
	S operator()(T* p, A x) const { return (p->*f)(x); }
};
template<class S, class T, class A>
class y_mem_fun1_ref_t :public binary_function<T, A, S> {//һ�Ρ����õ��á�non-const
private:
	S(T::* f)(A);
public:
	explicit y_mem_fun1_ref_t(S(T::* pf)(A)) :f(pf) {}
	S operator()(T& r, A x) const { return (r.*f)(x); }
};
template<class S, class T, class A>
class y_const_mem_fun1_ref_t :public binary_function<T, A, S> {//һ�Ρ����õ��á�const
private:
	S(T::* f)(A) const;
public:
	explicit y_const_mem_fun1_ref_t(S(T::* pf)(A) const) :f(pf) {}
	S operator()(T& r, A x) const { return (r.*f)(x); }
};
//����������������ʹ������class template
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