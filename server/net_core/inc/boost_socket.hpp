#pragma once

#include <boost/asio.hpp>
#include <net_socket.hpp>

class boost_socket : public net_socket, public std::enable_shared_from_this<boost_socket>
{

  bool is_pending_deletion_;
  boost::asio::ip::tcp::socket sock_;
  buffer recvbuf_;

  void do_recv_expected_len(recv_completion_handler& handler);
  void do_recv_data(recv_completion_handler& handler, unsigned expected_len);

public:
  boost_socket(boost::asio::ip::tcp::socket&& sock);

  virtual void async_recv(recv_completion_handler& handler) override;
  virtual void send_to_client(const buffer& data, unsigned size) override;
  virtual void force_close() override;
};
