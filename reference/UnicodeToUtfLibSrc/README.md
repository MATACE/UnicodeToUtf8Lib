# Unicode To Utf8 Transfer Lib Src

>NOTE: For C++ to use, if use to c project can use sqlsimpleApi.h offer UnicodeStrToUTF8Str function.

# How to build

## For x64 compile, please use command

```bash
# mkdir build
# cd build
# cmake ..
# make -j8
```

## For arm cross compile, please use command

>First set your pc arm cross compile tool

### Build platformat arm9 cmd

```bash
# mkdir build
# cd build
# cmake -DPLATFORM=arm -DDEVICE=arm9 ..
# make -j8
```

### Build platformat a8 cmd

```bash
# mkdir build
# cd build
# cmake -DPLATFORM=arm -DDEVICE=a8 ..
# make -j8
```