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

    const std::function<void(ScheduledCallback*, float)> user_callback;

    ScheduledCallback(
       std::function<void(ScheduledCallback*, float)> cb,
       const float time,
       const bool repeat = false,
       const float interval = 0)
    : timeRemaining(time), user_callback(std::move(cb)), callback([this](float deltaTime) {
        if (!this->cancelled) {
            this->user_callback(this, deltaTime);
        }
    }), interval(interval), repeat(repeat) {
        assert(time >= 0);
        assert(interval >= 0);
    }

    void cancel() {
        cancelled.store(true, std::memory_order_release);
    }

    [[nodiscard]] bool is_cancelled() const {
        return cancelled.load(std::memory_order_acquire);
    }

    void call(float dt) const {
        callback(dt);
    }

private:
    std::atomic_bool cancelled = false;
    std::function<void(float)> callback;

    const float interval; // interval = 0 would mean run once every frame, events run a maximum of once per frame

    const bool repeat;
};

#endif //ROOT_BOARD_GAME_FRONTEND_SCHEDULEDCALLBACK_H