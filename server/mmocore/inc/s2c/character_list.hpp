#pragma once

#include <array>
#include <lobby_character.hpp>

namespace lobby_packet_constructors
{
unsigned character_list(std::array<char, 2048>& buffer_to_fill,
                        const lobby_character::lobby_character_list& lobby_chars);
}