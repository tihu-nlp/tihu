# Tihu, Persian Text-To-Speech  [![Build Status](https://travis-ci.org/tihu-nlp/tihu.svg?branch=master)](https://travis-ci.org/tihu-nlp/tihu)


**Tihu** is an open source Persian text-to-speech engine. It's a cross-platform application and mostly is written in C++. Tihu uses [Hazm](https://github.com/sobhe/hazm) for digesting Persian text and [Tihu g2p-seq2seq](https://github.com/tihu-nlp/tihudict) for Grapheme-to-Phoneme conversion.


## Compile the source

Please make sure you have installed gcc (6.0 or higher). To check your gcc version run `gcc --version`.

### Compiling Tihu library

You can compile Tihu library by following these steps:
```
git clone https://github.com/tihu-nlp/tihu.git
cd tihu

make ready
make
```

### Tihu console

*Tihu console* is a gui application that allows you to work with Tihu library. Before compiling *Tihu console* please make sure you have installed [qt](https://www.qt.io) framework. You can also find a pre-compiled version of *Tihu console* in release page for linux x64.


### Tihu gRPC
Make sure you have installed [gRPC C++](https://github.com/grpc/grpc/tree/master/src/cpp). Then type: `make grpc`

### docekr
To create Tihu docker with gRPC endpint: type `make docker`


## About the Name

Tihu is Persian name for Partridge.

