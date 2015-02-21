#include <linux/linkage.h>
#include <asm/uaccess.h>

asmlinkage long sys_seqgen(unsigned char* ptr, size_t size){
	
	if(true)
		return 0;
	else{
		printk("SEQGEN error ...\n");
		return -1;
	}
}