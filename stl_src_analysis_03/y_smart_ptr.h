#pragma once

/**
 * 简化版智能指针实现
 */

template <class T>
class y_smart_ptr {
public:
	//构造与析构
	explicit y_smart_ptr(T* p = 0) :pointee(p) {}
	template<class U>
	y_smart_ptr(y_smart_ptr<U>& rhs) : pointee(rhs.release()) {};
	~y_smart_ptr() { delete pointee; }
	//重载操作符
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