#include<climits>
#include<iostream>
#include<stdio.h>

using namespace std;

/* computes x^y modulo m in at most 32 iterations
 * 
 * Relies on modular rules of multiplication:
 * n^2 mod m = (n * n) mod m = ((n mod m) * (n mod m)) mod m
 */ 
static unsigned long long int pwr(unsigned int x, unsigned int y, unsigned int m)
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

/*
 * Basic prime tests (non-deterministic)
 * 
 * returns:
 * 0 not prime
 * 1 prime
 * -1 possibly prime
 */
static int isPrimeBasic(unsigned long long int n)
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
    
    // possibly prime
    ret = -1;

done:
    return ret;
}

/* Optimized trial division (brute force). A deterministic test, but slower for large
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
static int isPrimeTrialDiv(unsigned long long int n)
{
    int ret = 0;
    unsigned long long int k;
    for(k = 1; 36 * k * k - 12 * k < n; ++k)
    {
        if((n % (6 * k + 1) == 0) || (n % (6 * k - 1) == 0))
        {
            // must be non-prime
            ret = 0;
            goto done;
        }
    }

    // must be prime
    ret = 1;
done:
    cout << "TrialDivision"; 
    if(n > UINT_MAX) cout << "(n>UINT_MAX)"; 
    cout << "\n" << flush; 
    return ret;
}

/*
 * Represents a single pass of Miller-Rubin
 */
static unsigned int witnessMR(unsigned int a, unsigned int u, unsigned int t, unsigned int n)
{
    unsigned int i;
    unsigned long long int x, y;
    x = pwr(n,t,a);
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

/*
 * A Deterministic form of Miller-Rabin
 * 
 * returns:
 * 0 not prime
 * 1 prime
 */
static int isPrimeMR(unsigned int n)
{
    int ret = 0;
    unsigned int d, s;
    d = n / 2;
    s = 1;
    while( !(d & 1) )
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

/* API */
bool isPrime(unsigned long long int n)
{
    int ret = 0;

    cout << "val=" << n << " " << flush;
    ret = isPrimeBasic(n); 
    if(ret>=0)
    {
        goto done;
    }

    if( (n < 1373653) || (n > UINT_MAX) )
    {
        // faster than MR for n < 1373653
        // also used for n > UINT_MAX, i.e. when n exceeds isPrimeMR() max
        ret = isPrimeTrialDiv(n); 
        if(ret>=0)
        {
            goto done; 
        }
    }

    // deterministic for n < sizeof(unsigned int)
    ret = isPrimeMR(n); 
done:
    return ret > 0;
}
