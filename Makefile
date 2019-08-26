LIBNAME = tihu
TARGET=lib$(LIBNAME).so
CC = g++
OBJSDIR = obj/
SRCS = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp)
OBJS = $(addprefix $(OBJSDIR),$(patsubst %.cpp,%.o,$(SRCS)))
INC = `python-config --includes` -Xlinker -export-dynamic
## for debugging
CXXFLAGS = -c -Wall -std=c++14 $(INC) -DDEBUG -g -fPIC
##CXXFLAGS = -c -fPIC -Wall -std=c++14 $(INC) -O3
LDFLAGS = `python-config --ldflags` -lsamplerate -shared -Wl,-soname,$(TARGET)


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o ./build/$(TARGET)

$(OBJSDIR)%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJSDIR)

console:
	cd tools/tihu_console && qmake && make

play:
	$(CC) tools/tihu_play/main.cpp -o ./build/tihu_play -L. -ldl

test:
	$(CC) -std=c++17 -DDEBUG  -g test/main.cpp -o  ./build/tihu_test -L. -ldl -lstdc++fs
	./build/tihu_test ./build/libtihu.so ../test/res 2>/dev/null

# to print variables in makefile
print-%  : ; @echo $* = $($*)


.PHONY: all clean play console test