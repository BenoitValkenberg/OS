/*
 * proc_repeater.c -- A sequence_p generator and repeater
 * ==================================================
 * Valkenberg Benoit, Boileau Quentin
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#include <linux/proc_seqgen.h>

#define MAX_SIZE 1000 // Limit the size of the sequence_p


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Sequence Generator Repeater with proc entry");
MODULE_AUTHOR("Quentin Boileau , Benoit Valkenberg");


void freeSequenceProc(void);
ssize_t read_proc(char *buf,char **start,off_t offset,int count,int *eof,void *data );
int write_proc(struct file *file,const char *buf,unsigned long count,void *data );

Sequence_proc sequence_p;

static struct proc_dir_entry *proc_write_entry;


int proc_init (void) {
    proc_write_entry = create_proc_entry("seqgen",0666,NULL);
	if(!proc_write_entry){
		printk(KERN_INFO "Error creating proc entry");
		return -1;
	}
	proc_write_entry->read_proc = read_proc ;
	proc_write_entry->write_proc = write_proc;
	printk("proc initialized");
    return 0;
}

void proc_cleanup(void) {
    printk("Cleanup_module\n");
    remove_proc_entry("seqgen",NULL);
}

ssize_t read_proc(char *buf,char **start,off_t offset,int count,int *eof,void *data ){
	int len = 0;
	if(sequence_p == NULL) {
		printk(KERN_ERR "[PROC_SEQGEN] Error : Sequence unset\n");
		return -2;
	}
	
	if(count < 0) {
		printk(KERN_ERR "[PROC_SEQGEN] Error : Size argument not valid\n");
		return -3;
	}
	
	
	len = sprintf(buf, "%s" , sequence_p->seq);
	
	return len;
}

int write_proc(struct file *file,const char *buf,unsigned long count,void *data ){
	if(buf <= 0 || count <= 0 || count > MAX_SIZE){
		printk(KERN_ERR "[PROC_SEQGEN] Error : Wrong arguments\n");
		return -2;
	}
	
	if(count > strlen(buf)) {
		printk(KERN_ERR "[PROC_SEQGEN] Error : Given size is bigger than sequence_p size\n");
		return -3;
	}
	
	if(sequence_p != NULL){
		freeSequenceProc();
	}
	
	sequence_p = kmalloc(sizeof(Sequence_proc) , GFP_KERNEL);
	if(sequence_p == NULL){
		printk(KERN_ERR "[PROC_SEQGEN] Error : Allocation error\n");
		return -4;
	}
	
	sequence_p->size = count;
	sequence_p->seq = kmalloc(count + 1, GFP_KERNEL);
	sequence_p->seq[count] = '\0';
	
	if(sequence_p->seq == NULL){
		printk(KERN_ERR "[PROC_SEQGEN] Error : Allocation error\n");
		return -4;
	}

	copy_from_user(sequence_p->seq, buf, count);
	printk("[PROC_SEQGEN] Sequence of size %lu is set to %s\n", count,  sequence_p->seq);
	
	return count;
}

void freeSequenceProc(void) {

	if(sequence_p !=NULL) {
		if(sequence_p->seq != NULL)
			kfree(sequence_p->seq);
		kfree(sequence_p);
	}	
}

module_init(proc_init);
module_exit(proc_cleanup);

