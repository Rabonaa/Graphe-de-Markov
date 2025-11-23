cmake_minimum_required(VERSION 3.24)
project(2526_TI301_PJT C)

set(CMAKE_C_STANDARD 11)


include_directories(hdr/)
add_executable(TI_301_PJT
        main.c
        src/partie1/utils.c
        hasse.c
        src/partie1/cglm.c
        src/partie2/etape1.c
        hdr/partie2/etape1.h
)

