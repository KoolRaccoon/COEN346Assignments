/**
 * Project 1: Linux Kernel Modules.
 * Michael Magnabosco	27189737
 * Felix Adrian Lucaciu 27397941
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>

#define NUM_OF_PEOPLE 5

/*Birthday Structure*/
struct birthday {
	int day;
	int month;
	int year;
	struct list_head list;
};

/*Initializes and declares the head of the linked list*/
static LIST_HEAD(birthday_list); 

/* This function is called when the module is loaded. */
int project1_init(void) {
	struct birthday *person, *ptr; 
	int i;

	printk(KERN_INFO "Loading Module\n");	

	/*Adds 5 birthdays to the kernel*/
	for(i=0; i < NUM_OF_PEOPLE; i++) {
		/*Requests malloc to the kernel*/
		person = kmalloc(sizeof(*person), GFP_KERNEL); 
		/*gives the value to the birthday struct*/		
		person->day = 22+i; 
		person->month= 1; 
		person->year = 2018; 
		/*initializes the list*/
		INIT_LIST_HEAD(&person->list); 

		/*Moves the struct to the end of the list (tail)*/
		list_add_tail(&person->list, &birthday_list); 
	}	
	
	/*goes through the list and prints all the birthdays*/
	printk(KERN_INFO "Birthdays:\n");

	list_for_each_entry(ptr, &birthday_list, list) { 
		printk(KERN_INFO "day: %d, month: %d, year:%d\n", ptr->day, ptr->month, ptr->year);
	}

	return 0;
}

/* This function is called when the module is removed. */
void project1_exit(void) {
	struct birthday *ptr,*next;	

	/*goes through the list and removes all the birthdays and prints them*/
	printk(KERN_INFO "Removing Module\n");
	
	list_for_each_entry_safe(ptr,next,&birthday_list,list) { 
		printk(KERN_INFO "Removing: day: %d, month: %d, year:%d\n", ptr->day, ptr->month, ptr->year);
		list_del(&ptr->list); 
		kfree(ptr); 
	}
}

/* Macros for registering module entry and exit points. */
module_init(project1_init);
module_exit(project1_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Project1 Module");
MODULE_AUTHOR("SGG");
