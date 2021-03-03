#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/moduleparam.h>
#include<linux/fs.h>
#include<linux/buffer_head.h>
#include<asm/segment.h>
#include<asm/uaccess.h>


MODULE_LICENSE("GPL");

struct file *open_file_for_read(const char *path, int flags, int rights){
	struct file *filp = NULL;
	mm_segment_t oldfs;
	int err = 0;

	oldfs = get_fs();
	set_fs(get_ds());
	filp = filp_open(path, flags, rights);

	set_fs(oldfs);

	if(IS_ERR(filp)){
		err = PTR_ERR(filp);
		return NULL;
	}

	return filp;

}

void close_file(struct file *file){
	filp_close(file, NULL);
}

int read_from_file_until(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size){
	mm_segment_t oldfs;
	int ret;

	oldfs = get_fs();
	set_fs(get_ds());

	ret = vfs_read(file, data, size, &offset);

	set_fs(oldfs);
	return ret;
	
}

struct file *fp;
static int hello_init(void){
	printk(KERN_ALERT "Hello CSCE-3402 :)\n");

	fp = open_file_for_read("/proc/version",0,0);
	char *data;
	read_from_file_until(fp, 0, data, 1024);
	close_file(fp);
	return 0;
}

static void hello_exit(void){
	printk(KERN_ALERT "Bye Bye CSCE-3402 :)\n");
}

module_init(hello_init);
module_exit(hello_exit);
