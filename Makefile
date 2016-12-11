TARGET=test
CXX=g++
CXXFLAGS=-c -std=c++11 -I./include/ -DRENDERER_DEBUG
LD=g++
LDFLAGS=-lmingw32 -lglfw3 -lopengl32 -mconsole
RM=rm -rf
OBJS=\
	cube.o \
	glad.o \
	main.o \
	shaderprogram.o \
	util.o \
	window.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(OBJS) -o $(TARGET) $(LDFLAGS)

cube.o: cube.cpp cube.h
	$(CXX) $(CXXFLAGS) cube.cpp -o cube.o

glad.o: glad.c
	$(CXX) $(CXXFLAGS) glad.c -o glad.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o main.o

shaderprogram.o: shaderprogram.cpp shaderprogram.h
	$(CXX) $(CXXFLAGS) shaderprogram.cpp -o shaderprogram.o

util.o: util.cpp util.h
	$(CXX) $(CXXFLAGS) util.cpp -o util.o

window.o: window.cpp window.h
	$(CXX) $(CXXFLAGS) window.cpp -o window.o

clean:
	$(RM) $(OBJS)

distclean:
	$(RM) $(OBJS) $(TARGET)
