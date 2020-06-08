# sentencepiece_java
A minimal Java wrapper for google sentencepiece library (https://github.com/google/sentencepiece)

Currenlty this support only decode methods. It is easy to add additional sentencepiece methods in the same way.

## Compile and test commands
```
mkdir build
cd build/
cmake ..
cmake --build .
java -Djava.library.path=. -DMODEL_PATH=<sentencepiece_model> -jar Test.jar
```
You can generate a sentencepiece model for testing following https://github.com/google/sentencepiece#end-to-end-example

