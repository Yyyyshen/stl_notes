#pragma once

/**
 * �򻯰�����ָ��ʵ��
 */

template <class T>
class y_smart_ptr {
public:
	//����������
	explicit y_smart_ptr(T* p = 0) :pointee(p) {}
	template<class U>
	y_smart_ptr(y_smart_ptr<U>& rhs) : pointee(rhs.release()) {};
	~y_smart_ptr() { delete pointee; }
	//���ز�����
	template <class U>
	y_smart_ptr<T>& operator=(y_smart_ptr<U>& rhs) {
		if (this != &rhs) reset(rhs.release());
		return *this;
	}
	T& operator*() const { return *pointee; }
	T* operator->() const { return pointee; }
	T* get() const { return pointee; }

	//...

private:
	T* pointee;
};