LIBNAME = tihu
TARGET=lib$(LIBNAME).so
CC = g++
OBJSDIR = obj/
SRCS = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp)
OBJS = $(addprefix $(OBJSDIR),$(patsubst %.cpp,%.o,$(SRCS)))
INC = `python2-config --includes` -Xlinker -export-dynamic
## for debugging
CXXFLAGS = -c -Wall -std=c++14 $(INC) -DDEBUG -g -fPIC
##CXXFLAGS = -c -fPIC -Wall -std=c++14 $(INC) -O3
LDFLAGS = `python2-config --ldflags` -lsamplerate -shared -Wl,-soname,$(TARGET)


all: deps build clean

deps:
	mkdir ./deps
	@echo === Installing Mbrola
	git clone https://github.com/numediart/MBROLA ./deps/MBROLA
	cd ./deps/MBROLA && make && cp Bin/mbrola ../../build/

	@echo === Installing g2p-seq2seq
	pip2 install tensorflow==1.9.0 --user
	pip2 install tensor2tensor==1.7.0 --user
	git clone https://github.com/tihu-nlp/g2p-seq2seq.git ./deps/g2p-seq2seq
	cd ./deps/g2p-seq2seq && python2 setup.py install --user

	@echo === Installing Hazm
	pip2 install --upgrade --user pip
	pip2 install six --upgrade --user
	pip2 install pip --upgrade --user
	pip2 install hazm --user

	@echo === Downloading Tihudict
	curl -Lo ./deps/g2p-seq2seq-tihudict.tar.gz https://github.com/tihu-nlp/tihudict/releases/download/v2.0/g2p-seq2seq-tihudict-model-2.0.tar.gz
	cd ./build/data && tar xf ../../deps/g2p-seq2seq-tihudict.tar.gz

	rm -rf ./deps

build: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o ./build/$(TARGET)

$(OBJSDIR)%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJSDIR)

console:
	cd tools/tihu_console && qmake && make

grpc:
	cd tools/grpc && make

play:
	$(CC) tools/tihu_play/main.cpp -o ./build/tihu_play -L. -ldl

test:
	$(CC) -std=c++17 -DDEBUG  -g test/main.cpp -o  ./build/tihu_test -L. -ldl -lstdc++fs
	./build/tihu_test ./build/libtihu.so ../test/res 2>/dev/null


docker:
	docker build . --tag tihu

# to print variables in makefile
print-%  : ; @echo $* = $($*)


.PHONY: all clean play console test tools
