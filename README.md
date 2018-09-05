# Tihu, Persian Text-To-Speech  [![Build Status](https://travis-ci.org/tihu-nlp/tihu.svg?branch=master)](https://travis-ci.org/tihu-nlp/tihu)


**Tihu** is an open source Persian text-to-speech engine. It's a cross-platform application and mostly is written in C++. Tihu uses [Hazm](https://github.com/sobhe/hazm) for digesting Persian text and [Tihu g2p-seq2seq](https://github.com/tihu-nlp/g2p-seq2seq-tihudict) for Grapheme-to-Phoneme conversion.


## Compile
Please make sure you have installed gcc (6.0 or higher). To check your gcc version run `gcc --version`.

You can compile Tihu by following these steps:
```
git clone https://github.com/tihu-nlp/tihu.git
cd tihu

# install hazm
pip install hazm

# espeak-data
curl -sLO http://sourceforge.net/projects/espeak/files/espeak/espeak-1.48/espeak-1.48.04-source.zip
unzip espeak-1.48.04-source.zip
cp -r espeak-1.48.04-source/espeak-data src/build/data/
rm espeak-1.48.04-source.zip
rm -rf espeak-1.48.04-source

# compiling code
cd src/tihu
make
make test
make install
```

**Tihu console**

*Tihu console* is a gui application that allows you to work with Tihu library. Before compiling *Tihu console* please make sure you have installed [qt](https://www.qt.io) framework. You can also find a pre-compiled version of *Tihu console* in release page for linux x64.


Note: Windows users can compile Tihu by their own risks.

## Live demo
<<TBD>>


## About the Name

Tihu is Persian name for Partridge.

