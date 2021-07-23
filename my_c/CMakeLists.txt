cmake_minimum_required(VERSION 3.19)
project(my_c C)

set(CMAKE_C_STANDARD 11)

# GLOB_RECURSE会自动遍历工程跟目录下的所有文件目录
file(GLOB_RECURSE files *.c)
foreach (file ${files})
    string(REGEX REPLACE ".+/(.+)\\..*" "\\1" exe ${file})
    add_executable(${exe} ${file})
    message(\ \ \ \ --\ src/${exe}.c\ will\ be\ compiled\ to\ bin/${exe})
endforeach ()