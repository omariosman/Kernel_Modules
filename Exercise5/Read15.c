#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/moduleparam.h>
#include<linux/fs.h>
#include<linux/buffer_head.h>
#include<asm/segment.h>
#include<asm/uaccess.h>
#include<linux/slab.h>
#include<linux/syscalls.h>
#include<linux/kallsyms.h>


MODULE_LICENSE("GPL");

struct myfile{
	struct file*f;
	mm_segment_t fs;
	loff_t pos;
};

struct myfile *open_file_for_read(char *filename){
	struct myfile *myfile_obj;
	myfile_obj = kmalloc(sizeof(struct myfile), GFP_KERNEL);

	
	
	int err = 0;

	myfile_obj->fs = get_fs();
	set_fs(get_ds());
	myfile_obj->f = filp_open(filename, O_RDONLY,0);
	myfile_obj->pos = 0;


	set_fs(myfile_obj->fs);

	if(IS_ERR(myfile_obj->f)){
		err = PTR_ERR(myfile_obj->f);
		return NULL;
	}

	return myfile_obj;

}

void close_file(struct myfile *mf){
	filp_close(mf->f, NULL);
	kfree(mf);
}

int read_from_file_until(struct myfile *mf, char *buf, unsigned long vlen, char c){
	
	int ret = 0;
	mf->fs = get_fs();
	set_fs(get_ds());
	
	int i;
	for (i = 0; i < vlen; i++){
		ret = vfs_read(mf->f, buf + i,1, &(mf->pos));
		if ((buf[i] == c) || (buf[i]) == '\0'){
			break;
		}
	}

	i++;
	buf[i] = '\0';
	set_fs(mf->fs);
	return ret;
	
}

struct file *fp;
static int hello_init(void){
	/*
	char *system_map;
	system_map = kmalloc(100*sizeof(char), GFP_KERNEL);
	memset(system_map, 0, 100);
	*/

	struct myfile* mf = open_file_for_read("/proc/version");
	char *data;
	data = kmalloc(100*sizeof(char), GFP_KERNEL);
	read_from_file_until(mf, data, 1024, ' ');
	read_from_file_until(mf, data, 1024, ' ');
	read_from_file_until(mf, data, 1024, ' ');
	
	printk("Kernel Version: %s\n", data);
	/*
	system_map = "System.map-";
	
	int k;
	int my_counter = 11;
	for(k = 0; k < strlen(data); k++){
		system_map[my_counter] = data[k];
		my_counter++;
	}
	
	strcat(system_map, data);
	printk("system map: %s\n", system_map);
	*/
	close_file(mf);

	/* open systme map file */
	struct myfile *sysmap = open_file_for_read("/boot/System.map-4.19.0-13-amd64");
	char *line;
	line = kmalloc(100*sizeof(char), GFP_KERNEL);

	char *word;
	word = kmalloc(100*sizeof(char), GFP_KERNEL);
	word = "sys_call_table";

	while(true){
	
	
		read_from_file_until(sysmap, line, 1024, '\n');
		if(strstr(line, word)){
			break;
		}
	}

	printk("sys call: %s\n", line);

	//parse thise line to get the address 
	char *sys_call_addr;
	sys_call_addr = kmalloc(100*sizeof(char), GFP_KERNEL);
	strncpy(sys_call_addr, line, 16);

	
	printk("Addr: %s\n", sys_call_addr);
/*
	char *forkaddress;
	forkaddress = kmalloc(100*sizeof(char), GFP_KERNEL);

	unsigned long* sys_call_arr;
	sys_call_arr = (unsigned long *)sys_call_addr;

	unsigned long** temp;	
	temp = sys_call_arr[__NR_fork];
*/
	

	
//	printk("fork address: %ld\n", temp);

	kfree(data);
	kfree(word);
	kfree(line);
	kfree(sys_call_addr);

	return 0;
}

static void hello_exit(void){

	printk(KERN_ALERT "Bye Bye CSCE-3402 :)\n");
	
}

module_init(hello_init);
module_exit(hello_exit);
