# HTTP Server

Small, portable & multithreaded HTTP server in C++.

## PSocket - Multi Platform Socket API

PSocket is an internal API I made to make this server as platform agnostic as possible. It's heavily inspired by and modeled after [TinyCSocket](https://tinycsocket.readthedocs.io/en/latest/), which is likely superior to my library in every way. Knowing this, if you'd still like me to make a separate repository so you can use it in your projects, please contact me at kyle.oestreich06@gmail.com.

You can read the docs for PSocket at [PSOCKET.md](https://github.com/kystreich/http-server/blob/psocket-rework/PSOCKET.md).

> [!NOTE]
> `./makefiles.sh` is for my windows machine specifically - it is not portable

>[!NOTE]
> `./CmakeLists.txt` has a definition for the location of my OpenSSL library - this might not be the same for you. You may be able to just remove the definition entirely if CMake is able to locate OpenSSL for you with default behavior.

> [!CAUTION]
> This project is almost certainly vulnerability riddled and should not be used in productions. 

## Cool Resources
[The World Of Select - Spencer Low](https://www.lowtek.com/sockets/select.html)

[The Bible, Beej's Guide to Network Programming - Brian Hall](https://beej.us/guide/bgnet/)

[C++ Patterns - Joseph Mansfield](https://cpppatterns.com/)

[C/C++ By Example - Sean Valeo](https://www.cbyexample.com/)

[Understanding Blocking and Non-Blocking Sockets in C Programming: A Comprehensive Guide - Vivek Yadav](https://dev.to/vivekyadav200988/understanding-blocking-and-non-blocking-sockets-in-c-programming-a-comprehensive-guide-2ien)


