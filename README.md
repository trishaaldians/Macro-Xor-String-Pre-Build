# Macro-Xor-String-Pre-Build
Compile time string Xor for visual studio 2010-2019 (not sure).

# Share
https://www.unknowncheats.me/forum/c-and-c-/449137-macro-xor-string-pre-build-event.html

# Original Post by
https://github.com/stevemk14ebr/VS2013-Compile-Time-XOR

---------------------------

How to setup:
1. Go to Properties (your project) -> Configuration Properties -> Build Events -> Pre-Build Event
2. Add the Command Line: -> call "$(SolutionDir)\XorStr.exe"

How to use:
1. Add #include XorStr.h to your project
2. https://github.com/trishaaldians/Macro-Xor-String-Pre-Build/blob/main/Common/XorStr/XorStr.h
```C++
#include "../Common/XorStr/XorStr.h"
```

Example:
```C++
1. Just Add XOR("")
2. printf(XOR("my string"));
3. After Build -> printf(/*my string*/XorStr<0x29,10,0xE184BE23>("\x44\x53\x0B\x5F\x59\x5C\x46\x5E\x56"+0xE184BE23).decode);
```
