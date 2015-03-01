#include <linux/linkage.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/seqgen.h>


Sequence sequence;


asmlinkage long sys_seqgen(unsigned char* ptr, size_t size){
	
	if(ptr == NULL || size <= 0){
		printk(KERN_ALERT "SEQGEN Error : Wrong arguments\n");
		return -2;
	}
	
	if(sequence != NULL){
		freeSequence();
	}
	
	sequence = kmalloc(sizeof(Sequence) , GFP_KERNEL);
	if(sequence == NULL){
		printk(KERN_ALERT "SEQGEN Error : Allocation error\n");
		return -3;
	}
	
	sequence->size = size;
	sequence->seq = kmalloc(size, GFP_KERNEL);
	
	if(sequence->seq == NULL){
		printk(KERN_ALERT "SEQGEN Error : Allocation error\n");
		return -3;
	}
			
	//TODO faut-il vraiment allouer seq du coup ??
	copy_from_user(sequence->seq, ptr, strlen_user(ptr));
	
	printk("mysyscall: %s\n", sequence->seq);
	
	//TODO REMOVE
	//freeSequence();
	
	return 0;
}

/*Sequence getSequence(void){
	
	return sequence;
}*/

void freeSequence(void){
	if(sequence !=NULL){
		if(sequence->seq != NULL)
			kfree(sequence->seq);
		kfree(sequence);
	}
}


