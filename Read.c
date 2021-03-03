#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/moduleparam.h>
#include<linux/fs.h>
#include<asm/uaccess.h>

MODULE_LICENSE("GPL");

#define EOK 0

volatile static int is_open = 0;

static char message[1024];
int num_bytes = 0;


ssize_t read_from_file_until (struct file* filep, char __user* outb, size_t nbytes, loff_t *offset){
	int bytes_read = 0;
	if (offset == NULL){
		return -EINVAL;
	}
	if (*offset >= nbytes){
		return 0;
	}
	while ((bytes_read < nbytes) && (*offset < num_bytes)){
		put_user(message[*offset], &outb[bytes_read]);
		*offset = *offset + 1;
		bytes_read++;
	}
	return bytes_read;

}

int open_file_for_read (struct inode *inodep, struct file * filep){
	if (is_open == 1){
		printk(KERN_INFO "Already Open\n");
		return -EBUSY;
	}

	is_open = 1;
	try_module_get(THIS_MODULE);
	return 0;
}

int close_file (struct inode* inodep, struct file *filep){
	if (is_open == 0){
		printk(KERN_INFO "Already Closed\n");
		return -EBUSY;
	}
	is_open = 0;
	module_put(THIS_MODULE);
	return 0;
}

struct file_operations fops = {
	open: open_file_for_read,
	read: read_from_file_until,
	release: close_file
};

static int devnum;
static int hello_init(void){
	
	printk(KERN_ALERT "Hello CSCE-3402 :)\n");
	
	strncpy(message, "Test msg", 1024);
	num_bytes = strlen(message);

	devnum = register_chrdev(0, "ReadFile1", &fops);
	printk("Dev num: %d\n", devnum);
	//display();
	return 0;
}

static void hello_exit(void){
	printk(KERN_ALERT "Bye Bye CSCE-3402 :)\n");
}

module_init(hello_init);
module_exit(hello_exit);
