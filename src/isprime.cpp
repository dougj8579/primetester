/* isprime.cpp
 *  
 * Doug Jones, May 24, 2017
 *  
 * Implements a primality test for unsigned integers.
 */

#include<climits>
#include<string>
#include<vector>
#include<iostream>
#include<stdio.h>
#include "isprime.h"

using namespace std;

/* 
 * class IPrimalityAlgorithm
 * 
 * Abstract base class from which all concrete algorithms are derived. The isPrime()
 * method must be overriden by derived classes.
 */
class IPrimalityAlgorithm
{
public:
    /*
     * Test primality of an unsigned integer (may be non-deterministic) 
     * 
     * returns:
     * 0 not prime
     * 1 prime
     * -1 possibly prime
     */
    virtual int isPrime(unsigned long long n) = 0; // pure virtual

}; 

/* 
 * class IPrimalityAlgorithmWrapper
 * 
 * Abstract base class from which all concrete *non-deterministic* algorithms are derived.
 * Each concrete class will override the isPrime() method to enhance overall performance
 * of a composite algorithm.  Implements the decorator/wrapper design pattern (i.e. both
 * "has a" and "is a" base IPrimalityAlgorithm).
 */
class IPrimalityAlgorithmWrapper: public IPrimalityAlgorithm
{
public:
    IPrimalityAlgorithm *wrap(IPrimalityAlgorithm *alg)
    {
        wrappedAlgorithm_ = alg;
        return this;
    }
 
    int isPrime(unsigned long long n) // virtual
    {
        if(wrappedAlgorithm_)
        {
            return wrappedAlgorithm_->isPrime(n);
        }
         else
        {
            return -1;
        }
    }

protected:
    IPrimalityAlgorithm *wrappedAlgorithm_; 
};


/**********************************************************************************
 * Non-deterministic algorithims
 * 
 * These algorithims serve as decorators/wrappers to extend the functionality of a 
 * deterministic but slower "catch-all" algorithm. The composite algorithm achieves
 * overall performance than the core algorithm alone while still guaranteeing a
 * deterministic result.
 */

/* 
 * class BasicFastAlgorithm
 * 
 * Implements a basic "very fast" but non-deterministic prime test.
 */
class BasicFastAlgorithm : public IPrimalityAlgorithmWrapper
{
public:
    /*
     * A basic "very fast" prime test (non-deterministic)
     *
     * returns:
     * 0 not prime
     * 1 prime
     * -1 possibly prime
     */
    int isPrime(unsigned long long n) // virtual
    {
        int ret = 0;

        // even (excluding n=2) 
        if((!(n & 1)) && (n != 2))
        {
            cout << "Basic(even)\n" << flush;
            goto done;
        }

        // n = 0 or 1
        if(n < 2)
        {
            cout << "Basic(0 or 1)\n" << flush;
            goto done;
        }

        // divides by 3 (excluding n=3)
        if( (!(n % 3)) && (n != 3) )
        {
            cout << "Basic(divides by 3)\n" << flush;
            goto done;
        }
        
        /* Still possibly prime, delegate to wrapped algorithm */
        ret = IPrimalityAlgorithmWrapper::isPrime(n); 

    done:
        return ret;
    }

};

/**********************************************************************************
 * Deterministic algorithims
 * 
 * One of the following algorithims will function as the core of each composite algorithm,
 * selected to achieve best (e.g. fastest) results for a given integer under test.
 */

/* 
 * class TrialDivisionAlgorithm
 * 
 * Implements an optimized form of trial division with deterministic results.
 */
class TrialDivisionAlgorithm : public IPrimalityAlgorithm
{
public:

    /* *Optimized* trial division (brute force). A deterministic test, but slower for large
     * numbers.
     * 
     * All prime nums except 2 and 3 can be represented as 6k+1 or 6k-1, k = int >0. This
     * method tests all nums less than the square root of given number, to determine if they
     * are of this form.
     * 
     * returns:
     * 0 not prime
     * 1 prime
     */
    int isPrime(unsigned long long n)
    {
        int ret = 0;
        unsigned long long k;
        for(k = 1; 36 * k * k - 12 * k < n; ++k)
        {
            if((n % (6 * k + 1) == 0) || (n % (6 * k - 1) == 0))
            {
                // non-prime
                ret = 0;
                goto done;
            }
        }

        // prime
        ret = 1;
    done:
        cout << "TrialDivision"; 
        if(n > UINT_MAX) cout << "(n>UINT_MAX)"; 
        cout << "\n" << flush; 
        return ret;
    }

}; 

/* 
 * class MillerRabinAlgorithm
 * 
 * Implements a deterministic form of Miller-Rabin.
 */
class MillerRabinAlgorithm : public IPrimalityAlgorithm
{
public:

    /*
     * A Deterministic form of Miller-Rabin for integers < 4,759,123,141
     * 
     * returns:
     * 0 not prime
     * 1 prime
     * -1 failed to determine primality, e.g. due to invalid integer value
     */
    int isPrime(unsigned long long n)
    {
        int ret = 0;

        if(n > UINT_MAX)
        {
            ret = -1; // n out of range for this algorithm
            goto done;
        }

        unsigned int d, s;
        d = n / 2;
        s = 1;
        while(!(d & 1))
        {
            d /= 2;
            s++;
        }
        if(n < 1373653)
        {
            ret = witnessMR(n, s, d, 2) &&
                witnessMR(n, s, d, 3);
            goto done;
        }
        if(n < 9080191)
        {
            ret = witnessMR(n, s, d, 31) &&
                witnessMR(n, s, d, 73);
            goto done;
        }
        // n < 4,759,123,141
        ret = witnessMR(n, s, d, 2) &&
            witnessMR(n, s, d, 7) &&
            witnessMR(n, s, d, 61);

    done:
        cout << "Miller-Rabin\n" << flush;
        return ret;
    }

private:

    /*
     * Represents a single pass of Miller-Rabin
     */
    unsigned int witnessMR(unsigned int a, unsigned int u, unsigned int t, unsigned int n)
    {
        unsigned int i;
        unsigned long long x, y;
        x = pwr(n, t, a);
        for(i = 1; i <= u; ++i)
        {
            y = (x * x) % a;
            if((y == 1) && (x != 1) && (x != a - 1))
            {
                return 0;
            }
            x = y;
        }
        if(y != 1)
        {
            return 0;
        }
        return 1;
    }

    /* computes x^y modulo m in at most 32 iterations
     * 
     * Relies on modular rules of multiplication:
     * n^2 mod m = (n * n) mod m = ((n mod m) * (n mod m)) mod m
     */
    unsigned long long int pwr(unsigned int x, unsigned int y, unsigned int m)
    {
        unsigned long long int ret = 1;
        unsigned long long int p = x;

        while(y)
        {
            if(y & 1)
            {
                ret = (ret * p) % m;
            }
            p = (p * p) % m;
            y >>= 1;
        }
        return ret;
    }

};

/* 
 * class AlgorithmFactory
 * 
 * Responsible for composing the best (e.g. fastest) algorithm for a given integer.
 */
class AlgorithmFactory
{
    #define PRIMETEST_MAX_ALGORITHIMS 20
    enum AlgIdx { MillerRabin = 0, TrialDivision = 1,  BasicFast = 2 }; // enumerate any new algorithms here

public:
    IPrimalityAlgorithm *composeAlgorithm(unsigned long long n)
    {
        IPrimalityAlgorithm *compositeAlgorithm;

        /* 
         * Start with a deterministic algorithim (i.e. the "catch all" algorithm)
         */

        /* Optimized trial division is faster than Miller-Rabin for smaller ints. We also use
           it for ints too large for the existing Miller-Rabin algorithim */
        if((n < 1373653) || (n > UINT_MAX))
        {
            compositeAlgorithm = algorithms_[AlgIdx::TrialDivision];
        }
        /* otherwise finish with Miller-Rabin */
        else 
        {
            compositeAlgorithm = algorithms_[AlgIdx::MillerRabin];
        }

        /* 
         * Wrap with any additional non-deterministic algorithims here for performance tuning
         */
        

        /* As outermost layer, wrap with non-deterministic BasicFast algorithm (this will be
           first algorithm of the composite to be invoked) */
        compositeAlgorithm = dynamic_cast<IPrimalityAlgorithmWrapper*>(algorithms_[BasicFast])->wrap(compositeAlgorithm); 

        return compositeAlgorithm;
    }

    AlgorithmFactory() // constructor
    {
        // instantiate the desired algorithims for our tool bag
        algorithms_[AlgIdx::MillerRabin] = new MillerRabinAlgorithm;
        algorithms_[AlgIdx::TrialDivision] = new TrialDivisionAlgorithm;
        algorithms_[AlgIdx::BasicFast] = new BasicFastAlgorithm;
    }

    ~AlgorithmFactory() // destructor
    {
        for(int i = 0; algorithms_[i]; i++) 
        {
            delete algorithms_[i]; 
        }
    }

private:
    IPrimalityAlgorithm *algorithms_[PRIMETEST_MAX_ALGORITHIMS] = { 0 }; 
}; 


/********************************************************************************** 
 * 
 * Member function definitions for the public PrimalityTest class
 * 
 */

PrimalityTest::PrimalityTest() // constructor
{
    factory_ = new AlgorithmFactory;
}

PrimalityTest::~PrimalityTest() // destructor
{
    delete factory_;
}

int PrimalityTest::isPrime(std::string &val)
{
    unsigned long long n; // TODO convert this to a bigint 
    n = stoull(val);

    cout << "val=" << n << " " << flush;

    IPrimalityAlgorithm *algorithm = factory_->composeAlgorithm(n);
    if(algorithm == NULL) return -1; // failed to compose algorithm (unlikely)

    return algorithm->isPrime(n); 
}

