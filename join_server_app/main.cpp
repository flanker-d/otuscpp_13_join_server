#include <join_server_app/join_server.h>
#include <thread>

int main(int argc, char **argv)
{
  if(argc == 2)
  {
    boost::asio::io_service io_service;

    common::tcp_server_params_t params;
    params.use_strand = true;
    params.port = std::stoi(argv[1]);

    js::join_server::ptr server = std::make_unique<js::join_server>(params, io_service);

    std::vector<std::thread> thread_group;
    int threads_count = std::thread::hardware_concurrency();
    for(int i = 0; i < threads_count; i++)
    {
      thread_group.emplace_back(std::thread([&io_service](){
        io_service.run();
      }));
    }

    for(auto& thr : thread_group)
      thr.join();
  }
  else
    std::cout << "usage: join_server_app <port>" << std::endl;

  return 0;
}