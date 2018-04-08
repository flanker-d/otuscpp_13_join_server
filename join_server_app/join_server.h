#pragma once

#include <common/interface/interface.h>
#include <common/communications/communications.h>
#include <data_base/data_base.h>
#include <iostream>

namespace js
{
  using namespace common::tcp;
  using namespace db;

  class join_server : public common::interface<join_server>
  {
    public:
      join_server(common::tcp_server_params_t& a_params, boost::asio::io_service& a_io_service);

    private:
      void on_message(const int a_client_id, const char *a_data, std::size_t a_size);

    private:
      iserver::ref m_server;
      data_base::ref m_data_base;
  };
} //namespace js
