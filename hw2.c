#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h> // Required for task_struct and current
#include <linux/list.h> // Required for list_for_each_entry

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
        task = task->parent; // Koren: what's the difference between parent and real_parent?
    }

    if (task != current) {
        return -ECHILD;
    }

    return sum;
}

asmlinkage long sys_get_heaviest_sibling(void) {
    struct task_struct *sibling;
    int max_weight = -1;
    pid_t heaviest_pid = -1;

    /*
    Syntax:

    list_for_each_entry(pos, head, member)

    pos: A pointer to the structure that contains the list_head. It is used as the loop cursor.
    head: A pointer to the head of the list.
    member: The name of the list_head field within the structure.

    So in our case:

    struct task_struct *sibling: This pointer will be used to traverse the list.
    
    list_for_each_entry(sibling, &current->real_parent->children, sibling)
    
    sibling: The loop cursor, which will point to each sibling process in turn.
    &current->real_parent->children: The head of the list, which is the children field in the real_parent of the current process.
    sibling: The name of the list_head field in the task_struct structure, which links the sibling processes.
    */
    list_for_each_entry(sibling, &current->real_parent->children, sibling) { // Koren: what's the best? list_for_each / list_entry / list_for_each_entry?
        // If the current sibling has a higher weight than max_weight, update max_weight and heaviest_pid
        if (sibling->weight > max_weight) {
            max_weight = sibling->weight;
            heaviest_pid = sibling->pid;
        } 
        // If the current sibling has the same weight as the max_weight, choose the sibling with the smallest PID
        else if (sibling->weight == max_weight) {
            if (heaviest_pid == -1 || sibling->pid < heaviest_pid) {
                heaviest_pid = sibling->pid;
            }
        }
    }

    return heaviest_pid;
}
