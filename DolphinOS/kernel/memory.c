/*(C) Olaf 25/8/2018 memory.c
 *init the memory, design the physical & virtual memory bitmap
 *design a function which is used to convert virtual memory address to phsical memory address
 *prepare for Operating System paging
 *prepare for malloc function
 */

#include "../com/types.h"
#include "memory.h"
#include "printk.h"
#include "bitmap.h"

Pool kernel_pool, user_pool;
Virtual_Addr_BitMap kernel_vaddr;

/*I am not familiar the memory management of linux, So I manage the memory by myself.
 *It is very important file in kernel
 *You can see in DolphinOS, 0~12mb of memory is not avaiable for user.
 *Why? because 0~1mb of memory is occupied by kernel and 1~5mb of memory is occupied by paging & paging directoy.
 *5~8MB are free memory space. I will make kernel program to use this memory(5~8MB). 8~12mb is vram.
 *memory_total_size means that the memory of machine.
 *unit of USED_MEMORY_SIZE is 'bytes'. 12x1024x1024 bytes=12mb.
 */

/*我并不熟悉Linux系统的内存管理，所以这套内存管理是自己创建的
 *对于系统内核来说这是一个非常重要的文件
 *对于用户空间来说，0~12mb是无法使用的
 *因为0~1mb被内核占领，1~5mb被页表和页目录表占领
 *5~8mb是可以使用的，但是未来我会添加一些内核程序在这段内存(5~8MB)，8~12mb是vram。
 *memory_total_size是您的机器内存的总大小
 *USED_MEMORY_SIZE的单位是bytes，12x1024x1024 bytes=12mb
 */

void init_memory(){
	
	refresh();
	
	uint32_t memory_total_size;
	memory_total_size=get_ards_infor();
	uint32_t total_pages=memory_total_size/PAGE_SIZE;
	uint32_t used_memory_size=USED_MEMORY_SIZE;
	uint32_t used_phy_pages=used_memory_size/PAGE_SIZE;
	uint32_t free_pages=total_pages-used_phy_pages;
	uint32_t kernel_pages=free_pages/2;
	uint32_t kbm_length=kernel_pages/8;
	uint32_t kp_start_address=USED_MEMORY_SIZE;
	uint32_t ubm_length=(free_pages-kernel_pages)/8;
	
	kernel_pool.phy_addr_start=kp_start_address;
	kernel_pool.pool_bitmap.bits = (void*)MEM_BITMAP_BASE;
	printk("kernel start:");
	puts_int32(kernel_pool.pool_bitmap.bits);
	kernel_pool.pool_size = kernel_pages * PAGE_SIZE;
	
	kernel_pool.pool_bitmap.bm_total_len = kbm_length;
	user_pool.pool_bitmap.bm_total_len = ubm_length;
	
	user_pool.pool_bitmap.bits=(void*)(kernel_pool.pool_bitmap.bits+kbm_length);
	printk("  user start:");
	puts_int32(user_pool.pool_bitmap.bits);
	
	init_bitmap(&kernel_pool.pool_bitmap);
	
	/* this is bitmap of kernel virtual address, the array up to real machine physical address.
	 * 下面初始化内核虚拟地址的位图,按实际物理内存大小生成数组。
	 */
	kernel_vaddr.vaddr_bitmap.bm_total_len = kbm_length;      // 用于维护内核堆的虚拟地址,所以要和内核内存池大小一致
	
	/* 位图的数组指向一块未使用的内存,目前定位在内核内存池和用户内存池之外*/
	kernel_vaddr.vaddr_bitmap.bits = (void*)(kernel_pool.pool_bitmap.bits + kbm_length + ubm_length);
	//printk("qqq");
	kernel_vaddr.vaddr_start=K_HEAP_START;
	//uint32_t a= K_HEAP_START;
	//puts_int32(a);
	//printk("111");
	//uint32_t *p=(void*)(0x80501000);
	//kernel_vaddr.vaddr_bitmap.bits[0]=0xab;
	//*p=0xabdef012;
	//puts_int32(kernel_vaddr.vaddr_bitmap.bits);
	
	printk("\nmemory init......\n");
	
	//BitMap* bm = &kernel_vaddr.vaddr_bitmap;
	//bm->bits[0]=0xff;		
	
	enum pool_flags pk=PF_KERNEL;
	malloc_page(pk,3);
	malloc_page(pk,1);
}

/*Allocated pg_cnt pages, if success, return start of virtual address, or retuen NULL 
 *分配pg_cnt个页空间,成功则返回起始虚拟地址,失败时返回NULL */

void* malloc_page(enum pool_flags pf, uint32_t pg_cnt) {
/***********   malloc_page include three steps ************
	  1. Using vaddr_get() to apply virtual address in virtual memory pool
	  2. Using palloc() to apply real physical address in physical pool
	  3. Using page_table_add() to created a mapping between virtual address and real physical address.
***************************************************************/

/***********   malloc_page的原理是三个动作的合成:   ************
      1通过vaddr_get在虚拟内存池中申请虚拟地址
      2通过palloc在物理内存池中申请物理页
      3通过page_table_add将以上得到的虚拟地址和物理地址在页表中完成映射
***************************************************************/

   void* vaddr_start = vaddr_get(pf, pg_cnt);
   
  // printk("  :");
  // puts_int32(vaddr_start);
   
   if (vaddr_start == NULL) {
	  //  printk(" ?? ");
      return NULL;
   }

   uint32_t vaddr = (uint32_t)vaddr_start;
   printk("\nkernel_pool_adder_vir:");
   puts_int32(vaddr );
   uint32_t cnt = pg_cnt;
   
  // Pool* mem_pool = pf & PF_KERNEL ? &kernel_pool : &user_pool;
   
   Pool* mem_pool = &kernel_pool;
   /* 因为虚拟地址是连续的,但物理地址可以是不连续的,所以逐个做映射*/
   while (cnt-- > 0) {
      void* page_phyaddr = palloc(mem_pool);
	  printk("\nkernel_pool_adder_phy:");
	  puts_int32(page_phyaddr);
      if (page_phyaddr == NULL) {  // 失败时要将曾经已申请的虚拟地址和物理页全部回滚，在将来完成内存回收时再补充
		return NULL;
      }
     // page_table_add((void*)vaddr, page_phyaddr); // 在页表中做映射 
     // vaddr += PAGE_SIZE;		 // 下一个虚拟页
   }
   return vaddr_start;
}


/* 在pf表示的虚拟内存池中申请pg_cnt个虚拟页,
 * 成功则返回虚拟页的起始地址, 失败则返回NULL */
static void* vaddr_get(enum pool_flags pf, uint32_t pg_cnt) {
   int vaddr_start = 0, bit_idx_start = -1;
   uint32_t cnt = 0;
   if (pf == PF_KERNEL) {
      bit_idx_start  = bitmap_scan(&kernel_vaddr.vaddr_bitmap, pg_cnt);
      if (bit_idx_start == -1) {
	 return NULL;
      }
      while(cnt < pg_cnt) {
	 bitmap_set(&kernel_vaddr.vaddr_bitmap, bit_idx_start + cnt++, 1);
      }
      vaddr_start = kernel_vaddr.vaddr_start + bit_idx_start * PAGE_SIZE;
   } else {	
   // 用户内存池,将来实现用户进程再补充
   }
   
   // printk(" $ ");
   return (void*)vaddr_start;
}

/* 在m_pool指向的物理内存池中分配1个物理页,
 * 成功则返回页框的物理地址,失败则返回NULL */
static void* palloc(Pool* m_pool) {
   /* 扫描或设置位图要保证原子操作 */
   int bit_idx = bitmap_scan(&m_pool->pool_bitmap, 1);    // 找一个物理页面
   if (bit_idx == -1 ) {
      return NULL;
   }
   bitmap_set(&m_pool->pool_bitmap, bit_idx, 1);	// 将此位bit_idx置1
   uint32_t page_phyaddr = ((bit_idx * PAGE_SIZE) + m_pool->phy_addr_start);
   return (void*)page_phyaddr;
}

//刷新4kb内存，因为不刷新内存就会出现乱码，未来bitmap的地址将会加载到这里
void refresh(){
	/*char* rf=MEM_BITMAP_BASE;
	for(uint32_t i=0;i<4673;i++){
		rf[i]=0x00;
	}*/
	//char *a = (char *)0x400000;
	//char *b = (char *)0x8ffffe;
	
	//*a = 0xaa;
	//*b = 0xff;
	
	
	char* rf=MEM_BITMAP_BASE+4674;
	//printk("ERROR:");
	//puts_int32(MEM_BITMAP_BASE+4674);
	rf[0]=0x00;
	
	char* rf2=MEM_BITMAP_BASE+4675;
	for(uint32_t j=0;j<500;j++){
		rf2[j]=0x00;
	}
}
