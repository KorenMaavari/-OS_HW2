#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h> // Required for task_struct and current

asmlinkage long sys_hello(void) {
    printk("Hello, World!\n");
    return 0;
}

asmlinkage long sys_set_weight(int weight) {
    if (weight < 0) {
        return -EINVAL; // Return error if weight is negative
    }
    current->weight = weight;
    return 0;
}

asmlinkage long sys_get_weight(void) {
    return current->weight;
}
