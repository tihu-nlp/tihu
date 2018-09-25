LIBNAME = tihu
TARGET=lib$(LIBNAME).so
CC = g++
SRCS = $(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp)
OBJS = $(addprefix obj/,$(patsubst %.cpp,%.o,$(SRCS)))
OBJDIRS = $(dir $(OBJS))
INC = `python2.7-config --includes` -I. -Xlinker -export-dynamic
## for debugging
#CXXFLAGS = -c -Wall -std=c++14 $(INC) -DDEBUG -g -fPIC
CXXFLAGS = -c -fPIC -Wall -std=c++14 $(INC) -O3 -g
LDFLAGS = `python2.7-config --ldflags` -shared -Wl,-soname,$(TARGET) -o $(TARGET)
LIBS =

.PHONY: all
all: $(TARGET) install

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

obj/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf ./obj

install:
	cp $(TARGET) ../build/

# to print variables in makefile
print-%  : ; @echo $* = $($*)