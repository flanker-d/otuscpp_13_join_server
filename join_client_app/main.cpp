#include <common/communications/communications.h>
#include <chrono>
#include <iostream>
#include <thread>

class join_client : public common::interface<join_client>
{
  public:
    join_client(common::tcp_client_params_t& a_params, boost::asio::io_service& a_io_service)
      : m_client(common::tcp::create_client(a_params, a_io_service))
    {
      m_client->set_on_connected([this]{
        m_is_connected = !m_is_connected;
        if(!m_is_connected)
            return;
        std::cout << "client_connected" << std::endl;
        if(!m_thread.joinable())
          m_thread = std::thread([this]{test();});

      });
      m_client->set_on_disconnected([this]{
        m_is_connected = false;
        std::cout << "client_disconnected" << std::endl;
        m_thread.join();
      });
      m_client->set_on_message([](const std::string& a_msg){
        std::cout << "< " << a_msg;
      });

      m_client->run();
    }

    static join_client::ref create_echo_client(common::tcp_client_params_t& a_params, boost::asio::io_service& a_io_service)
    {
      return std::make_shared<join_client>(a_params, a_io_service);
    }

    void send_msg(const std::string& msg)
    {
      const int msec = 50;
      std::cout << "> " + msg;
      m_client->send_message(msg);
      std::this_thread::sleep_for(std::chrono::milliseconds(msec));
    }

  private:
    void test()
    {
      std::cout << "---------------------" << std::endl;

      send_msg("INSERT A 0 test\n");
      send_msg("TRUNCATE A\n");

      std::cout << "---------------------" << std::endl;

      send_msg("WRONG_INSERT A 0 lean\n");
      send_msg("INSERT A 0 lean\n");
      send_msg("INSERT A 0 understand\n");
      send_msg("INSERT A 1 sweater\n");
      send_msg("INSERT A 2 frank\n");
      send_msg("INSERT A 3 violation\n");
      send_msg("INSERT A 4 quality\n");
      send_msg("INSERT A 5 precision\n");
      send_msg("INSERT B 3 proposal\n");
      send_msg("INSERT B 4 example\n");
      send_msg("INSERT B 5 lake\n");
      send_msg("INSERT B 6 flour\n");
      send_msg("INSERT B 7 wonder\n");
      send_msg("INSERT B 8 selection\n");

      std::cout << "---------------------" << std::endl;

      send_msg("INTERSECTION\n");

      std::cout << "---------------------" << std::endl;

      send_msg("SYMMETRIC_DIFFERENCE\n");

      std::cout << "---------------------" << std::endl;
    }

  private:
    common::tcp::iclient::ref m_client;
    std::thread m_thread;
    bool m_is_connected = false;
};

int main(int argc, char** argv)
{
  common::tcp_client_params_t params;
  params.ip = "127.0.0.1";
  params.port = 9000;

  boost::asio::io_service io_service;

  auto client = join_client::create_echo_client(params, io_service);

  std::vector<std::thread> thread_group;
  int cores_count = std::thread::hardware_concurrency();
  for(int i = 0; i < cores_count; i++)
  {
    thread_group.emplace_back(std::thread([&io_service](){
      io_service.run();
    }));
  }

  for(auto& thr : thread_group)
    thr.join();

  return 0;
}
