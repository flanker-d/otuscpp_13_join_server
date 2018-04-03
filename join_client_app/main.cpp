#include <common/communications.h>
#include <boost/thread.hpp>
#include <chrono>
#include <thread>

class join_client : public common::interface<join_client>
{
  public:
    join_client(const std::string& a_ip, const int a_port, boost::asio::io_service::strand& a_strand)
      : m_client(common::tcp::create_client(a_ip, a_port, a_strand))
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

    static join_client::ref create_echo_client(const std::string& a_ip, const int a_port, boost::asio::io_service::strand& a_strand)
    {
      return std::make_shared<join_client>(a_ip, a_port, a_strand);
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
  const std::string ip = "127.0.0.1";
  const int port = 9001;

  boost::asio::io_service io_service;
  boost::asio::io_service::strand strand(io_service);

  auto client = join_client::create_echo_client(ip, port, strand);

  boost::thread_group tgroup;
  int cores_count = boost::thread::hardware_concurrency();
  for(int i = 0; i < cores_count; i++)
  {
    tgroup.create_thread([&io_service](){
      io_service.run();
    });
  }

  tgroup.join_all();

  return 0;
}
