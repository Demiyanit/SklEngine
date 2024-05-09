#include <util/event.hpp>
std::unordered_map<std::type_index, std::vector<EventCallback>> Event::events;
