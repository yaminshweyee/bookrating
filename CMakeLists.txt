cmake_minimum_required(VERSION 3.21)
project(P1)

set(CMAKE_CXX_STANDARD 14)

add_executable(P1 p1.cpp BookList.h MemberList.h BookList.cpp MemberList.cpp RatingList.cpp RatingList.h)
