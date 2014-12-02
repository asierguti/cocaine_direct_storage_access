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

#include "cocaine/direct_access.hpp"

#include <memory>

#include <cocaine/context.hpp>
#include <cocaine/logging.hpp>

#include <cocaine/traits/tuple.hpp>

using namespace cocaine;
using namespace cocaine::io;
using namespace cocaine::service;

using namespace std::placeholders;

using namespace ioremap::elliptics;

direct_access_t::direct_access_t(cocaine::context_t& context, cocaine::io::reactor_t& reactor, const std::string& name, const Json::Value& args):
  service_t(context, reactor, name, args)
{
  on<cocaine::io::direct_access::read_data>("read_data", std::bind(&direct_access_t::read_data, this, _1, _2, _3, _4));
  on<cocaine::io::direct_access::lookup>("lookup", std::bind(&direct_access_t::lookup, this, _1));
  //  on<cocaine::io::direct_access::write_data>("write_data", std::bind(&direct_access_t::write_data, this, _1, _2, _3, _4));

  /*  on<io::chrono::notify_after>(std::bind(&chroOAno_t::notify_after, this, _1, _2));
    on<io::chrono::notify_every>(std::bind(&chrono_t::notify_every, this, _1, _2));
  on<io::chrono::cancel>(std::bind(&chrono_t::cancel, this, _1));
  on<io::chrono::restart>(std::bind(&chrono_t::restart, this, _1));*/

  //  m_no.add_remote(address("127.0.0.1", 1025));

  init();
}

void direct_access_t::init() {

}

deferred<async_read_result> direct_access_t::read_data(const std::string &id, const std::vector<int> &groups, uint64_t offset, uint64_t size)//const key &id
{
  //  deferred<std::string> promise;
  deferred<async_read_result> promise;

  ioremap::elliptics::session sess (m_no);

  async_read_result r = sess.read_data (id, groups, offset, size);

  //  r.connect(std::bind(&direct_access_t::on_read_completed, this, promise, _1, _2));
  return promise;
}

/*deferred<async_write_result> direct_access_t::write_data(const key &id, const data_pointer &file, uint64_t remote_offset, uint64_t chunk_size)
{
  deferred<async_write_result> promise;

  ioremap::elliptics::session sess (m_no);

  async_write_result r = sess.write_data (id, file, remote_offset, chunk_size);

  // r.connect(std::bind(&direct_access_t::on_read_completed, this, promise, _1, _2));
  return promise;

  }*/

deferred<async_lookup_result> direct_access_t::lookup(const std::string &id)
{

  deferred<async_lookup_result> promise;

  ioremap::elliptics::session sess (m_no);

  //  async_lookup_result r = sess.lookup(id);

  // r.connect(std::bind(&direct_access_t::on_read_completed, this, promise, _1, _2));
  return promise;
}

void direct_access_t::on_read_completed(deferred<async_read_result> promise,
					const std::vector<ioremap::elliptics::read_result_entry> &result,
					const ioremap::elliptics::error_info &error)
{
  if (error) {
    promise.abort(-error.code(), error.message());
  }/* else {
    promise.write(result[0].file().to_string());
    }*/
}

/*cocaine::streamed<io::timer_id_t>
chrono_t::notify_after(double time, bool send_id) {
  return set_timer_impl(time, 0.0, send_id);
}

cocaine::streamed<io::timer_id_t>
chrono_t::notify_every(double time, bool send_id) {
  return set_timer_impl(time, time, send_id);
}

cocaine::streamed<io::timer_id_t>
chrono_t::set_timer_impl(double first, double repeat, bool send_id) {
  streamed<io::timer_id_t> promise;
  std::shared_ptr<boost::asio::deadline_timer> timer(new boost::asio::deadline_timer(asio_));

  timer_desc_t desc;
  desc.timer_ = timer;
  desc.interval_ = repeat;

  io::timer_id_t timer_id;

  auto ptr = timers_.synchronize();

  do {
    timer_id = std::rand();
  } while (ptr->find(timer_id) != ptr->end());

  try {
    ptr->insert(std::make_pair(timer_id, desc));

    desc.timer_->expires_from_now(boost::posix_time::seconds(first));
    desc.timer_->async_wait(std::bind(&chrono_t::on_timer, this, std::placeholders::_1, timer_id));

    if (send_id) {
      desc.promise_.write(timer_id);
    }
  } catch (std::exception& ex) {
    remove_timer(timer_id);
    throw;
  }

  return desc.promise_;
}

void
chrono_t::cancel(io::timer_id_t timer_id) {
  auto ptr = timers_.synchronize();

  if (ptr->find(timer_id) != ptr->end()) {
    remove_timer(timer_id);
  } else {
    COCAINE_LOG_INFO(log_, "attempt to cancel timer that does not exist, id is %1%", timer_id);
  }
}

void
chrono_t::restart(io::timer_id_t timer_id) {
  timer_desc_t& timer_desc = timers_->at(timer_id);

  timer_desc.timer_->expires_from_now(boost::posix_time::seconds(timer_desc.interval_));
  timer_desc.timer_->async_wait(std::bind(&chrono_t::on_timer, this, std::placeholders::_1, timer_id));
}

void
chrono_t::on_timer(const boost::system::error_code& ec, io::timer_id_t timer_id) {
  if(ec == boost::asio::error::operation_aborted) {
    return;
  }

  BOOST_ASSERT(!ec);

  auto ptr = timers_.synchronize();

  try {
    timer_desc_t& timer_desc = ptr->at(timer_id);

    try {
      timer_desc.promise_.write(timer_id);
    } catch (const std::exception& ex) {
      remove_timer(timer_id);
    }

    if (!timer_desc.interval_ && ptr->find(timer_id) != ptr->end()) {
      remove_timer(timer_id);
    } else if (timer_desc.interval_) {
      restart(timer_id);
    }
  } catch (std::exception& ex) {
    COCAINE_LOG_ERROR(log_, "possibly bug in timer service in chrono_t::on_timer(), error is %s", ex.what());
  }
}

void
chrono_t::remove_timer(io::timer_id_t timer_id) {
  try {
    // Already locked.
    timers_.unsafe().at(timer_id).promise_.close();
    timers_.unsafe().erase(timer_id);
  } catch (const std::exception& ex) {
    COCAINE_LOG_ERROR(log_, "error occured while removing timer %s", ex.what());
    }
}
*/
