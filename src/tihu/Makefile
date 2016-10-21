LIBNAME = tihu
TARGET=lib$(LIBNAME).so
TARGET_TEST=$(LIBNAME)_test
CC = g++
INC = .
CXXFLAGS = -c -m32 -Wall -std=c++14 -I$(INC) -O3 -DDEBUG -g
LDFLAGS = -shared -m32 -Wl,-soname,$(TARGET) -o $(TARGET)
LIBS = 
RM = rm -f

SRCS= \
	tokenization/tokenizer/char_map.cpp \
	tokenization/tokenizer/char_map_row.cpp \
	tokenization/tokenizer/char_mapper.cpp \
	tokenization/tokenizer/tokenizer.cpp \
	morphology/pos_tagger/afx_manager.cpp \
	morphology/pos_tagger/afx_entry.cpp \
	morphology/pos_tagger/hash_manager.cpp \
	morphology/pos_tagger/pfx_entry.cpp \
	morphology/pos_tagger/sfx_entry.cpp \
	morphology/pos_tagger/pos_tagger.cpp \
	morphology/pos_disambiguator/pos_disambiguator.cpp \
	correction/text_tagger/text_tagger.cpp \
	diacritization/letter_to_sound/persian_to_phoneme.cpp \
    diacritization/letter_to_sound/english_to_phoneme.cpp \
    diacritization/letter_to_sound/punctuation_to_phoneme.cpp \
    diacritization/letter_to_sound/number_to_phoneme.cpp \
    diacritization/letter_to_sound/letter_to_sound.cpp \
    diacritization/letter_to_sound/g2p_seq2seq_linux.cpp \
	synthesis/espeak/espeak_lib.cpp \
	synthesis/espeak/espeak_syn.cpp \
	synthesis/mbrola/mbrola_lib.cpp \
	synthesis/mbrola/mbrola_syn.cpp \
	synthesis/mbrola/mbrowrap.cpp \
	corpus.cpp \
	entry.cpp \
	event.cpp \
    phoneme.cpp \
	pitch_range.cpp \
	word.cpp \
	tihu.cpp \
    parser.cpp \
    engine.cpp \
	path_manager.cpp \
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
