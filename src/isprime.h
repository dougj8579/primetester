/* isprime.h
 *  
 * Doug Jones, May 24, 2017
 *  
 * Exposes a public API for testing primality of unsigned integers.
 */

#include<stdio.h>

class AlgorithmFactory;

class PrimalityTest
{
public:
    /*
     * Test primality of an unsigned integer
     * 
     * returns:
     * 0 not prime
     * 1 prime
     * -1 failed to determine primality, e.g. due to invalid integer value
     */
    int isPrime(std::string &val);

    PrimalityTest();
    ~PrimalityTest();

private:
    AlgorithmFactory *factory_;
};


