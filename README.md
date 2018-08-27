# Tihu, Persian Text-To-Speech  [![Build Status](https://travis-ci.org/tihu-nlp/tihu.svg?branch=master)](https://travis-ci.org/tihu-nlp/tihu)


[Tihu](https://github.com/tihu-nlp/tihu) is an open source Persian text-to-speech engine. It's a cross-platform application and mostly is written in C++. It uses [Tihu g2p-seq2seq](https://github.com/tihu-nlp/g2p-seq2seq-tihudict) for Grapheme-to-Phoneme conversion.


## Compile
Please make sure you have installed gcc (6.0 or higher). To check your gcc version run `gcc --version`.

You can compile Tihu by following these steps:
```
git clone https://github.com/tihu-nlp/tihu.git
cd tihu
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

## Example
You can check the Tihu's output for this sentence: `بحران آب حیات تمدن ایرانی را تهدید می‌کند.` (Water crisis endangers Iranian civilization).
The example folder contains digested text and audio outputs: mbrola-male.wav, mbrola-female.wav and espeak-male.wav

Please check Example folder for more information.

## Part-Of-Speech tags
Tihu has several tags for showing Part-Of-Speech. These tags are listed here with some explanations:

| Tag            | Description            |
|----------------|------------------------|
| N              | Noun                   |
| N_PR           | Proper noun            |
| ADJ            | Adjective              |
| ADJ_CMPR       | Comparative adjective  |
| ADJ_SUP        | Superlative adjective  |
| ADJ_INO        | Participle adjective   |
| ADV            | Adverb                 |
| PRO            | Pronoun                |
| P              | Preposition            |
| DET            | Determiner             |
| CON            | Conjunction            |
| NUM            | Numeral                |
| INT            | Interjection           |
| FW             | Foreign                |
| V              | Verb                   |
| V_AUX          | Auxiliary verb         |
| V_PP           | Participle verb        |
| V_SUB          | Subjective verb        |
| V_PRS          | Present verb           |
| V_PA           | Past verb              |
| V_IMP          | Imperative verb        |
| DEL            | Delimiter              |


## About the Name

Tihu is Persian name for Partridge.

