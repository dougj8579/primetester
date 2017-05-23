all: primetest

primetest:  src/main.cpp src/isprime.cpp
	g++ ./src/main.cpp ./src/isprime.cpp -std=gnu++11 -lnet -o primetest

clean:
	rm primetest


