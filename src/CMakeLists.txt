add_library(n2t
    n2t/n2s.cpp
    n2t/n2t.cpp
    n2t/socket.cpp
    n2t/tcpsession.cpp
    n2t/udpsession.cpp
    n2t/utils.cpp
    sys_arch.c
    lwip/src/core/tcp_out.c
    lwip/src/core/tcp_in.c
    lwip/src/core/memp.c
    lwip/src/core/netif.c
    lwip/src/core/ipv6/ip6_frag.c
    lwip/src/core/ipv6/ip6.c
    lwip/src/core/ipv6/icmp6.c
    lwip/src/core/ipv6/ip6_addr.c
    lwip/src/core/ipv6/mld6.c
    lwip/src/core/ipv6/nd6.c
    lwip/src/core/timeouts.c
    lwip/src/core/tcp.c
    lwip/src/core/ip.c
    lwip/src/core/udp.c
    lwip/src/core/mem.c
    lwip/src/core/def.c
    lwip/src/core/init.c
    lwip/src/core/pbuf.c
    lwip/src/core/ipv4/icmp.c
    lwip/src/core/ipv4/ip4.c
    lwip/src/core/ipv4/ip4_addr.c
    lwip/src/core/inet_chksum.c)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/lwip/src/include
)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(n2t ${CMAKE_THREAD_LIBS_INIT})
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.54.0 REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(n2t ${Boost_LIBRARIES})
