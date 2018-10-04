LIBNAME = tihu
TARGET=lib$(LIBNAME).so
CC = g++
OBJSDIR = obj/
SRCS = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp)
OBJS = $(addprefix $(OBJSDIR),$(patsubst %.cpp,%.o,$(SRCS)))
INC = `python2.7-config --includes` -Xlinker -export-dynamic
## for debugging
CXXFLAGS = -c -Wall -std=c++14 $(INC) -DDEBUG -g -fPIC
#CXXFLAGS = -c -fPIC -Wall -std=c++14 $(INC) -O3 -g
LDFLAGS = `python2.7-config --ldflags` -shared -Wl,-soname,$(TARGET)


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o ./build/$(TARGET)

$(OBJSDIR)%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJSDIR)

play:
	$(CC) tools/tihu_play/main.cpp -o ./build/tihu_play -L. -ldl

test:
	$(CC) -std=c++17 test/main.cpp -o  ./build/tihu_test -L. -ldl -lstdc++fs
	./build/tihu_test ./build/libtihu.so ../test/res

# to print variables in makefile
print-%  : ; @echo $* = $($*)


.PHONY: all clean test