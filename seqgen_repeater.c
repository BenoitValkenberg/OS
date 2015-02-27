#include <linux/module.h>
#include <linux/string.h>
#include<linux/fs.h>
#include <asm/uaccess.h>
//#include <seqgen.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Sequence Generator Repeater");
MODULE_AUTHOR("Quentin Boileau , Benoit Valkenberg");

static char msg[100]={0};
static short readPos= 0;
static int device_open = 0;
static int MAJOR = 261;
static char* name = "seqgen";
static int mode;

extern Sequence sequence;

static struct class *seqgen_class; // Global variable for the device class

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
	.release = seqgen_release,
	.ioctl =  seqgen_ioctl,
};



// called when module is loaded
int seqgen_init(void){
	int result = register_chrdev(MAJOR, name , &fops);
	if(result < 0){
		printk(KERN_ALERT "SEQGEN Error : seqgen registration failed\n");
		return result;
	}
	
	printk(KERN_ALERT "seqgen registered\n");
	
	//Pas déclaré ??
	seqgen_class = class_create(THIS_MODULE, name);
	if (IS_ERR(seqgen_class))
		return PTR_ERR(seqgen_class);
	device_create(seqgen_class, NULL, MKDEV(SEQGEN_MAJOR, minor), NULL, name);
	
	return t;
}	

// called when module is unloaded
void seqgen_exit(void){
	unregister_chrdev(MAJOR , name);
	
	device_destroy(seqgen_class, first);
	class_destroy(seqgen_class);
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

static int seqgen_release(struct inode * inod, struct file *fil){
	if(device_open)
		device_open--;
	printk(KERN_ALERT "seqgen closed\n");
	return 0;
}

static ssize_t seqgen_read(struct file *filp, char *buff, size_t len, loff_t *off){
	
	printk(KERN_ALERT "SEQGEN Message : READ\n");
	short count = 0;
	
	if(sequence == NULL){
		printk(KERN_ALERT "SEQGEN Error : No sequence encoded\n");
		return -1;
	}
	
	if(mode == 0){
		while(1){
			printk(KERN_ALERT sequence->seq);
		}
	}
	
	else{
		printk(KERN_ALERT sequence->seq);
	}
	
	return count;
}

static ssize_t seqgen_write(struct file *filp,const char *buff, size_t len, loff_t *off){
	printk(KERN_ALERT "SEQGEN Error : This operation isn't supported.\n");
	return -EINVAL;
}

int seqgen_ioctl(struct inode *inode,struct file *filp,unsigned int cmd,unsigned long arg) { 
	if(cmd == 0)
		mode = 0;
	else if(cmd == 1)
		mode = 1;
	else
		printk(KERN_ALERT "SEQGEN Error : Wrong mode\n");
	
	printk(KERN_ALERT "Mode updated !\n");
	return 0;
}



/*while (len &&(sequence->seq[readPos] != 0)){
		put_user(sequence->seq[readPos] , buff++); //Copy byte from kernel space to user space
		count++;
		len--;
		readPos++;
	}*/









