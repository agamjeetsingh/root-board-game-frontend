//
// Created by Agamjeet Singh on 19/03/2026.
//

#include "../../include/events/Listener.h"
#include "../../include/events/EventBus.h"

class EventBus;

void Listener::registerWith(EventBus& event_bus) const {
    event_bus.registerListener(*this);
}
