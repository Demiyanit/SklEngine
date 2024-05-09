#pragma once
#include <unordered_map>
#include <typeindex>

class Event;
using EventCallback = void (*)(Event*);

class Event {
public:

	Event() = default;
	virtual ~Event() = default;
	static std::unordered_map<std::type_index, std::vector<EventCallback>> events;
	template<class T>
	static void Register(EventCallback c) {
		static_assert(std::is_base_of<Event, T>::value, "this class should be derived from Event");
		if (events.find(typeid(T)) != events.end())
			events[typeid(T)].push_back(c);
		else
			events[typeid(T)] = { c };
	}
	void Fire() {
		for (auto callback : events[typeid(*this)])
			callback(this);
	}
};
