TARGET=test
CXX=g++
CXXFLAGS=-c -std=c++11 -I./include/ -DRENDERER_DEBUG
LD=g++
LDFLAGS=-lmingw32 -lglfw3 -lopengl32 -mconsole
RM=rm -rf
OBJS=\
	glad.o \
	main.o \
	programmanager.o \
	triangle.o \
	util.o \
	window.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(OBJS) -o $(TARGET) $(LDFLAGS)

glad.o: glad.c
	$(CXX) $(CXXFLAGS) glad.c -o glad.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o main.o

programmanager.o: programmanager.cpp programmanager.h
	$(CXX) $(CXXFLAGS) programmanager.cpp -o programmanager.o

triangle.o: triangle.cpp triangle.h
	$(CXX) $(CXXFLAGS) triangle.cpp -o triangle.o

util.o: util.cpp util.h
	$(CXX) $(CXXFLAGS) util.cpp -o util.o

window.o: window.cpp window.h
	$(CXX) $(CXXFLAGS) window.cpp -o window.o

clean:
	$(RM) $(OBJS)

distclean:
	$(RM) $(OBJS) $(TARGET)
