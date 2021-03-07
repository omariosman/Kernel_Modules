#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/moduleparam.h>
#include<linux/fs.h>
#include<linux/buffer_head.h>
#include<asm/segment.h>
#include<asm/uaccess.h>
#include<linux/slab.h>

MODULE_LICENSE("GPL");

struct file *open_file_for_read(const char *path){
	struct file *filp = NULL;
	mm_segment_t oldfs;
	int err = 0;

	oldfs = get_fs();
	set_fs(get_ds());
	filp = filp_open(path, 0,0);

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

	fp = open_file_for_read("/proc/version");
	char *data;
	data = kmalloc(sizeof(char), GFP_KERNEL);
	char *linux_version;
	linux_version = kmalloc(sizeof(char), GFP_KERNEL);
	read_from_file_until(fp, 0, data, 1024);
	printk("Content: %s\n", data);
	close_file(fp);
	int i;
	int space_counter = 0;
	int k = 0;
	for (i = 0; i < strlen(data); i++){	
		if (data[i] == ' '){
			space_counter++;
		}
		if (space_counter==2){
			if (data[i] == ' '){
				linux_version[k] = data[i+1];
			} else {
				linux_version[k] = data[i];
			}
			k++;
		}
		
	}
	printk("My Linux version: %s\n", linux_version);
	kfree(linux_version);
	kfree(data);
	return 0;
}

static void hello_exit(void){
	printk(KERN_ALERT "Bye Bye CSCE-3402 :)\n");
}

module_init(hello_init);
module_exit(hello_exit);
