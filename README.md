# primetester
Implements a client/server application testing primality of an unsigned integer
over a RESTful web interface.  

Currently handles unsigned integers up to 2^64-1.


DEPENDENCIES:

Pistache - a modern HTTP and REST framework written in pure-C++11 http://pistache.io/  **requires patch  
rapidjson-dev - http://us.archive.ubuntu.com/ubuntu vivid main universe  
cmake  


SETUP:

1. Clone repos and apply patch to pistache:

git clone https://github.com/dougj8579/primetester.git  
git clone https://github.com/oktal/pistache.git  
cd pistache  
git apply ../primetester/pistache.patch  

2. Compile and install pistache:

git submodule update --init  
mkdir build && cd build  
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..  
make  
sudo make install  
sudo ldconfig  
cd ../..  

3. Compile primetest:

cd primetester/  
make  


LAUNCH & TEST:

~/cp/primetester $ ./primetest &  
~/cp/primetester $ ./test.sh testcases  


EXAMPLE:

1. from a local terminal do:

curl -X POST http://localhost:9080/isprime/4294967357

2. Generates a response string "true" indicating 4294967357 is prime.

