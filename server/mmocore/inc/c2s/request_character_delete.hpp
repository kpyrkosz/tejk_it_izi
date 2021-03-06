#pragma once

#include <binary_deserializer.hpp>
#include <database_facade.hpp>
#include <lobby_character.hpp>
#include <logger.hpp>
#include <net_session.hpp>
#include <received_packet.hpp>

class request_character_delete : public received_packet
{
  std::shared_ptr<net_session> my_session_;
  lobby_character::lobby_character_list& lobby_chars_;
  logger& logger_;
  database_facade& db_;
  std::string char_name_;

public:
  request_character_delete(binary_deserializer& bd,
                           std::shared_ptr<net_session> my_session,
                           logger& logger,
                           lobby_character::lobby_character_list& lobby_chars,
                           database_facade& db);
  virtual void execute_associated_action() override;
};