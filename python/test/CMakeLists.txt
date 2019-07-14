cmake_minimum_required(VERSION 2.8)

ADD_LIBRARY(oo SHARED oo.cpp)

set_target_properties(oo PROPERTIES PREFIX "")

