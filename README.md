# primetester
Implements a client/server application testing primality of an unsigned integer
over a RESTful web interface.  

Currently handles unsigned integers up to 2^64-1 


DEPENDENCIES:

Pistache - a modern HTTP and REST framework written in pure-C++11 http://pistache.io/
rapidjson-dev - http://us.archive.ubuntu.com/ubuntu vivid main universe
cmake


SETUP:

~/cp $ git clone https://github.com/oktal/pistache.git
~/cp $ cd pistache
~/cp/pistache $ git submodule update --init
~/cp/pistache $ mkdir build && cd build
~/cp/pistache/build $ cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
~/cp/pistache/build $ make
~/cp/pistache/build $ sudo make install
~/cp/pistache/build $ sudo ldconfig
~/cp/pistache/build $ cd ../..

~/cp $ git clone https://github.com/dougj8579/primetester.git
~/cp $ cd primetester/
~/cp/primetester $ make


LAUNCH & TEST:

~/cp/primetester $ ./primetest &
~/cp/primetester $ ./test.sh testcases


EXAMPLE:

$ curl -X POST http://localhost:9080/isprime/4294967357

Generates a response string "true" indicating 4294967357 is prime.









