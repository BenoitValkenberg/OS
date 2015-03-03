/*
 * seqgen_repeater.c -- A sequence generator repeater
 * ==================================================
 * Valkenberg Benoit, Boileau Quentin
 *
 */

#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/seqgen.h>
#include <linux/ioctl.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/device.h>
#include <linux/slab.h>


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SeqGen Repeater");
MODULE_AUTHOR("Quentin Boileau , Benoit Valkenberg");


static int device_open = 0;
static int* mode; // Repeater mode | 0 => repeate (default) | 1 => single read


static struct class *seqgen_class; // Global variable for the device class

static int seqgen_open(struct inode * , struct file *);
static int seqgen_release(struct inode * , struct file *);
static ssize_t seqgen_read(struct file *, char * , size_t, loff_t *);
static ssize_t seqgen_write(struct file *, const char * , size_t , loff_t *);

static long seqgen_ioctl(struct file *filp,unsigned int cmd,unsigned long arg);


static const struct file_operations seqgen_fops = {
	.owner = THIS_MODULE,
	.read = seqgen_read,
	.open = seqgen_open,
	.write = seqgen_write,
	.release = seqgen_release,
	.unlocked_ioctl = seqgen_ioctl,
};



int init_module(void) {

	int result = register_chrdev(SEQGEN_MAJOR, "seqgen", &seqgen_fops);
	if(result < 0) {
		printk(KERN_ERR "[SEQGEN_REPEATER] Error : seqgen_repeater registration failed\n");
		return result;
	}
	
	seqgen_class = class_create(THIS_MODULE, "seqgen");
	if (IS_ERR(seqgen_class)) {
		unregister_chrdev(SEQGEN_MAJOR, "seqgen");
		return PTR_ERR(seqgen_class);
	}
		
	device_create(seqgen_class, NULL, MKDEV(SEQGEN_MAJOR, 1), NULL, "seqgen");
	
	if(mode == NULL) {
		mode = kmalloc(sizeof(int), GFP_KERNEL);
		if(mode == NULL) {
			printk(KERN_ERR "[SEQGEN_REPEATER] Error : allocation error\n");
			return -2;
		}
	}
	*mode = 0;
	
	return 0;
}


void cleanup_module(void) {

	unregister_chrdev(SEQGEN_MAJOR, "seqgen");
	device_destroy(seqgen_class, MKDEV(SEQGEN_MAJOR, 1));
	class_destroy(seqgen_class);
	if(mode != NULL) {
		kfree(mode);
		mode = NULL;
	}
}



static int seqgen_open(struct inode *inod, struct file *fil) {

	if (device_open){
		printk(KERN_ERR "[SEQGEN_REPEATER] Error : seqgen already opened\n");
		return -EBUSY;
	}
	
	fil->f_op = &seqgen_fops;
	device_open++;
	try_module_get(THIS_MODULE);
	
	return 0;
}

static int seqgen_release(struct inode * inod, struct file *fil) {

	if(device_open)
		device_open--;
		
	module_put(THIS_MODULE);
	
	return 0;
}

static ssize_t seqgen_read(struct file *filp, char *buff, size_t len, loff_t *off){
	
	if(sequence == NULL) {
		printk(KERN_ERR "[SEQGEN_REPEATER] Error : Sequence unset\n");
		return -3;
	}
	
	if(len < 0) {
		printk(KERN_ERR "[SEQGEN_REPEATER] Error : Size argument not valid\n");
		return -4;
	}
	
	if(len > strlen(buff)) {
		printk(KERN_ERR "[SEQGEN_REPEATER] Error : Give size iss bigger than buffer size\n");
		return -5;
	}
	
	copy_to_user(buff, sequence->seq, len);
		/* NOT REPEATED MOD */
	if(*mode == 1) {
		*off += sequence->size;
		return 2 * sequence->size - *off;
	}
	
	return sequence->size;
}

static ssize_t seqgen_write(struct file *filp,const char *buff, size_t len, loff_t *off) {

	printk(KERN_WARNING "[SEQGEN_REPEATER] Error : This operation isn't supported.\n");
	return -EINVAL;
}

static long seqgen_ioctl(struct file *filp,unsigned int cmd,unsigned long arg) { 

	if(cmd == 0)
		*mode = 0;
	else if(cmd == 1)
		*mode = 1;
	else
		printk(KERN_ERR "[SEQGEN_REPEATER] Error : Wrong mode\n");

	return 0;
}

module_init(init_module);
module_exit(cleanup_module);

