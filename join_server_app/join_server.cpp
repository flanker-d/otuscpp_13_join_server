#include <join_server_app/join_server.h>
#include <common/communications/communacations_types.h>

namespace js
{
  join_server::join_server(common::tcp_server_params_t& a_params, boost::asio::io_service &a_io_service)
    : m_server(common::tcp::create_server(a_params, a_io_service))
    , m_data_base(std::make_shared<data_base>())
  {
    m_server->set_on_connected([](const int  a_client_id){

    });
    m_server->set_on_disconnected([](const int a_client_id){

    });
    m_server->set_on_message(std::bind(&join_server::on_message, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    m_server->run();
  }

  void join_server::on_message(const int a_client_id, const char *a_data, std::size_t a_size)
  {
    std::string command{a_data, a_size};

    result_t result = m_data_base->process_command(command);

    m_server->send_message(a_client_id, result);
  }
} //namespace js

