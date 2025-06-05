#pragma once
#include "List.hpp"

template <typename Sender, typename MessageArgs>
class Event {
public:
	void subscribe(void (*funcPtr)(Sender* sender, MessageArgs* args));
	void unsubscribe (void (*funcPtr)(Sender* sender, MessageArgs* args));
	void trigger (Sender* sender, MessageArgs* args);

private:
	List<void (*)(Sender* sender, MessageArgs* args)> subscribedFunctions;
};

template<typename Sender, typename MessageArgs>
inline void Event<Sender, MessageArgs>::subscribe (void (*funcPtr)(Sender* sender, MessageArgs* args)) {
	this->subscribedFunctions.add(funcPtr);
}

template<typename Sender, typename MessageArgs>
inline void Event<Sender, MessageArgs>::unsubscribe(void(*funcPtr)(Sender* sender, MessageArgs* args)) {
	this->subscribedFunctions.remove(funcPtr);
}

template<typename Sender, typename MessageArgs>
inline void Event<Sender, MessageArgs>::trigger (Sender* sender, MessageArgs* args) {
	for (size_t i = 0; i < subscribedFunctions.getLength(); i++) {
		this->subscribedFunctions[i](sender, args);
	}
};
