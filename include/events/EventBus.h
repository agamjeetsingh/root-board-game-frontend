//
// Created by Agamjeet Singh on 19/03/2026.
//

#ifndef ROOT_BOARD_GAME_FRONTEND_EVENTBUS_H
#define ROOT_BOARD_GAME_FRONTEND_EVENTBUS_H

#include <queue>
#include <set>
#include <typeindex>
#include <unordered_map>

#include "Event.h"
#include <mutex>

#include "Listener.h"

/**
 * @brief Comparator for sorting listeners by priority.
 *
 * Used by std::multiset to maintain listeners in priority order within the EventBus.
 * Lower priority values are executed first.
 */
struct ListenerComparator {
    /**
     * @brief Compare two listeners by their priority.
     * @param listenerA First listener to compare.
     * @param listenerB Second listener to compare.
     * @return true if listenerA has lower priority than listenerB.
     */
    bool operator()(const Listener& listenerA, const Listener& listenerB) const {
        return listenerA.priority < listenerB.priority;
    }
};

/**
 * @brief A singleton event bus that provides communication between different systems.
 *
 * The EventBus enables communication between different systems (e.g., Input,
 * Physics, Scheduler) through event emission and dispatch. Instead of direct calls,
 * events are emitted and dispatched to listeners at defined execution times.
 *
 * Key Features:
 * - Type-safe event emission using templates
 * - Priority-based listener execution
 * - Deferred event execution at specific times
 * - FIFO event processing per execution time
 *
 * @note The correctness of the system depends on calling execute() at the appropriate times.
 */
class EventBus {
public:
    /**
     * @brief Emit an event to be executed at the specified time.
     *
     * Events are wrapped into type-erased Event objects and queued for execution
     * at the specified time. If execute_time is NOW, the event is dispatched immediately.
     * If no listeners are registered for this event type, the event is discarded.
     *
     * @note If an event is called with EventExecuteTime::NOW, then the listeners lock would be acquired. Otherwise,
     * the events lock is acquired via a `executeNow` call.
     *
     * @tparam T The event type (any class can be used as an event).
     * @param event The event instance to emit.
     */
    template<typename T>
    void emit(T event) {
        emit(Event{event});
    }

    /**
     * @brief Execute all queued events for the specified execution time.
     *
     * Events are processed in FIFO order. For each event, listeners are called
     * according to their ListenerPriority: LOWEST → LOW → NORMAL → HIGH → HIGHEST → MONITOR.
     *
     * @note Acquires event and listener locks. But doesn't hold any locks when a listener is called.
     */
    void execute();

    /**
     * @brief Register a listener to receive events.
     *
     * This method is typically called automatically from the Listener constructor.
     *
     * @param listener The listener to register.
     */
    void registerListener(Listener listener);

    /**
     * @brief Clears all events in the queue and unregisters all listeners.
     * @warning This function should NOT be called from inside a listener as it will lead to a deadlock.
     */
    void clear();

private:
    /** @brief Queued events. */
    std::queue<Event> events;

    /** @brief Listeners organized by event type and sorted by priority. */
    std::unordered_map<std::type_index, std::multiset<Listener, ListenerComparator>> listeners;

    std::mutex events_mtx;
    std::mutex listeners_mtx;

    /**
     * @brief Internal method to emit a pre-constructed Event object.
     * @param event The event to emit.
     */
    void emit(Event event);
};


#endif //ROOT_BOARD_GAME_FRONTEND_EVENTBUS_H