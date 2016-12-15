TARGET=libmain.a
CXX=g++
CXXFLAGS=-c -std=c++11 -Wall -Werror -I../include/ -DRENDERER_DEBUG
AR=ar
ARFLAGS=rcs
RM=rm -rf
OBJS=\
	camera.o \
	directdrawtexture.o \
	util.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJS)

camera.o: camera.cpp
	$(CXX) $(CXXFLAGS) camera.cpp -o camera.o

directdrawtexture.o: directdrawtexture.cpp 
	$(CXX) $(CXXFLAGS) directdrawtexture.cpp -o directdrawtexture.o

util.o: util.cpp
	$(CXX) $(CXXFLAGS) util.cpp -o util.o

clean:
	$(RM) $(OBJS)

distclean:
	$(RM) $(OBJS) $(TARGET)
