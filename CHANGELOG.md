# Changelog

## 2.1.0

- More than 30% improvement in final results
- Using [new model](https://github.com/tihu-nlp/tihudict) for Persian g2p-seq2seq prediction
- Detecting Persian Kaser Ezafe (Genitive detection)
- More compatible with Hazm tag-sets
- Using espeak-ng with farsi language
- Updating Persian affixes
- [Homograph disambiguation](./test/res/homographs.txt) based on their POS and weight, [examples]
- [Breaking](./test/res/decompose.txt) unknown words
- Simplifying lexicon results
- Dockerfile is added
- Adding more tests cases
- Updating lexicon
- Fixing some bugs and issues

## 2.0.0

- Add grpc_server  tools to run tihu on a server
- Update functions and callback
- Load all synthesizer (voices) at the beginning
- Update tools (console and play)
- Fix travis errors
- fix some issues on looking up word
- Using tihu tokenizer to pars the text
- A word can have more than one entry
- Make corpus module
- reorganize the folders
- Fix bugs and issues

## 1.0.0

Initial commit