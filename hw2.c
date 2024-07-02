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

asmlinkage long sys_get_path_sum(pid_t target) {
    struct task_struct *task;
    int sum = 0;

    task = pid_task(find_vpid(target), PIDTYPE_PID); // Find the task struct for the given PID
    if (!task) {
        return -ECHILD;
    }

    while (task) {
        sum += task->weight;
        if (task == current) {
            break;
        }
        task = task->parent;
    }

    if (task != current) {
        return -ECHILD;
    }

    return sum;
}
