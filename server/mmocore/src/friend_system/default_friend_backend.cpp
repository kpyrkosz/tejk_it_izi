#include <friend_system/default_friend_backend.hpp>
#include <s2c/broken_friendship.hpp>
#include <s2c/friend_add_fail.hpp>
#include <s2c/friend_add_result.hpp>
#include <s2c/friend_leave.hpp>
#include <s2c/friend_login.hpp>
#include <s2c/friend_online_while_you_login.hpp>
#include <s2c/friend_removal_fail.hpp>
#include <s2c/friendship_request.hpp>
#include <s2c/new_friendship.hpp>

default_friend_backend::default_friend_backend(net_session::session_ptr my_session)
  : my_session_(my_session)
{
}

void
default_friend_backend::on_friend_online_while_you_login(const std::string& his_nick)
{
  net_socket::buffer buffer;
  auto len = ingame_packet_constructors::friend_online_while_you_login(buffer, his_nick);
  my_session_->send_to_client(buffer, len);
}

void
default_friend_backend::on_friend_login(const std::string& his_nick)
{
  net_socket::buffer buffer;
  auto len = ingame_packet_constructors::friend_login(buffer, his_nick);
  my_session_->send_to_client(buffer, len);
}

void
default_friend_backend::on_friend_leave(const std::string& his_nick)
{
  net_socket::buffer buffer;
  auto len = ingame_packet_constructors::friend_leave(buffer, his_nick);
  my_session_->send_to_client(buffer, len);
}

void
default_friend_backend::on_new_friendship(const std::string& his_nick)
{
  net_socket::buffer buffer;
  auto len = ingame_packet_constructors::new_friendship(buffer, his_nick);
  my_session_->send_to_client(buffer, len);
}

void
default_friend_backend::on_broken_friendship(const std::string& his_nick)
{
  net_socket::buffer buffer;
  auto len = ingame_packet_constructors::broken_friendship(buffer, his_nick);
  my_session_->send_to_client(buffer, len);
}

void
default_friend_backend::on_friendship_request(const std::string& his_nick)
{
  net_socket::buffer buffer;
  auto len = ingame_packet_constructors::friendship_request(buffer, his_nick);
  my_session_->send_to_client(buffer, len);
}

void
default_friend_backend::on_my_request_sent(const std::string& his_nick,
                                           friend_add_request_result result)
{
  net_socket::buffer buffer;
  auto len = ingame_packet_constructors::friend_add_result(buffer, result, his_nick);
  my_session_->send_to_client(buffer, len);
}

void
default_friend_backend::on_friend_add_fail(const std::string& his_nick,
                                           friend_add_fail_reason reason)
{
  net_socket::buffer buffer;
  auto len = ingame_packet_constructors::friend_add_fail(buffer, reason, his_nick);
  my_session_->send_to_client(buffer, len);
}

void
default_friend_backend::on_friend_removal_fail(const std::string& his_nick)
{
  net_socket::buffer buffer;
  auto len = ingame_packet_constructors::friend_removal_fail(buffer, his_nick);
  my_session_->send_to_client(buffer, len);
}
