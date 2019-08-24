#include <binary_serializer.hpp>
#include <c2s/join_chat.hpp>
#include <fake_user_environment.hpp>
#include <gtest/gtest.h>
#include <ingame_state_protocol.hpp>

TEST(join_chat, construction)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << std::string("some_room_name");
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  fake_user_environment env;
  auto pair_of_mocks = env.make_env("krzysztof", "KoncepcyjnyMiliarder");
  user_environment& my_env = *pair_of_mocks.first;
  std::shared_ptr<fake_net_session> my_session = pair_of_mocks.second;

  EXPECT_NO_THROW(join_chat(incoming_stream, my_env));
}

TEST(join_chat, throw_on_packet_underflow)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << 7; // will cause underflow
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  fake_user_environment env;
  auto pair_of_mocks = env.make_env("krzysztof", "KoncepcyjnyMiliarder");
  user_environment& my_env = *pair_of_mocks.first;
  std::shared_ptr<fake_net_session> my_session = pair_of_mocks.second;

  EXPECT_THROW(join_chat(incoming_stream, my_env), std::underflow_error);
}

TEST(join_chat, world_state_after_executing_packet)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << std::string("some_room_name") << std::string("some_other_room");
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  fake_user_environment env;
  auto pair_of_mocks = env.make_env("krzysztof", "KoncepcyjnyMiliarder");
  user_environment& my_env = *pair_of_mocks.first;
  std::shared_ptr<fake_net_session> my_session = pair_of_mocks.second;
  auto& sock_queue = my_session->get_sock()->get_packets_sent_to_client();

  // user sent join chat request
  join_chat(incoming_stream, my_env).execute_associated_action();
  {
    ASSERT_EQ(1, sock_queue.size());
    std::string data_client_received(std::move(sock_queue.front()));
    sock_queue.pop();
    binary_deserializer bd(data_client_received);
    std::string chatroom, user;
    ingame_state_protocol::to_client_packet_opcodes opcode;
    bd >> opcode >> chatroom >> user;
    EXPECT_EQ(ingame_state_protocol::to_client_packet_opcodes::user_joined_chat, opcode);
    EXPECT_EQ("some_room_name", chatroom);
    EXPECT_EQ("KoncepcyjnyMiliarder", user);
    EXPECT_EQ(1, my_env.chat_controller_.count_of_chatrooms_im_in());
  }
  // sent again
  join_chat(incoming_stream, my_env).execute_associated_action();
  {
    ASSERT_EQ(1, sock_queue.size());
    std::string data_client_received(std::move(sock_queue.front()));
    sock_queue.pop();
    binary_deserializer bd(data_client_received);
    std::string chatroom, user;
    ingame_state_protocol::to_client_packet_opcodes opcode;
    bd >> opcode >> chatroom >> user;
    EXPECT_EQ(ingame_state_protocol::to_client_packet_opcodes::user_joined_chat, opcode);
    EXPECT_EQ("some_other_room", chatroom);
    EXPECT_EQ("KoncepcyjnyMiliarder", user);
    EXPECT_EQ(2, my_env.chat_controller_.count_of_chatrooms_im_in());
  }
}