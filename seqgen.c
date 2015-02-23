#include <linux/linkage.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

asmlinkage long sys_seqgen(unsigned char* ptr, size_t size){
	
	if(ptr == NULL || size < 0){
		printk(KERN_ALERT "SEQGEN Error : Wrong arguments\n");
		return -2;
	}
	
	char* seq = kmalloc(size, GFP_KERNEL);
	
	if(seq == NULL){
		printk(KERN_ALERT "SEQGEN Error : Allocation error\n");
		return -3;
	}
			
	copy_from_user(seq, ptr, strlen_user(ptr));
	
	printk("mysyscall: %s\n", seq);
	
	
	return 0;
}