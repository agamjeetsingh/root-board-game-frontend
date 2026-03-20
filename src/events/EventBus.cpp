//
// Created by Agamjeet Singh on 19/03/2026.
//

#include "../../include/events/EventBus.h"

#include "../../include/events/Listener.h"
#include <mutex>

void EventBus::emit(Event event) {
    std::lock_guard events_lock{events_mtx};
    events.push(std::move(event));
}

// TODO - Get all listeners before while loop. But have to change it so they are all shared_ptrs instead
void EventBus::execute() {
    std::queue<Event> events;
    {
        std::lock_guard events_lock{events_mtx};
        std::swap(events, this->events);
    }
    while (!events.empty()) {
        Event event = events.front();
        events.pop();
        std::vector<Listener> snapshot;
        {
            std::lock_guard listeners_lock{listeners_mtx};
            const auto& multiset = listeners[event.type_index];
            snapshot.reserve(multiset.size());
            std::ranges::copy(multiset, std::back_inserter(snapshot));
        }
        for (auto& listener : snapshot) {
            listener.call(&event);
        }
    }
}

void EventBus::registerListener(Listener listener) {
    std::unique_lock listeners_lock(listeners_mtx);
    listeners[listener.type_index].insert(std::move(listener));
}

void EventBus::clear() {
    std::unique_lock events_lock{events_mtx};
    std::unique_lock listeners_lock{listeners_mtx};
    listeners.clear();
    // Queue class does not have a clear method unfortunately
    events = std::queue<Event>{};
}