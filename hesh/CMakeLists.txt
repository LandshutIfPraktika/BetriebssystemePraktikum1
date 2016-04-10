cmake_minimum_required(VERSION 3.5)
project(hesh)

set(SOURCE_FILES main.c hesh.c hesh.h hesh_logger.c hesh_logger.h)
add_executable(hesh ${SOURCE_FILES})