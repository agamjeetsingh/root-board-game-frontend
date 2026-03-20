//
// Created by Agamjeet Singh on 19/03/2026.
//

#ifndef ROOT_BOARD_GAME_FRONTEND_LISTENER_H
#define ROOT_BOARD_GAME_FRONTEND_LISTENER_H

#include <functional>
#include <typeindex>
#include <utility>

#include "Event.h"
#include "ListenerPriority.h"

class EventBus;
/**
 * @brief A type-erased listener that can receive events of a specific type.
 *
 * The Listener class stores type information about the event it listens to through
 * std::type_index and keeps a type-erased callback. Listeners are automatically
 * registered with the EventBus upon construction.
 *
 * Priority-based Execution:
 * Listeners are executed in priority order: LOWEST → LOW → NORMAL → HIGH → HIGHEST → MONITOR.
 * If multiple listeners have the same priority, their relative execution order is not guaranteed.
 *
 * Usage:
 * \code
 * // Create a listener in a constructor or member initializer
 * auto listener = Listener::make_listener<MyEvent>([this](const MyEvent& event) {
 *     // Handle the event
 *     processEvent(event);
 * }, ListenerPriority::HIGH);
 * \endcode
 */
class Listener {
public:
    /**
     * @brief Create a type-safe listener for events of type T.
     *
     * This is the primary way to construct listeners. The callback must take a const T&
     * and return void. The listener will be automatically registered with the EventBus.
     *
     * @tparam T The event type to listen for.
     * @tparam F The callback function type.
     * @param cb The callback function that will be invoked when the event is dispatched.
     * @param event_bus The event bus to register the listener in
     * @param priority The priority of this listener (defaults to NORMAL).
     * @return A constructed Listener that has been registered with the EventBus.
     *
     * @note The callback constraint: std::invocable<F, const T&> && std::same_as<std::invoke_result_t<F, const T&>, void>
     */
    template<typename T, typename F>
    requires std::invocable<F, const T&> &&
             std::same_as<std::invoke_result_t<F, const T&>, void>
    static Listener make_listener(F&& cb, EventBus& event_bus, ListenerPriority priority = ListenerPriority::NORMAL) {
        Listener listener(std::type_identity<T>{}, std::forward<F>(cb), priority);
        listener.registerWith(event_bus);
        return listener;
    }

    /**
     * @brief Register this listener with the given EventBus.
     * @param event_bus The event bus to register with.
     */
    void registerWith(EventBus& event_bus) const;

    /** @brief The priority of this listener, determines execution order. */
    const ListenerPriority priority;

    /** @brief The type of event this listener handles. */
    const std::type_index type_index;

    /**
     * @brief Invoke the listener's callback with the provided event.
     * @param ev Pointer to the event data.
     */
    void call(const void* ev) const {
        erased_cb(ev);
    }

private:
    /** @brief Type-erased callback function. */
    std::function<void(const void*)> erased_cb;

    /**
     * @brief Private constructor that creates a type-erased listener.
     *
     * This constructor is called by make_listener() and handles the type erasure
     * of the callback while preserving type safety.
     *
     * @tparam T The event type.
     * @tparam F The callback function type.
     * @param std::type_identity<T> Type identity wrapper for template deduction.
     * @param cb The callback function to wrap.
     * @param priority The listener priority.
     */
    template<typename T, typename F>
    requires std::invocable<F, const T&> &&
             std::same_as<std::invoke_result_t<F, const T&>, void>
    explicit Listener(std::type_identity<T>, F&& cb, ListenerPriority priority = ListenerPriority::NORMAL) :
        priority(priority), type_index(typeid(T)), erased_cb([fn = std::forward<F>(cb)](const void* ev) {
            const Event& e = *static_cast<const Event*>(ev);
            if (auto payload = e.getIf<T>()) {
                fn(*payload);
            }
        }) {}
};


#endif //ROOT_BOARD_GAME_FRONTEND_LISTENER_H