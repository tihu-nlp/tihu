LIBNAME = tihu
TARGET=lib$(LIBNAME).so
TARGET_TEST=$(LIBNAME)_test
CC = g++

INC =  `python2.7-config --includes` -I. -Xlinker -export-dynamic
CXXFLAGS = -c -fPIC -Wall -std=c++14 $(INC) -O3 -g
## for debugging
##CXXFLAGS = -c -Wall -std=c++14 $(INC) -DDEBUG -g -fPIC
LDFLAGS =  `python2.7-config --ldflags` -shared -Wl,-soname,$(TARGET) -o $(TARGET)
LIBS =
RM = rm -f

SRCS= \
	ipa_transcript/tihu_dict/afx_manager.cpp \
	ipa_transcript/tihu_dict/afx_entry.cpp \
	ipa_transcript/tihu_dict/hash_manager.cpp \
	ipa_transcript/tihu_dict/pfx_entry.cpp \
	ipa_transcript/tihu_dict/sfx_entry.cpp \
	ipa_transcript/tihu_dict/tihu_dict.cpp \
	ipa_transcript/letter_to_sound/persian_to_phoneme.cpp \
    ipa_transcript/letter_to_sound/english_to_phoneme.cpp \
    ipa_transcript/letter_to_sound/punctuation_to_phoneme.cpp \
    ipa_transcript/letter_to_sound/number_to_phoneme.cpp \
    ipa_transcript/letter_to_sound/letter_to_sound.cpp \
    ipa_transcript/letter_to_sound/g2p_seq2seq_linux.cpp \
	synthesis/espeak/espeak_lib.cpp \
	synthesis/espeak/espeak_syn.cpp \
	synthesis/mbrola/mbrola_lib.cpp \
	synthesis/mbrola/mbrola_syn.cpp \
	synthesis/mbrola/mbrowrap.cpp \
    hazm/hazm.cpp \
    text_tagger.cpp \
	corpus.cpp \
	event.cpp \
    phoneme.cpp \
	pitch_range.cpp \
	word.cpp \
	tihu.cpp \
    parser.cpp \
    engine.cpp \
	settings.cpp \
	synthesizer.cpp \
	helper.cpp \
	hunzip.cpp \
	file_manager.cpp

OBJS = $(SRCS:.cpp=.o)


.PHONY: all
all: $(TARGET) install

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

.cpp.o:
	$(CC) $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	-$(RM) $(TARGET) $(OBJS) $(TARGET_TEST)


install:
	cp $(TARGET) ../build/

.PHONY: test
test: $(TARGET_TEST)
	./$(TARGET_TEST) ../build/$(TARGET)

$(TARGET_TEST): $(TARGET)
	$(CC) $(TARGET_TEST).cpp -o $@ -L. -ldl
