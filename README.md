# Tihu, Persian Text-To-Speech


[Tihu](https://github.com/tihu-nlp/tihu) is an open source Persian text-to-speech engine. It's a cross-platform application and  written in C++. 

#### !Warning for windows users
Letter-To-Sound module doesn't work on windows yet. It's because [tensorflow](http://tensorflow.org) is not available for Windows OS right now. It is supposed to be working on further releases.

## build requirements
Before compiling tihu you need to make sure you have installed following packages:

### Qt
for running tihu_console you need to install Qt framework. Find latest Qt framework and Qt Creator [here](https://www.qt.io/).

## Compiling
To compile tihu on linux operation system run 'make' on tihu folder.

Windows users can compile tihu by visual studio.

##Part-Of-Speech tags
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

