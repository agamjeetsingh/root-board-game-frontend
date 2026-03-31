//
// Created by Agamjeet Singh on 30/03/2026.
//

#ifndef ROOT_BOARD_GAME_FRONTEND_SCHEDULEDCALLBACK_H
#define ROOT_BOARD_GAME_FRONTEND_SCHEDULEDCALLBACK_H
#include <atomic>
#include <cassert>
#include <functional>

struct ScheduledCallback {
    static ScheduledCallback RunOnce(std::function<void(ScheduledCallback*, float)> cb) {
        return {std::move(cb), 0.0f, false, 0.0f};
    }

    static ScheduledCallback RunRepeating(float interval, std::function<void(ScheduledCallback*, float)> cb) {
        return {std::move(cb), 0.0f, true, interval};
    }

    static ScheduledCallback RunEveryFrame(std::function<void(ScheduledCallback*, float)> cb) {
        return {std::move(cb), 0.0f, true, 0.0f};
    }

    ScheduledCallback withDelay(float delay) const {
        return {user_callback, delay, repeat, interval};
    }

    float timeRemaining;
    float spentTime = 0;

    ScheduledCallback(
       std::function<void(ScheduledCallback*, float)> cb,
       const float time,
       const bool repeat = false,
       const float interval = 0)
    : timeRemaining(time), user_callback(std::move(cb)), interval(interval), repeat(repeat) {
        assert(time >= 0);
        assert(interval >= 0);
    }

    void cancel() {
        cancelled.store(true, std::memory_order_release);
    }

    [[nodiscard]] bool is_cancelled() const {
        return cancelled.load(std::memory_order_acquire);
    }

    void call(float dt) {
        if (!this->cancelled) {
            this->user_callback(this, dt);
        }
    }

    float getInterval() const {
        return interval;
    }

    bool isRepeating() const {
        return repeat;
    }

private:
    std::atomic_bool cancelled = false;

    float interval; // interval = 0 would mean run once every frame, events run a maximum of once per frame

    bool repeat;

    std::function<void(ScheduledCallback*, float)> user_callback;

public:

    // 1. Copy Constructor
    ScheduledCallback(const ScheduledCallback& other)
        : timeRemaining(other.timeRemaining),
          spentTime(other.spentTime),
          interval(other.interval),
          repeat(other.repeat),
          user_callback(other.user_callback),
          cancelled(other.cancelled.load(std::memory_order_acquire)) {}

    // 2. Move Constructor
    ScheduledCallback(ScheduledCallback&& other) noexcept
        : timeRemaining(other.timeRemaining),
          spentTime(other.spentTime),
          interval(other.interval),
          repeat(other.repeat),
          user_callback(std::move(other.user_callback)),
          cancelled(other.cancelled.load(std::memory_order_acquire)) {}

    // 3. Copy Assignment Operator
    ScheduledCallback& operator=(const ScheduledCallback& other) {
        if (this != &other) {
            timeRemaining = other.timeRemaining;
            spentTime = other.spentTime;
            interval = other.interval;
            repeat = other.repeat;
            user_callback = other.user_callback;
            cancelled.store(other.cancelled.load(std::memory_order_acquire), std::memory_order_release);
        }
        return *this;
    }

    // 4. Move Assignment Operator
    ScheduledCallback& operator=(ScheduledCallback&& other) noexcept {
        if (this != &other) {
            timeRemaining = other.timeRemaining;
            spentTime = other.spentTime;
            interval = other.interval;
            repeat = other.repeat;
            user_callback = std::move(other.user_callback);
            cancelled.store(other.cancelled.load(std::memory_order_acquire), std::memory_order_release);
        }
        return *this;
    }
};

#endif //ROOT_BOARD_GAME_FRONTEND_SCHEDULEDCALLBACK_H