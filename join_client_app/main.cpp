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

        std::cout << "client_connected" << std::endl;

        m_thread = std::thread([this]{test();});

      });
      m_client->set_on_disconnected([this]{
        std::cout << "client_disconnected" << std::endl;
        m_thread.join();
      });
      m_client->set_on_message([](const std::string& a_msg){
        std::cout << "> " << a_msg;
      });

      m_client->run();
    }

    static join_client::ref create_echo_client(const std::string& a_ip, const int a_port, boost::asio::io_service::strand& a_strand)
    {
      return std::make_shared<join_client>(a_ip, a_port, a_strand);
    }

  private:
    void test()
    {
      const int msec = 250;

      std::cout << "---------------------" << std::endl;

      std::cout << "> INSERT A 0 test" << std::endl;
      m_client->send_message("INSERT A 0 test\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(msec));

      std::cout << "> TRUNCATE A" << std::endl;
      m_client->send_message("TRUNCATE A\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(msec));

      std::cout << "---------------------" << std::endl;

      std::cout << "> INSERT A 0 lean" << std::endl;
      m_client->send_message("INSERT A 0 lean\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(msec));

      std::cout << "> INSERT A 0 understand" << std::endl;
      m_client->send_message("INSERT A 0 understand\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(msec));

      std::cout << "> INSERT A 1 sweater" << std::endl;
      m_client->send_message("INSERT A 1 sweater\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(msec));

      std::cout << "> INSERT A 2 frank" << std::endl;
      m_client->send_message("INSERT A 2 frank\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(msec));

      std::cout << "> INSERT A 3 violation" << std::endl;
      m_client->send_message("INSERT A 3 violation\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(msec));

      std::cout << "> INSERT A 4 quality" << std::endl;
      m_client->send_message("INSERT A 4 quality\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(msec));

      std::cout << "> INSERT A 5 precision" << std::endl;
      m_client->send_message("INSERT A 5 precision\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(msec));

      std::cout << "> INSERT B 3 proposal" << std::endl;
      m_client->send_message("INSERT B 3 proposal\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(msec));

      std::cout << "> INSERT B 4 example" << std::endl;
      m_client->send_message("INSERT B 4 example\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(msec));

      std::cout << "> INSERT B 5 lake" << std::endl;
      m_client->send_message("INSERT B 5 lake\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(msec));

      std::cout << "> INSERT B 6 flour" << std::endl;
      m_client->send_message("INSERT B 6 flour\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(msec));

      std::cout << "> INSERT B 7 wonder" << std::endl;
      m_client->send_message("INSERT B 7 wonder\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(msec));

      std::cout << "> INSERT B 8 selection" << std::endl;
      m_client->send_message("INSERT B 8 selection\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(msec));

      std::cout << "---------------------" << std::endl;

      std::cout << "> INTERSECTION" << std::endl;
      m_client->send_message("INTERSECTION\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(msec));

      std::cout << "---------------------" << std::endl;

      std::cout << "> SYMMETRIC_DIFFERENCE" << std::endl;
      m_client->send_message("SYMMETRIC_DIFFERENCE\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(msec));
    }

  private:
    common::tcp::iclient::ref m_client;
    std::thread m_thread;
};

int main(int argc, char** argv)
{
  const std::string ip = "127.0.0.1";
  const int port = 9000;

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
