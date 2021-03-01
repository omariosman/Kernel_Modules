#include<linux/init.h>
#include<linux/module.h>

#include<linux/moduleparam.h>

int counter = 0;
char *msg;

module_param(counter, int, S_IRUSR | S_IWUSR);
module_param(msg, charp, 0);

void display(void){
	int i;
	for(i =0;i < counter; i++){
		printk("%s", msg);
	}
	//printk("Value: %d", counter);
	//printk("Msg is: %s", msg);
}

static int hello_init(void){
	printk("Hello CSCE-3402 :)");
	display();
	return 0;
}

static void hello_exit(void){
	printk("Bye Bye CSCE-3402 :)");
}

module_init(hello_init);
module_exit(hello_exit);
