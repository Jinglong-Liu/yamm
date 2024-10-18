# 指定编译器
CC = gcc
CFLAGS = -g -Wall -Werror

# 目标可执行文件名称
TARGET = main

# 所有 .c 文件及其对应的 .o 文件
CPP_SOURCES = $(wildcard *.c)
OBJ_FILES = $(CPP_SOURCES:.c=.o)

# 规则：生成可执行文件
$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(TARGET)

# 规则：编译 .c 文件为 .o 文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 伪目标：清理生成的文件
.PHONY: clean
clean:
	rm -f $(OBJ_FILES) $(TARGET)
