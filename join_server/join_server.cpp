#include <join_server/join_server.h>

namespace js
{
  join_server::join_server(const int a_port, boost::asio::io_service::strand &a_strand) : m_server(create_server(a_port, a_strand))
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
    std::string command{a_data, a_size - 1};

    result_t result = m_data_base->process_command(command);

    m_server->send_message(a_client_id, result);
  }
} //namespace js

