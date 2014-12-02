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

#ifndef COCAINE_DIRECT_ACCESS_SERVICE_INTERFACE_HPP
#define COCAINE_DIRECT_ACCESS_SERVICE_INTERFACE_HPP

#include <cocaine/rpc/protocol.hpp>

#include <elliptics/session.hpp>

using namespace ioremap::elliptics;

namespace cocaine { namespace io {

    struct direct_access_tag;

    typedef int64_t timer_id_t;

    struct direct_access {
      struct read_data {
        typedef direct_access_tag tag;

        static const char* alias() {
	  return "notify_after";
        }

        typedef boost::mpl::list<
	  std::string,
	  std::vector<int>,
	  uint64_t,
	  uint64_t
	  > tuple_type;

	typedef deferred<async_read_result> result_type;
      };

      /*      struct write_data {
        typedef direct_access_tag tag;

        static const char* alias() {
	  return "notify_every";
        }

        typedef boost::mpl::list<
	  std::string,
	  data_pointer,
	  uint64_t,
	  uint64_t
	  > tuple_type;

	typedef deferred<async_write_result> result_type;
	};*/

      struct lookup {
        typedef direct_access_tag tag;

        static const char* alias() {
	  return "cancel";
        }

        typedef boost::mpl::list<
	  std::string
	  > tuple_type;

	typedef deferred<async_lookup_result> result_type;
      };

      /*      struct restart {
        typedef direct_access_tag tag;

        static const char* alias() {
	  return "restart";
        }

        typedef boost::mpl::list<
	  /* timer id  timer_id_t
	  > argument_type;
      };*/
    };

    template<>
    struct protocol<direct_access_tag> {
      typedef boost::mpl::int_<
        1
	>::type version;

      typedef mpl::list<
        direct_access::read_data,
	direct_access::lookup
	/*        direct_access::notify_every,
        direct_access::cancel,
        direct_access::restart*/
	> type;

      //      typedef direct_access type;
    };

  }} // namespace cocaine::io

#endif
