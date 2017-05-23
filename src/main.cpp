#include<iostream>
#include<stdio.h>
#include<pistache/endpoint.h>
#include<pistache/router.h>
#include "isprime.h"

using namespace Net;
using namespace std;


class HelloHandler : public Http::Handler {
public:

    HTTP_PROTOTYPE(HelloHandler)

    void onRequest(const Http::Request& request, Http::ResponseWriter response) {
         response.send(Http::Code::Ok, "Hello, World");
    }

};

void isPrimeREST(const Rest::Request &request, Http::ResponseWriter response)
{
    auto val = request.param(":val").as<unsigned long long int>();
    if(isPrime(val))
    {
        response.send(Http::Code::Ok, "true");
    }
    else
    {
        response.send(Http::Code::Ok, "false");
    }
}

int main()
{
    int port = 9080;
    cout << "\nprimetest service is running on port " << port << "...\n";
    Net::Address addr(Net::Ipv4::any(), Net::Port(port));

    Rest::Router router;
    Rest::Routes::Post(router, "/isprime/:val", Rest::Routes::bind(&isPrimeREST));

    auto opts = Http::Endpoint::options().threads(10);
    Http::Endpoint server(addr);
    server.init(opts);
    server.setHandler(router.handler());
    server.serve();

    return 0;
}
