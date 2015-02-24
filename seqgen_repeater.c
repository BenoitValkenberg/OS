#include <linux/module.h>
#include <linux/string.h>
#include<linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Sequence Generator Repeater");
MODULE_AUTHOR("Quenti Boileau , Valkenberg Benoit");

static char msg[100]={0};
static short readPos= 0;

static int device_open = 0;

static int MAJOR = 89;

// called when 'open' system call is done on the seqgen file
static int seqgen_open(struct inode * , struct file *);

// caled when 'write' system call is done on the seqgen file
static int seqgen_rls(struct inode * , struct file *);

// called when 'read' system call is done on the seqgen file
static ssize_t seqgen_read(struct file *, char * , size_t, loff_t *);

// caled when 'write' system call is done on the seqgen file
static ssize_t seqgen_write(struct file *, const char * , size_t , loff_t *);

//structure containg callbacks
const struct file_operations fops = {
	.read = seqgen_read,
	.open = seqgen_open,
	.write = seqgen_write,
	.release = seqgen_rls,
};



// called when module is loaded
int init_module(void){
	int t = register_chrdev(MAJOR, "SeqGen_Repeater" , &fops);
	if(t < 0){
		printk(KERN_ALERT "SEQGEN Error : seqgen registration failed\n");
		return -1;
	}
	
	printk(KERN_ALERT "seqgen registered\n");
	
	//Pas déclaré ??
	seqgen_class = class_create(THIS_MODULE, "seqgen");
	if (IS_ERR(seqgen_class))
		return PTR_ERR(seqgen_class);
	device_create(seqgen_class, NULL, MKDEV(SEQGEN_MAJOR, minor), NULL, "seqgen");
	
	return t;
}	

// called when module is unloaded
void cleanup_module(void){
	unregister_chrdev(MAJOR , "SeqGen_Repeater");
}



static int seqgen_open(struct inode *inod, struct file *fil){
	if (device_open){
		printk(KERN_ALERT "SEQGEN Error : seqgen already opened\n");
		return -EBUSY;
	}
	
	fil->f_op = &seqgen_fops;
	device_open++;
	printk(KERN_ALERT "seqgen opened\n" );
	return 0;
}

static int seqgen_rls(struct inode * inod, struct file *fil){
	if(device_open)
		device_open--;
	printk(KERN_ALERT "seqgen closed\n");
	return 0;
}

static ssize_t seqgen_read(struct file *filp, char *buff, size_t len, loff_t *off){
	short count = 0;
	while (len &&(msg[readPos] != 0)){
		put_user(msg[readPos] , buff++); //Copy byte from kernel space to user space
		count++;
		len--;
		readPos++;
	}
	return count;
}

static ssize_t seqgen_write(struct file *filp,const char *buff, size_t len, loff_t *off){
	printk(KERN_ALERT "Sorry, this operation isn't supported.\n");
	return -EINVAL;
}


















