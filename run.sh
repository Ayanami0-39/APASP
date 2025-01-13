#!/bin/bash

# 设置构建目录名称
BUILD_DIR="build"

# 检查构建目录是否存在，如果不存在则创建
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory: $BUILD_DIR"
    mkdir "$BUILD_DIR"
fi

# 进入构建目录
cd "$BUILD_DIR"

# 执行 cmake 进行构建
echo "Running CMake build in foler '$BUILD_DIR'"
cmake .. > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "cmake ok"
else
    echo "'camke ..' failed with return value: $?"
    exit $?
fi

make > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "build ok"
else
    echo "'make' failed with return value: $?"
    exit $?
fi

cd ..
python3 ./validation.py