TARGET=test
CXX=g++
CXXFLAGS=-c -std=c++11 -I./include/
LD=g++
LDFLAGS=-lmingw32 -lglfw3 -lopengl32 -mconsole
RM=rm -rf
OBJS=\
	glad.o \
	main.o \
	programmanager.o \
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

window.o: window.cpp window.h
	$(CXX) $(CXXFLAGS) window.cpp -o window.o

clean:
	$(RM) $(OBJS)

distclean:
	$(RM) $(OBJS) $(TARGET)
