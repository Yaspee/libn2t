/*
 * This file is part of the libn2t project.
 * Libn2t is a C++ library transforming network layer into transport layer.
 * Copyright (C) 2018  GreaterFire
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "n2s.h"
#include <memory>
#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
using namespace std;
using namespace boost::asio;
using namespace boost::asio::posix;

namespace Net2Tr {
    class N2S::N2SInternal {
    public:
        io_service service;
        stream_descriptor fd;
        N2T &n2t;
        string socks5_addr;
        uint16_t socks5_port;
        steady_timer lwip_timer;

        N2SInternal(int tun_fd, N2T &n2t, const string &socks5_addr, uint16_t socks5_port) : fd(service, tun_fd), n2t(n2t), socks5_addr(socks5_addr), socks5_port(socks5_port), lwip_timer(service) {}

        void async_wait_timer()
        {
            lwip_timer.expires_from_now(boost::asio::chrono::milliseconds(250));
            lwip_timer.async_wait([this](const boost::system::error_code &error)
            {
                if (!error)
                    n2t.process_events();
                async_wait_timer();
            });
        }

        void async_read_fd()
        {
            fd.async_wait(stream_descriptor::wait_read, [this](const boost::system::error_code &error)
            {
                if (!error) {
                    char buf[2000];
                    int len = read(fd.native_handle(), buf, sizeof(buf));
                    n2t.input(string(buf, len));
                }
                async_read_fd();
            });
        }

        void async_output()
        {
            n2t.async_output([this](const string &packet)
            {
                write(fd.native_handle(), packet.c_str(), packet.size());
                async_output();
            });
        }

        void async_accept()
        {

        }
    };

    N2S::N2S(int tun_fd, N2T &n2t, const string &socks5_addr, uint16_t socks5_port)
    {
        internal = new N2SInternal(tun_fd, n2t, socks5_addr, socks5_port);
    }

    N2S::~N2S()
    {
        delete internal;
    }

    void N2S::start()
    {
        internal->async_wait_timer();
        internal->async_read_fd();
        internal->async_output();
        internal->async_accept();
        internal->service.run();
    }
}