#pragma once
/**
 * SGI �ڶ���������
 *
 * ���鹻�󣬳���128bytesʱ���ƽ���һ������������С��128bytesʱ�����ڴ�ع���
 * ÿ������һ����ڴ棬��ά����Ӧ��free-list���´�������ͬ��С�ڴ�����ֱ�Ӵ�free-list�аγ�
 * �ͻ����ͷ�С�����飬�������������ܵ�free-list��
 * �ڶ������������������κ�С�������ڴ��������ϵ���8��������
 * 16��free-list���Թ����СΪ8��16��24��32��40��48��56��64��71��80��88��96��104��112��120��128bytes��С������
 *
 * ���ļ�Ϊ�ڶ�������������ʵ������
 */

namespace yshen {

	enum { __ALIGN = 8 };//С�������ϵ��߽�
	enum { __MAX_BYTES = 128 };//С�Ϳ�����
	enum { __NFREELISTS = __MAX_BYTES / __ALIGN };//free-lists����

	//
	template <bool threads, int inst>
	class __default_alloc_template {
	private:
		//��bytes�ϵ���8��������
		static size_t ROUND_UP(size_t bytes) {
			return ((bytes)+__ALIGN - 1) & ~(__ALIGN - 1);
		}
	private:
		//free-list�ڵ�ṹ
		//Ϊ��ά������ÿ���ڵ���Ҫ����ָ����ʵҲ��һ�ָ��������Ѿ�Ŭ���Ľ�ʡ�ڴ濪��
		//����ʹ��union����һ���ֶο���obj����Ϊָ�룬ָ����һ��obj
		//�ڶ����ֶο���obj����Ϊָ��ʵ�������ָ��
		//union��struct���𣬲ο���https://blog.csdn.net/firefly_2002/article/details/7954458
		union obj {
			union obj* free_list_link;
			char client_data[1];
		};
	private:
		//volatile���ѱ�����������������ı�����ʱ���п��ܸı�
		//��˱����ĳ���ÿ����Ҫ�洢���ȡ���������ʱ�򣬶���ֱ�Ӵӱ�����ַ�ж�ȡ���ݡ�
		//���û��volatile�ؼ��֣�������������Ż���ȡ�ʹ洢��������ʱʹ�üĴ����е�ֵ�������������ɱ�ĳ�������˵Ļ��������ֲ�һ�µ�����
		static obj* volatile free_list[__NFREELISTS]; //16��free-list
		//���������С�������õ�n��free-list��n��1����
		static size_t FREELIST_INDEX(size_t bytes) {
			return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
		}
		//����һ����СΪn�Ķ��󣬲����ܼ����СΪn���������鵽free-list
		static void* refill(size_t n);
		//����һ���ռ䣬������nobjs����СΪsize������
		static char* chunk_alloc(size_t size, int& nobjs);
		//chunk allocation state
		static char* start_free;//�ڴ����ʼλ��
		static char* end_free;//ֻ��chunk_alloc()�б仯
		static size_t heap_size;

	public:
		//����
		static void* allocate(size_t n) {
			obj* volatile* my_free_list;
			obj* result;
			//����128���õ�һ��������
			if (n > (size_t)__MAX_BYTES)
			{
				return (malloc_alloc::allocate(n));
			}
			//��16��free-list��Ѱ���ʵ���һ��
			my_free_list = free_list + FREELIST_INDEX(n);
			result = *my_free_list;
			if (result == 0)
			{
				//û�ҵ�����free-list���������
				void* r = refill(ROUND_UP(n));
				return r;
			}
			//����free-list
			*my_free_list = result->free_list_link;
			return (result);
		}
		//�ͷ�
		static void deallocate(void* p, size_t n) {
			obj* q = (obj*)p;
			obj* volatile* my_free_list;
			//ͬ���ģ�����128ʹ�õ�һ��������
			if (n > (size_t)__MAX_BYTES)
			{
				malloc_alloc::deallocate(p, n);
			}
			//Ѱ�Ҷ�Ӧfree-list
			my_free_list = free_list + FREELIST_INDEX(n);
			//����������������
			q->free_list_link = *my_free_list;
			*my_free_list = q;
		}
		static void* reallocate(void* p, size_t old_sz, size_t new_sz) {};
	};

	//��ʼֵ����
	template<bool threads, int inst>
	char* __default_alloc_template<threads, inst>::start_free = 0;
	template<bool threads, int inst>
	char* __default_alloc_template<threads, inst>::end_free = 0;
	template<bool threads, int inst>
	size_t __default_alloc_template<threads, inst>::heap_size = 0;

	//template<bool threads, int inst>
	//__default_alloc_template<threads, inst>::obj* volatile
	//	__default_alloc_template<threads, inst>::free_list[__NFREELISTS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	//��free-listû�п������飬����refill���µĿռ�ȡ���ڴ�أ�chunk_alloc��ȱʡȡ��20���½ڵ㣬���ռ䲻�㣬���ܻ��Զ�����
	//����һ����СΪn�Ķ�����ʱ��Ϊ�ʵ�free-list���ӽڵ㣬����n�Ѿ��ϵ���8�ı���
	template<bool threads, int inst>
	void* __default_alloc_template<threads, inst>::refill(size_t n)
	{
		int nobjs = 20;
		//����chunk_alloc������ȡ��nobjs��������Ϊ�½ڵ㣬nobjs�����ô�ֵ�����ܸı�
		char* chunk = chunk_alloc(n, nobjs);
		obj* volatile* my_free_list;
		obj* result;
		obj* current_obj, * next_obj;
		int i;
		//���ֻ���һ�����飬����������ߣ����½ڵ�
		if (1 == nobjs) return(chunk);
		//���򣬵���free-list�������½ڵ�
		my_free_list = free_list + FREELIST_INDEX(n);

		//��chunk�ռ��ڽ���free-list
		result = (obj*)chunk;//�·���ĳ�ʼ��ַ����
		//����free-listָ�������õĿռ�
		*my_free_list = next_obj = (obj*)(chunk + n);
		//�����ڵ㴮����
		for (i = 1;; i++)
		{
			current_obj = next_obj;
			next_obj = (obj*)((char*)next_obj + n);
			if (nobjs - 1 == i)
			{
				current_obj->free_list_link = 0;
				break;
			}
			else {
				current_obj->free_list_link = next_obj;
			}
		}
		return(result);
	}

	//���ڴ��ȡ�ռ��free-listʹ��
	template<bool threads, int inst>
	char* __default_alloc_template<threads, inst>::chunk_alloc(size_t size, int& nobjs) {
		char* result;
		size_t total_bytes = size * nobjs;
		size_t bytes_left = end_free - start_free;//�ڴ��ʣ��ռ�
		if (bytes_left >= total_bytes)
		{
			//ʣ��ռ���ȫ����
			result = start_free;
			start_free += total_bytes;
			return(result);
		}
		else if (bytes_left >= size)
		{
			//����ȫ�������󣬵��ܹ���Ӧһ����������
			nobjs = bytes_left / size;
			total_bytes = size * nobjs;//�ж��پͷֶ���
			result = start_free;
			start_free += total_bytes;
			return(result);
		}
		else
		{
			//һ�����鶼�޷��ṩ
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			//�������ڴ���еĲ��໹�����ü�ֵ
			if (bytes_left > 0)
			{
				//��ʣ��ķ�����ʵ�free-list
				obj* volatile* my_free_list = free_list + FREELIST_INDEX(bytes_left);
				((obj*)start_free)->free_list_link = *my_free_list;
				*my_free_list = (obj*)start_free;
			}
			//����heap�ռ䣬�����ڴ��
			start_free = (char*)malloc(bytes_to_get);
			if (0 == start_free)
			{
				//heap�ռ䲻�㣬mallocʧ��
				int i;
				obj* volatile* my_free_list, * p;
				//���Լ��ӵ�еĶ����������㳢�����ý�С���飬��Ϊ���ڶԽ��̻����ϳ�����
				//��Ѱ�ʵ���free-list���ʵ�ָ���ǣ�����δ�����飬�����鹻��
				for (i = size; i <= __MAX_BYTES; i += __ALIGN)
				{
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (0 != p)
					{
						*my_free_list = p->free_list_link;
						start_free = (char*)p;
						end_free = start_free + i;
						//�ݹ�����Լ�������nobjs
						return(chunk_alloc(size, nobjs));
					}
				}
				end_free = 0;//ʵ��û���ڴ�����ˣ����õ�һ������������oom�����Ƿ�����Ч
				start_free = (char*)malloc_alloc::allocate(bytes_to_get);
			}
			//����ɹ�����¼�µĵ�ַ
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return(chunk_alloc(size, nobjs));//�ݹ�����Լ�������nobjs
		}
	}

}//end of namespace yshen