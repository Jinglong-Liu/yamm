#include"yamm.h"
#include"list.h"

LIST_HEAD(free_list);
LIST_HEAD(allocated_list);
LIST_HEAD(idle_list);

struct memory_block {
    struct list_head list;
    // TODO: some code goes here
    // 添加其他必要的属性
};


int yammInit(int size) {
    // TODO: some code goes here
    return 0;
}

int yammDump() {
    // TODO: some code goes here
    return 0;
}

void* yammAlloc(int size) {
    // TODO: some code goes here
    return NULL;
}

int yammFree(void* ptr) {
    // TODO: some code goes here
    return 0;
}

int yammDestroy() {
    // TODO: some code goes here
    return 0;
}
