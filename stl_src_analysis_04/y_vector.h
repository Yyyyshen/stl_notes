#pragma once

/**
 * vector�ؼ���
 */

class y_alloc { /* Ĭ�Ͽռ������� */ };
template<class T, class Alloc>
class y_simple_alloc { /* ���������Ԫ�ش�СΪ���õ�λ */ };
template<class T, class Alloc = y_alloc>
class y_vector {
public:
	//��Ƕ�ͱ���
	typedef T			value_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef value_type* iterator;	//vector֧�������ȡ����ָͨ�������������磬vector<int>::iterator iter;��iter����int*
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;

protected:
	iterator start;				//��ǰʹ�ÿռ��ͷ
	iterator finish;			//��ǰʹ�ÿռ��β
	iterator end_of_storage;	//��ǰ���ÿռ��β��Ҳ����ʵ�����ô�С���ܱ�ʹ�������������Ա�����

public:
	//��������������ʵ����β��ʶ����С���������пա�[]�������ǰ���Ԫ�ص�
	iterator begin() { return start; }
	iterator end() { return finish; }
	size_type size() const { return size_type(end() - begin()); }
	size_type capacity() const { return size_type(end_of_storage - begin()); }
	bool empty() const { return begin() == end(); }
	reference operator[](size_type n) { return *(begin() + n); }
	reference front() { return *begin(); }
	reference back() { return*(end() - 1); }
	//�ռ�������
	typedef y_simple_alloc<value_type, Alloc> data_allocator;
	//���ֹ���
	y_vector() : start(0), finish(0), end_of_storage(0) {} //��vec
	y_vector(size_type n, const T& value) { fill_initialize(n, value); } //���һ��������ĳֵ
	explicit y_vector(size_type n) { fill_initialize(n, T()); }
	~y_vector() { destroy(start, finish); deallocate(); }

protected:
	void fill_initialize(size_type n, const T& value) {
		//����������Գ�ʼ��
		start = allocate_and_fill(n, value);
		finish = start + n;
		end_of_storage = finish;
	}
	iterator allocate_and_fill(size_type n, const T& x) {
		iterator result = data_allocator::allocate(n);
		uninitialized_fill_n(result, n, x); //�������ʵ�ֵڶ������˽�
		return result;
	}
	void deallocate() {
		if (start) data_allocator::deallocate(start, end_of_storage - start);
	}
	void insert_aux(iterator position, const T& x) {
		if (finish != end_of_storage) {
			//ȷ���Ƿ��б��ÿռ䣬�����ٱ��ÿռ���ʼλ�ù���һ��Ԫ�أ����Ե�ǰ���һ��Ԫ��Ϊ��ֵ
			construct(finish, *(finish - 1));
			++finish;
			//����ĩβλ�ú󿽱�����
			T x_copy = x;
			copy_backward(position, finish - 2, finish - 1);
			*position = x_copy;
		}
		else {
			//�ޱ��ÿռ�
			const size_type old_size = size();
			const size_type len = old_size != 0 ? 2 * old_size : 1;//�����һ��ʼΪ�չ��죬�ȶ�Ϊ1������ɱ�����
			iterator new_start = data_allocator::allocate(len);//���·���ռ�
			iterator new_finish = new_start;
			try
			{
				//����ԭ�����ݵ��¿ռ�
				new_finish = uninitialized_copy(start, position, new_start);//�ڶ����˽��
				construct(new_finish, x);//�ڽ�β�����Ԫ��
				++new_finish;//����ָ��
				new_finish = uninitialized_copy(position, finish, new_finish);//��ԭvector���ÿռ�����Ҳ��������
			}
			catch (...)
			{
				//���������쳣���ͷſռ�
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, len);
				throw;
			}
			//�������ͷ�ԭvector�ռ�
			destroy(begin(), end());
			deallocate();
			//������������ԱΪ�µ�ֵ
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + len;
			//�ɴ˿ɼ��������vector�������������·���ռ䣬ָ��ԭvector�ĵ������ͻ�ʧЧ�ˣ�Ҫע��
		}
	}

public:
	//β������Ԫ��
	void push_back(const T& x) {
		//���ж��Ƿ��б��ÿռ�
		if (finish != end_of_storage)
		{
			//���пռ�
			construct(finish, x);//��stl_src_analysis_02�е�ȫ�ֺ���
			++finish;
		}
		else //�ޱ��ÿռ�
			insert_aux(end(), x);
	}
	//β��ɾ��Ԫ��
	void pop_back() {
		--finish;//��ǰ�ƣ�finish�ű�ʾ���һ��Ԫ��
		destroy(finish);
	}
	//�����Χ��Ԫ�ز��ƶ�λ��
	iterator erase(iterator first, iterator last) {
		iterator i = copy(last, finish, first);//������Χ��Ԫ�ص�Ҫɾ���Ŀ��ˣ����ؿ��������һ��Ԫ�ص�ָ��λ��
		destroy(i, finish);//ɾ�����������һ��Ԫ��λ�õ���β��Ԫ��
		//����������
		finish = finish - (last - first);
		return first;
	}
	//ɾ��ĳ��λ��Ԫ��
	iterator erase(iterator position) {
		if (position + 1 != end())
			copy(position + 1, finish, position);//�൱�ڽ�ָ��λ�ú�Ԫ������ǰ�ƣ����ɾ���ľ������һ��Ԫ�أ��൱��pop_back();
		//������Χ
		--finish;
		destroy(finish);
		return position;
	}
	//�������
	void clear() { erase(begin(), end()); }
	//��position��ʼ������n��Ԫ�أ���ֵΪx
	void insert(iterator position, size_type n, const T& x) {
		if (n != 0) //��0�Ļ�û����
		{
			if (size_type(end_of_storage - finish) >= n) //���ռ��Ƿ���
			{
				T x_copy = x;
				const size_type elems_after = finish - position;//��������֮���Ԫ�ظ���
				iterator old_finish = finish;//��¼��ǰ��β
				if (elems_after > n)
				{
					//����Ԫ�ش�����������
					uninitialized_copy(finish - n, finish, finish);
					finish += n;
					copy_backward(position, old_finish - n, old_finish);
					fill(position, position + n, x_copy);//�Ӳ���������Ԫ��
				}
				else
				{
					//����Ԫ�ظ���С�ڵ�����������
					uninitialized_fill_n(finish, n - elems_after, x_copy);
					finish += n - elems_after;
					uninitialized_copy(position, old_finish, finish);
					finish += elems_after;
					fill(position, old_finish, x_copy);
				}

			}
			else //�ռ䲻��
			{
				//ȷ���³���
				const size_type old_size = size();
				const size_type len = old_size + max(old_size, n); //��Ӹ���С�����д�С�����ǳɱ����ݣ���������Ҫ��ӵĴ�С
				//�����¿ռ䣬ͬpush_back�еĴ���
				iterator new_start = data_allocator::allocate(len);
				iterator new_finish = new_start;
				try
				{
					//����ԭ�����ݵ��¿ռ�
					new_finish = uninitialized_copy(start, position, new_start);//�ڶ����˽��
					construct(new_finish, x);//�ڽ�β�����Ԫ��
					++new_finish;//����ָ��
					new_finish = uninitialized_copy(position, finish, new_finish);//��ԭvector���ÿռ�����Ҳ��������
				}
				catch (...)
				{
					//���������쳣���ͷſռ�
					destroy(new_start, new_finish);
					data_allocator::deallocate(new_start, len);
					throw;
				}
				//�������ͷ�ԭvector�ռ�
				destroy(begin(), end());
				deallocate();
				//������������ԱΪ�µ�ֵ
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + len;
			}
		}
	}
};