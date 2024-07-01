#include "hw2_test.h"

int set_weight(int weight) {
    long r = syscall(334, weight);
    return r;
}

int get_weight() {
    long r = syscall(335);
    return r;
}

int get_path_sum(pid_t target) {
	long r = syscall(336, target);
    return r;
}

pid_t get_heaviest_sibling(void) {
	long r = syscall(337);
    return r;
}
