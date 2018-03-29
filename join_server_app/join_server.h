#pragma once

#include <common/interface.h>
#include <common/communications.h>
#include <data_base/data_base.h>
#include <iostream>

namespace js
{
  using namespace common::tcp;
  using namespace db;

  class join_server : public common::interface<join_server>
  {
    public:
      join_server(const int a_port, boost::asio::io_service::strand &a_strand);

    private:
      void on_message(const int a_client_id, const char *a_data, std::size_t a_size);

    private:
      iserver::ref m_server;
      data_base::ref m_data_base;
  };
} //namespace js
