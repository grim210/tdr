#!/bin/sh

g++ -std=c++11 sample02.cpp -o sample_02 -lGL -lglfw -ltdrmain -ltdrrenderer -ldl -I../../main/include -I../../renderer/include -L../../build/main/src -L../../build/renderer/src
