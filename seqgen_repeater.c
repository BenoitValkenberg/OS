#include <linux/module.h>
#include <linux/string.h>
#include<linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Sequence Generator Repeater");
MODULE_AUTHOR("Quenti Boileau , Valkenberg Benoit");

static char msg[100]={0};
static short readPos= 0;
static int times = 0;

static int MAJOR = 89;

// called when 'open' system call is done on the device file
static int dev_open(struct inode * , struct file *);

// caled when 'write' system call is done on the device file
static int dev_rls(struct inode * , struct file *);

// called when 'read' system call is done on the device file
static ssize_t dev_read(struct file *, char * , size_t, loff_t *);

// caled when 'write' system call is done on the device file
static ssize_t dev_write(struct file *, const char * , size_t , loff_t *);

//structure containg callbacks
static struct file_operations fops = {
	.read = dev_read,
	.open = dev_open,
	.write = dev_write,
	.release = dev_rls,
};



// called when module is loaded
int init_module(void){
	int t = register_chrdev(MAJOR, "SeqGen_Repeater" , &fops);
	if(t < 0)
		printk(KERN_ALERT "Device registration failed\n");
	else
		printk(KERN_ALERT "Device registered\n");
	
	return t;
}	

// called when module is unloaded
void cleanup_module(void){
	unregister_chrdev(89 , "SeqGen_Repeater");
}



static int dev_open(struct inode *inod, struct file *fil){
	times++;
	printk(KERN_ALERT "Device opened %d times\n" , times);
	return 0;
}

static int dev_rls(struct inode * inod, struct file *fil){
	printk(KERN_ALERT "Device closed\n");
	return 0;
}

static ssize_t dev_read(struct file *filp, char *buff, size_t len, loff_t *off){
	short count = 0;
	while (len &&(msg[readPos] != 0)){
		put_user(msg[readPos] , buff++); //Copy byte from kernel space to user space
		count++;
		len--;
		readPos++;
	}
	return count;
}

static ssize_t dev_write(struct file *filp,const char *buff, size_t len, loff_t *off){
	short ind = len-1;
	short count = 0;
	memset(msg,0,100);
	readPos=0;
	while(len > 0){
		msg[count++] = buff[ind--];
		len--;
	}
	return count;
}


















