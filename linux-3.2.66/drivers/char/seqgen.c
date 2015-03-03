/*
 * seqgen.c -- A sequence generator
 * ==================================================
 * Valkenberg Benoit, Boileau Quentin
 *
 */

#include <linux/linkage.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/seqgen.h>
#include <linux/string.h>

#define MAX_SIZE 1000 // Limit the size of the sequence

Sequence sequence;

asmlinkage long sys_seqgen(unsigned char* ptr, size_t size){
	
	if(ptr <= 0 || size <= 0 || size > MAX_SIZE){
		printk(KERN_ERR "[SEQGEN] Error : Wrong arguments\n");
		return -2;
	}
	
	if(size > strlen(ptr)) {
		printk(KERN_ERR "[SEQGEN] Error : Given size is bigger than sequence size\n");
		return -3;
	}
	
	if(sequence != NULL){
		freeSequence();
	}
	
	sequence = kmalloc(sizeof(Sequence) , GFP_KERNEL);
	if(sequence == NULL){
		printk(KERN_ERR "[SEQGEN] Error : Allocation error\n");
		return -4;
	}
	
	sequence->size = size;
	sequence->seq = kmalloc(size + 1, GFP_KERNEL);
	sequence->seq[size] = '\0';
	
	if(sequence->seq == NULL){
		printk(KERN_ERR "[SEQGEN] Error : Allocation error\n");
		return -4;
	}

	copy_from_user(sequence->seq, ptr, size);
	
	printk("[SEQGEN] Sequence of size %zu is set to %s\n", size,  sequence->seq);
	
	return 0;
}

void freeSequence(void) {

	if(sequence !=NULL) {
		if(sequence->seq != NULL)
			kfree(sequence->seq);
		kfree(sequence);
	printk("[SEQGEN] Sequence unset\n");
	}	
}



