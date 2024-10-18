#include "yamm.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

// 测试 malloc/free
int testcase1() {
    printf("testcase1 start\n");
    int r1 = yammInit(2048);
    assert(0 == r1);
    int r2 = yammInit(2048);
    assert(-1 == r2);
    void* r3 = yammAlloc(1024);
    assert(NULL != r3);
    void* r4 = yammAlloc(1025);
    assert(NULL == r4);
    void* r5 = yammAlloc(512);
    assert(NULL != r5);
    void* r6 = yammAlloc(512);
    assert(NULL != r6);
    int r7 = yammFree(r3);
    assert(0 == r7);
    int r8 = yammFree(r3);
    assert(-1 == r8);
    void* r9 = yammAlloc(1000);
    assert(NULL != r9);
    void* r10 = yammAlloc(24);
    assert(NULL != r10);
    void* r11 = yammAlloc(-1);
    assert(NULL == r11);
    void* r12 = yammAlloc(0);
    assert(NULL == r12);
    int r13 = yammFree(0);
    assert(-1 == r13);
    yammDestroy();
    printf("testcase1 passed\n\n");
    return 0;
}

// 测试 init/destroy 状态恢复
int testcase2() {
    printf("testcase2 start\n");
    int r1 = yammInit(2048);
    assert(0 == r1);
    void* r2 = yammAlloc(512);
    assert(NULL != r2);
    int r3 = yammDestroy();
    assert(0 == r3);
    int r4 = yammDestroy();
    assert(-1 == r4);
    int r5 = yammInit(1024);
    assert(0 == r5);
    void* r6 = yammAlloc(1024);
    assert(NULL != r6);
    void* r7 = yammAlloc(1);
    assert(NULL == r7);
    int r8 = yammDestroy();
    assert(0 == r8);
    int r9 = yammInit(512);
    assert(0 == r9);
    void* r10 = yammAlloc(512);
    assert(NULL != r10);
    int r11 = yammDestroy();
    assert(0 == r11);
    yammDestroy();
    yammInit(16384);
    for (int i = 0; i < (1 << 20); i++) {
        void* addr = yammAlloc(16384);
        assert(NULL != addr);
        assert(0 == yammFree(addr));
    }
    yammDestroy();
    printf("testcase2 passed\n\n");
    return 0;
}

// 测试结点数量管理
int testcase3() {
    printf("testcase3 start\n");
    yammInit(2048);
    // 一共32个结点可用
    // 用不完空间，需要一个结点额外保存free
    void* addr[MAX_MEMORY_BLOCKS];
    for(int i = 0; i < MAX_MEMORY_BLOCKS; i++) {
        addr[i] = yammAlloc(32);
        if (addr[i] == NULL) {
            assert(i == MAX_MEMORY_BLOCKS - 1);
            break;
        }
        //assert(NULL != addr[i]);
    }
    void* x = yammAlloc(32);
    assert(NULL == x);
    yammDestroy();
    // 空间刚好用完，不需要额外结点保存free
    yammInit(1024);
    for(int i = 0; i < MAX_MEMORY_BLOCKS + 1; i++) {
        addr[i] = yammAlloc(32);
        if (addr[i] == NULL) {
            assert(i == MAX_MEMORY_BLOCKS);
            break;
        }
    }
    x = yammAlloc(1);
    assert(NULL == x);
    yammDestroy();
    printf("testcase3 passed\n\n");
    return 0;
}

// 测试连续内存块合并
int testcase4() {
    printf("testcase4 start\n");
    yammInit(1024);
    void* addr[16];
    for (int i = 0; i < 16; i++) {
        addr[i] = yammAlloc(64);
        assert(NULL != addr[i]);
    }
    assert(NULL == yammAlloc(1));
    yammFree(addr[10]);
    yammFree(addr[12]);
    void* x = yammAlloc(128);
    assert(NULL == x);
    yammFree(addr[11]);
    x = yammAlloc(192);
    assert(NULL != x);
    assert(NULL == yammAlloc(1));
    yammDestroy();
    printf("testcase4 passed\n\n");
    return 0;
}

struct node {
    int x;
    int y;
    char str[56];
};

// 测试内存块使用情况
int testcase5() {
    printf("testcase5 start\n");
    yammInit(1024);
    struct node *nodes[16] = {NULL};
    // 分配并为每个节点赋值
    for (int i = 0; i < 16; i++) {
        nodes[i] = yammAlloc(sizeof(struct node)); // 分配内存
        assert(NULL != nodes[i]); // 确保分配成功
        // 为节点的字段赋值
        nodes[i]->x = i;
        nodes[i]->y = i * 2;
        snprintf(nodes[i]->str, sizeof(nodes[i]->str), "Node #%d", i);
    }

    // 检查，确保没有内存被覆盖
    for (int i = 0; i < 16; i++) {
        assert(nodes[i]->x == i); // 确认 x 字段没有被覆盖
        assert(nodes[i]->y == i * 2); // 确认 y 字段没有被覆盖
        char expected_str[56];
        snprintf(expected_str, sizeof(expected_str), "Node #%d", i);
        assert(strcmp(nodes[i]->str, expected_str) == 0); // 确认 str 字段没有被覆盖
        assert(0 == yammFree(nodes[i]));
    }
    yammDestroy();
    printf("testcase5 passed\n\n");
    return 0;
}

int main() {
    testcase1();
    testcase2();
    testcase3();
    testcase4();
    testcase5();
    return 0;
}
