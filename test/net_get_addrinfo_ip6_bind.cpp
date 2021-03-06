//
//  Author  : github.com/luncliff (luncliff@gmail.com)
//  License : CC BY 4.0
//
#include "socket.h"
#include <coroutine/net.h>

#include "test.h"
using namespace std;
using namespace coro;

auto net_getaddrinfo_ip6_bind_test() {
    init_network_api();
    auto on_return = gsl::finally([]() { release_network_api(); });

    addrinfo hint{};
    hint.ai_family = AF_INET6;
    hint.ai_socktype = SOCK_RAW;
    hint.ai_flags = AI_ALL | AI_V4MAPPED | AI_NUMERICHOST | AI_NUMERICSERV;

    size_t count = 0u;
    // since this is ipv6, ignore port(service) number
    for (sockaddr& ep : resolve(hint, "::0.0.0.0", nullptr)) {
        _require_(ep.sa_family == AF_INET6);

        const auto* in6 = reinterpret_cast<sockaddr_in6*>(addressof(ep));
        bool unspec = IN6_IS_ADDR_UNSPECIFIED(addressof(in6->sin6_addr));
        _require_(unspec);
        ++count;
    }
    _require_(count > 0);
    return EXIT_SUCCESS;
}

#if defined(CMAKE_TEST)
int main(int, char* []) {
    return net_getaddrinfo_ip6_bind_test();
}

#elif __has_include(<CppUnitTest.h>)
#include <CppUnitTest.h>

template <typename T>
using TestClass = ::Microsoft::VisualStudio::CppUnitTestFramework::TestClass<T>;

class net_getaddrinfo_ip6_bind : public TestClass<net_getaddrinfo_ip6_bind> {
    TEST_METHOD(test_net_getaddrinfo_ip6_bind) {
        net_getaddrinfo_ip6_bind_test();
    }
};
#endif
