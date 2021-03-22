#pragma once
/**
 * SGI 第二级配置器
 *
 * 区块够大，超过128bytes时，移交第一级配置器处理，小于128bytes时，以内存池管理
 * 每次配置一大块内存，并维护对应的free-list，下次若有相同大小内存需求，直接从free-list中拔出
 * 客户端释放小额区块，就由配置器会受到free-list中
 * 第二级配置器会主动将任何小额区块内存需求量上调至8的整数倍
 * 16个free-list各自管理大小为8、16、24、32、40、48、56、64、71、80、88、96、104、112、120、128bytes的小额区块
 *
 * 本文件为第二级配置器部分实现内容
 */

namespace yshen {

	enum { __ALIGN = 8 };//小型区块上调边界
	enum { __MAX_BYTES = 128 };//小型块上限
	enum { __NFREELISTS = __MAX_BYTES / __ALIGN };//free-lists个数

	//
	template <bool threads, int inst>
	class __default_alloc_template {
	private:
		//将bytes上调至8的整数倍
		static size_t ROUND_UP(size_t bytes) {
			return ((bytes)+__ALIGN - 1) & ~(__ALIGN - 1);
		}
	private:
		//free-list节点结构
		//为了维护链表，每个节点需要额外指针其实也是一种负担，但已经努力的节省内存开销
		//由于使用union，第一个字段看，obj可视为指针，指向另一个obj
		//第二个字段看，obj可视为指向实际区块的指针
		//union和struct区别，参考：https://blog.csdn.net/firefly_2002/article/details/7954458
		union obj {
			union obj* free_list_link;
			char client_data[1];
		};
	private:
		//volatile提醒编译器它后面所定义的变量随时都有可能改变
		//因此编译后的程序每次需要存储或读取这个变量的时候，都会直接从变量地址中读取数据。
		//如果没有volatile关键字，则编译器可能优化读取和存储，可能暂时使用寄存器中的值，如果这个变量由别的程序更新了的话，将出现不一致的现象。
		static obj* volatile free_list[__NFREELISTS]; //16个free-list
		//根据区块大小，决定用第n号free-list，n从1起算
		static size_t FREELIST_INDEX(size_t bytes) {
			return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
		}
		//返回一个大小为n的对象，并可能加入大小为n的其他区块到free-list
		static void* refill(size_t n);
		//配置一大块空间，可容纳nobjs个大小为size的区块
		static char* chunk_alloc(size_t size, int& nobjs);
		//chunk allocation state
		static char* start_free;//内存池起始位置
		static char* end_free;//只在chunk_alloc()中变化
		static size_t heap_size;

	public:
		//分配
		static void* allocate(size_t n) {
			obj* volatile* my_free_list;
			obj* result;
			//大于128调用第一级配置器
			if (n > (size_t)__MAX_BYTES)
			{
				return (malloc_alloc::allocate(n));
			}
			//在16个free-list中寻找适当的一个
			my_free_list = free_list + FREELIST_INDEX(n);
			result = *my_free_list;
			if (result == 0)
			{
				//没找到可用free-list，重新填充
				void* r = refill(ROUND_UP(n));
				return r;
			}
			//调整free-list
			*my_free_list = result->free_list_link;
			return (result);
		}
		//释放
		static void deallocate(void* p, size_t n) {
			obj* q = (obj*)p;
			obj* volatile* my_free_list;
			//同样的，大于128使用第一级配置器
			if (n > (size_t)__MAX_BYTES)
			{
				malloc_alloc::deallocate(p, n);
			}
			//寻找对应free-list
			my_free_list = free_list + FREELIST_INDEX(n);
			//调整链表，回收区块
			q->free_list_link = *my_free_list;
			*my_free_list = q;
		}
		static void* reallocate(void* p, size_t old_sz, size_t new_sz) {};
	};

	//初始值定义
	template<bool threads, int inst>
	char* __default_alloc_template<threads, inst>::start_free = 0;
	template<bool threads, int inst>
	char* __default_alloc_template<threads, inst>::end_free = 0;
	template<bool threads, int inst>
	size_t __default_alloc_template<threads, inst>::heap_size = 0;

	//template<bool threads, int inst>
	//__default_alloc_template<threads, inst>::obj* volatile
	//	__default_alloc_template<threads, inst>::free_list[__NFREELISTS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	//当free-list没有可用区块，调用refill，新的空间取自内存池（chunk_alloc）缺省取得20个新节点，若空间不足，可能会自动调整
	//返回一个大小为n的对象，有时会为适当free-list增加节点，假设n已经上调至8的倍数
	template<bool threads, int inst>
	void* __default_alloc_template<threads, inst>::refill(size_t n)
	{
		int nobjs = 20;
		//调用chunk_alloc，尝试取得nobjs个区块作为新节点，nobjs是引用传值，可能改变
		char* chunk = chunk_alloc(n, nobjs);
		obj* volatile* my_free_list;
		obj* result;
		obj* current_obj, * next_obj;
		int i;
		//如果只获得一个区块，分配给调用者，无新节点
		if (1 == nobjs) return(chunk);
		//否则，调整free-list，纳入新节点
		my_free_list = free_list + FREELIST_INDEX(n);

		//在chunk空间内建立free-list
		result = (obj*)chunk;//新分配的初始地址返回
		//导引free-list指向新配置的空间
		*my_free_list = next_obj = (obj*)(chunk + n);
		//将各节点串起来
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

	//从内存池取空间给free-list使用
	template<bool threads, int inst>
	char* __default_alloc_template<threads, inst>::chunk_alloc(size_t size, int& nobjs) {
		char* result;
		size_t total_bytes = size * nobjs;
		size_t bytes_left = end_free - start_free;//内存池剩余空间
		if (bytes_left >= total_bytes)
		{
			//剩余空间完全够用
			result = start_free;
			start_free += total_bytes;
			return(result);
		}
		else if (bytes_left >= size)
		{
			//不完全满足需求，但能够供应一个以上区块
			nobjs = bytes_left / size;
			total_bytes = size * nobjs;//有多少就分多少
			result = start_free;
			start_free += total_bytes;
			return(result);
		}
		else
		{
			//一个区块都无法提供
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			//试着让内存池中的残余还有利用价值
			if (bytes_left > 0)
			{
				//把剩余的分配给适当free-list
				obj* volatile* my_free_list = free_list + FREELIST_INDEX(bytes_left);
				((obj*)start_free)->free_list_link = *my_free_list;
				*my_free_list = (obj*)start_free;
			}
			//配置heap空间，补充内存池
			start_free = (char*)malloc(bytes_to_get);
			if (0 == start_free)
			{
				//heap空间不足，malloc失败
				int i;
				obj* volatile* my_free_list, * p;
				//尝试检查拥有的东西，不打算尝试配置较小区块，因为会在对进程机器上出问题
				//搜寻适当的free-list（适当指的是，尚有未用区块，且区块够大）
				for (i = size; i <= __MAX_BYTES; i += __ALIGN)
				{
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (0 != p)
					{
						*my_free_list = p->free_list_link;
						start_free = (char*)p;
						end_free = start_free + i;
						//递归调用自己，修正nobjs
						return(chunk_alloc(size, nobjs));
					}
				}
				end_free = 0;//实在没有内存可用了，调用第一级配置器，看oom机制是否能有效
				start_free = (char*)malloc_alloc::allocate(bytes_to_get);
			}
			//分配成功，记录新的地址
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return(chunk_alloc(size, nobjs));//递归调用自己，修正nobjs
		}
	}

}//end of namespace yshen