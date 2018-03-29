#include <common/communications.h>
#include <iostream>

namespace common
{
  namespace tcp
  {
    class client_session
     : public iclient_session
    {
      public:
        client_session(boost::asio::ip::tcp::socket& a_sock, boost::asio::io_service::strand& a_strand, iserver::ref& a_server);
        ~client_session() override;
        void send_message(const std::string& a_data) override;
        void start() override;
        void shutdown() override;
      
      private:
        void do_receive() override;

      private:
        boost::asio::io_service::strand& m_strand;
        boost::asio::ip::tcp::socket m_sock;
        iserver::weak_ref m_server;
        int m_client_id;
        pbuf_t m_buffer;
    };

    client_session::client_session(boost::asio::ip::tcp::socket& a_sock, boost::asio::io_service::strand& a_strand, iserver::ref& a_server)
     : m_strand(a_strand)
     , m_sock(std::move(a_sock))
     , m_server(a_server)
     , m_client_id(m_sock.native_handle())
     , m_buffer(std::make_unique<buf_t>())
    {
    }

    client_session::~client_session()
    {
      //std::cout << "client session dtor called" << std::endl;
    }

    void client_session::send_message(const std::string& a_data)
    {
      auto async_write_handler = [this](boost::system::error_code /*ec*/, std::size_t /*length*/)
      {
        //std::cout << to_send_str;
      };
      boost::asio::async_write(m_sock, boost::asio::buffer(a_data.c_str(), a_data.length()), m_strand.wrap(async_write_handler));
    }

    void client_session::start()
    {
      do_receive();
    }

    void client_session::shutdown()
    {
      m_sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
      m_sock.close();
    }

    void client_session::do_receive()
    {
      m_buffer->fill(0);
      
        auto async_read_completion_handler = [this](const boost::system::error_code& a_ec, std::size_t a_len)->std::size_t
        {
          if(a_ec)
            return 0;
          if(a_len > 0)
          {
            bool cond = (m_buffer->data()[a_len - 1] == '\n');
            return cond ? 0 : 1;
          }
          return 1;
        };
      
        auto async_read_handler = [this](const boost::system::error_code& a_ec, std::size_t a_len)
        {
          if(a_len == 0)
          {
            if(auto serv = m_server.lock())
            {
              serv->remove_client(m_client_id);
            }
          }

          if (!a_ec)
          {
            if(auto serv = m_server.lock())
            {
              serv->on_message(m_client_id, m_buffer->data(), a_len);
            }

            do_receive();
          }
          else
          {
            if(auto serv = m_server.lock())
            {
              serv->remove_client(m_client_id);
            }
          }
        };
      
        boost::asio::async_read(m_sock, boost::asio::buffer(m_buffer->data(), BUF_LENGTH), async_read_completion_handler, m_strand.wrap(async_read_handler));
    }
  } //namespace tcp
} //namespace common

namespace common
{
  namespace tcp
  {
    iclient_session::ref create_client_session(boost::asio::ip::tcp::socket& a_sock, boost::asio::io_service::strand& a_strand, iserver::ref a_server)
    {
      return std::make_shared<client_session>(a_sock, a_strand, a_server);
    }
  } //namespace tcp
} //namespace common