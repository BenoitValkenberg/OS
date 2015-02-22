#include <linux/linkage.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

asmlinkage long sys_seqgen(unsigned char* ptr, size_t size){
	
	if(ptr == NULL || size > 0){
		printk("SEQGEN Wrong arguments\n");
		return -1;
	}
	
	char* seq = kmalloc(size, GFP_KERNEL);
	
	if(seq == NULL){
		printk("SEQGEN Allocation error\n");
		return -1;
	}
			
	copy_from_user(seq, ptr, strlen_user(ptr));
	
	printk("mysyscall: %s\n", seq);
	
	
	return 0;
}