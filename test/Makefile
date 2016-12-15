TARGET=test.exe
CXX=g++
CXXFLAGS=-c -std=c++11 -Wall -Werror -I../renderer/include -I../main/include/
LD=g++
LDFLAGS=-lmingw32 -lrenderer -lglfw3 -lopengl32 -L../renderer/src/
RM=rm -rf
OBJS=	main.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(OBJS) -o $(TARGET) $(LDFLAGS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o main.o

clean:
	$(RM) $(OBJS)

distclean:
	$(RM) $(OBJS) $(TARGET)
