/* main.cpp
 *  
 * Doug Jones, May 24, 2017
 *  
 * Implements a client/server application testing primality of an unsigned integer over a
 * RESTful web interface. Currently handles unsigned integers up to 2^64 - 1 in size.
 * 
 * Exposes a RESTful API of the form http://<hostname>:<port>/isprime/<unsigned integer>
 * using the HTTP POST method, and responds with "true" if the given integer is prime,
 * "false" if not prime.  The server supports multiple simultaneous requests and
 * concurrent calculations.
 */

#include<iostream>
#include<stdio.h>
#include<pistache/endpoint.h> // 3rd-party REST library
#include<pistache/router.h> // 3rd-party REST library
#include "isprime.h" // custom API for accessing the primality test algorthims

/*
 * Primetester service configuration options 
 */

#define PSRV_LOCAL_PORT 9080 // local TCP port the server will listen on
#define PSRV_NUM_CONCURRENT_THREADS 10 // all threads are launched upon init, then handle incoming requests in round-robin fashion


using namespace Net;

static PrimalityTest test;

void restServiceHandler(const Rest::Request &request, Http::ResponseWriter response)
{
    std::string val = request.param(":val").value();
    switch(test.isPrime(val))
    {
        case 0:
            response.send(Http::Code::Ok, "false");
            break;
        case 1:
            response.send(Http::Code::Ok, "true");
            break;
        default:
            response.send(Http::Code::Bad_Request);
            break;
    }
}

int main()
{
    std::cout << "\nprimetest service is running on port " << PSRV_LOCAL_PORT << "..." << std::endl;

    Net::Address addr(Net::Ipv4::any(), Net::Port(PSRV_LOCAL_PORT));

    Rest::Router router;
    Rest::Routes::Post(router, "/isprime/:val", Rest::Routes::bind(&restServiceHandler));

    auto opts = Http::Endpoint::options().threads(PSRV_NUM_CONCURRENT_THREADS);
    Http::Endpoint server(addr);
    server.init(opts);
    server.setHandler(router.handler());
    server.serve();

    return 0;
}
