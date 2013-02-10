This library is available at <https://github.com/thejh/libbufio>.

License and Disclaimer
======================
Copyright (c) 2013 Jann Horn
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

What this library is good for
=============================
This C library provides buffered IO for nonblocking file descriptors and
uses libev. If you are already using libev and you want something that gives
you big write buffers and a way to read in fixed sizes from network
sockets, this might be it.
It works well for binary protocols with length prefixes, but is not appropriate
for text-based protocols.

Memory management
=================
If you didn't allocate something, don't try to free it. If you did allocate it,
you'll probably also have to free it. There's one exception: Output buffers will
be free'd via `free()` when they have been written or you tell the library to
clean up a connection. So, you don't own an output buffer anymore as soon as
you've given it to this library.

Matureness
==========
As of the time I wrote this text, I have only compiled the code, not tried it so far. Also, I'm not likely to use it in more than a few hobby projects at this time.
However, the code is not complicated at all and if you have patches for fixing bugs or adding features, I will probably accept them.

Compiling
=========
Run `make` to get `libbufio.so`.

Usage
=====
Generally, this library is not threadsafe for accesses to the same connection.
For everything else, see the header file.
