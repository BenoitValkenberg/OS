#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/seqgen.h>
#include <linux/ioctl.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/device.h>


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Sequence Generator Repeater");
MODULE_AUTHOR("Quentin Boileau , Benoit Valkenberg");


//static char msg[100]={0};
//static short readPos= 0;
static int device_open = 0;
static char* name = "seqgen";
static int mode;


static struct class *seqgen_class; // Global variable for the device class

static int seqgen_open(struct inode * , struct file *);
static int seqgen_release(struct inode * , struct file *);
static ssize_t seqgen_read(struct file *, char * , size_t, loff_t *);
static ssize_t seqgen_write(struct file *, const char * , size_t , loff_t *);

static int seqgen_ioctl(struct inode *inode,struct file *filp,unsigned int cmd,unsigned long arg);


//structure containg callbacks
static const struct file_operations seqgen_fops = {
	.read = seqgen_read,
	.open = seqgen_open,
	.write = seqgen_write,
	.release = seqgen_release,
	.unlocked_ioctl = seqgen_ioctl
};



// called when module is loaded
int init_module(void) {

	int result = register_chrdev(SEQGEN_REPEATER_MAJOR, name, &seqgen_fops);
	if(result < 0) {
		printk(KERN_ALERT "SEQGEN_REPEATER Error : seqgen registration failed\n");
		return result;
	}
	
	printk(KERN_ALERT "seqgen registered\n");
	
	
	seqgen_class = class_create(THIS_MODULE, name);
	if (IS_ERR(seqgen_class))
		return PTR_ERR(seqgen_class);
		
	device_create(seqgen_class, NULL, MKDEV(SEQGEN_REPEATER_MAJOR, 1), NULL, name);
	
	return 0;
}	

// called when module is unloaded
void cleanup_module(void) {

	unregister_chrdev(SEQGEN_REPEATER_MAJOR, name);
	device_destroy(seqgen_class, MKDEV(SEQGEN_REPEATER_MAJOR, 1));
	class_destroy(seqgen_class);
}



static int seqgen_open(struct inode *inod, struct file *fil) {

	if (device_open){
		printk(KERN_ALERT "SEQGEN_REPEATER Error : seqgen already opened\n");
		return -EBUSY;
	}
	
	fil->f_op = &seqgen_fops;
	device_open++;
	
	try_module_get(THIS_MODULE);
	//MOD_INC_USE_COUNT;
	printk(KERN_ALERT "seqgen opened\n" );
	
	return 0;
}

static int seqgen_release(struct inode * inod, struct file *fil) {

	if(device_open)
		device_open--;
		
	module_put(THIS_MODULE);
	//MOD_DEC_USE_COUNT;
	printk(KERN_ALERT "seqgen closed\n");
	
	return 0;
}

static ssize_t seqgen_read(struct file *filp, char *buff, size_t len, loff_t *off){
	
	int bytes_read = 0;
	printk(KERN_ALERT "SEQGEN_REPEATER Message : READ\n");
	
	if(sequence == NULL) {
		printk(KERN_ALERT "SEQGEN_REPEATER Error : No sequence encoded\n");
		return -1;
	}
	
	if(mode == 0) {
		while(1) {
			while(len && *(sequence->seq)) {
				put_user(*(sequence->seq++), buff++);
				len--;
				bytes_read++;
			}
		}
	}
	
	else {
		while(len && *(sequence->seq)) {
				put_user(*(sequence->seq)++, buff++);
				len--;
				bytes_read++;
			}
	}
	
	return bytes_read;
}

static ssize_t seqgen_write(struct file *filp,const char *buff, size_t len, loff_t *off) {

	printk(KERN_ALERT "SEQGEN_REPEATER Error : This operation isn't supported.\n");
	return -EINVAL;
}

static int seqgen_ioctl(struct inode *inode,struct file *filp,unsigned int cmd,unsigned long arg) { 
	if(cmd == 0)
		mode = 0;
	else if(cmd == 1)
		mode = 1;
	else
		printk(KERN_ALERT "SEQGEN_REPEATER Error : Wrong mode\n");
	
	printk(KERN_ALERT "Mode updated !\n");
	return 0;
}



/*while (len &&(sequence->seq[readPos] != 0)){
		put_user(sequence->seq[readPos] , buff++); //Copy byte from kernel space to user space
		count++;
		len--;
		readPos++;
	}*/








