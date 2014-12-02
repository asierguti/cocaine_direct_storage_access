/*
 * 2014+ Copyright (c) Asier Gutierrez <asierguti@gmail.com>
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef DIRECT_ACCESS_SERVICE_HPP
#define DIRECT_ACCESS_SERVICE_HPP

#include <cocaine/api/service.hpp>

#include "idl/direct_access.hpp"
//#include <cocaine/dispatch.hpp>
#include <elliptics/session.hpp>

//#include <cocaine/locked_ptr.hpp>

#include <boost/asio/deadline_timer.hpp>

#include <boost/asio.hpp>

using namespace ioremap::elliptics;

namespace cocaine { namespace service {

    class direct_access_t:
    public api::service_t
    {
    public:
      direct_access_t(cocaine::context_t& context, cocaine::io::reactor_t& reactor, const std::string& name, const Json::Value& args);

    private:

      deferred<async_read_result> read_data(const std::string &id, const std::vector<int> &groups, uint64_t offset, uint64_t size);
      //      deferred<async_write_result> write_data(const key &id, const data_pointer &file, uint64_t remote_offset, uint64_t chunk_size);
      deferred<async_lookup_result> lookup(const std::string &id);

      void on_read_completed(deferred<async_read_result> promise,
			     const std::vector<ioremap::elliptics::read_result_entry> &result,
			     const ioremap::elliptics::error_info &error);//deferred<std::string> promise, const ioremap::elliptics::sync_read_result &result,
			     //const ioremap::elliptics::error_info &error);

      void init();


      /*      struct timer_desc_t {
	std::shared_ptr<boost::asio::deadline_timer> timer_;
	streamed<io::timer_id_t> promise_;
	double interval_;
	};

      streamed<io::timer_id_t>
      notify_after(double time, bool send_id);

      streamed<io::timer_id_t>
      notify_every(double time, bool send_id);

        void
        cancel(io::timer_id_t timer_id);

        void
        restart(io::timer_id_t timer_id);

        void
        on_timer(const boost::system::error_code& ec, io::timer_id_t timer_id);

        void
        remove_timer(io::timer_id_t timer_id);

    private:
      streamed<io::timer_id_t>
      set_timer_impl(double first, double repeat, bool send_id);

      std::unique_ptr<logging::log_t> log_;
      synchronized<std::map<io::timer_id_t, timer_desc_t>> timers_;
      boost::asio::io_service& asio_;*/

    private:
      boost::asio::io_service m_queue;

      ioremap::elliptics::node m_no;

      //      m_no.add_remote(address(m_remote_address, m_remote_port));


//      ioremap::elliptics::session m_session;

      //      boost::asio::io_service::work work(m_queue);
    };

  }} // namespace cocaine::service

#endif
