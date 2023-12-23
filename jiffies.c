/**
 * jiffies.c
 *
 * Kernel module that prints the current jiffies
 * when cat /proc/jiffies is called
 *
 * matans1@mcmaster.ca
 * Based on John Wiley & Sons - 2018, hello.c
 */

// For kernel programming
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
// For proc read
#include <linux/proc_fs.h>
// For jiffies
#include <linux/jiffies.h>


// Defines a constant buffer size for the string to be printed when cat /proc/jiffies is called 
#define BUFFER_SIZE 128
// Defines the name of the proc
#define PROC_NAME "jiffies"


/**
 * Function Prototypes
 */
static ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

static struct proc_ops proc_ops = {
	.proc_read = proc_read,
};


/* This function is called when the module is loaded. */
int proc_init(void)
{
	/* creates the /proc/jiffies entry
	 * the following function call is a wrapper for
	 * proc_create_data() passing NULL as the last argument
	 */
	proc_create(PROC_NAME, 0666, NULL, &proc_ops);

	return 0;
}


/* This function is called when the module is removed */
void proc_exit(void)
{
	// removes the /proc/jiffies entry
	remove_proc_entry(PROC_NAME, NULL);
}

/**
 * This function is called each time the /proc/jiffies is read
 */

ssize_t proc_read(struct file *file, char __user *usr_buf,
		size_t count, loff_t *pos)
{
	// use rv to store the number of Jiffies
	int rv = 0;
	// initializes BUFFER_SIZE to the buffer variable
	char buffer[BUFFER_SIZE];
	// initializes the completed variable which is used to prevent repeat reads
	static int completed = 0;

	// Resets the completed variable back to 0 
	if (completed) {
		completed = 0;
		return 0;
	}

	// Set to 1 after the proc is read

	completed = 1;

	// Stores the number of jiffies in rv
	// sprintf returns the number of jiffies
	// in buffer (excluding null padding characters)
	rv = sprintf(buffer, "Jiffies: %lu\n", jiffies);
	// copies the contents of buffer to userspace usr.buf
	copy_to_user(usr_buf, buffer, rv);
	// return printed jiffies
	return rv;
}

/* Set the module entry and exit points */
module_init(proc_init);
module_exit(proc_exit);

/* Maintaining original license */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello Module");
MODULE_AUTHOR("SGG");