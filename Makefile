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


all: deps build

deps:
	@echo === Installing espeak-ng
	sudo apt-get install libespeak-ng-dev -y
	@echo === Downloading libsamplerate
	sudo apt-get install libsamplerate0-dev -y
	@echo === Installing Mbrola
	rm -rf /tmp/MBROLA
	git clone https://github.com/numediart/MBROLA /tmp/MBROLA
	cd /tmp/MBROLA && make
	cp /tmp/MBROLA/Bin/mbrola ./build/
	@echo === Installing g2p-seq2seq
	pip install tensorflow==1.9.0 --user
	pip install tensor2tensor==1.7.0 --user
	rm -rf /tmp/g2p-seq2seq
	git clone https://github.com/tihu-nlp/g2p-seq2seq.git /tmp/g2p-seq2seq
	cd /tmp/g2p-seq2seq && python setup.py install --user
	@echo === Installing Hazm
	sudo pip install --upgrade pip
	pip install six --upgrade --user
	pip install pip --upgrade --user
	pip install hazm --user
	@echo === Downloading Tihudict
	wget -O /tmp/g2p-seq2seq-tihudict.tar.gz https://github.com/tihu-nlp/tihudict/releases/download/v2.0/g2p-seq2seq-tihudict-model-2.0.tar.gz
	cd ./build/data && tar xf /tmp/g2p-seq2seq-tihudict.tar.gz

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

play:
	$(CC) tools/tihu_play/main.cpp -o ./build/tihu_play -L. -ldl

test:
	$(CC) -std=c++17 -DDEBUG  -g test/main.cpp -o  ./build/tihu_test -L. -ldl -lstdc++fs
	./build/tihu_test ./build/libtihu.so ../test/res 2>/dev/null

# to print variables in makefile
print-%  : ; @echo $* = $($*)


.PHONY: all clean play console test tools