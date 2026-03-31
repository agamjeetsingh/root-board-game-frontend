//
// Created by Agamjeet Singh on 30/03/2026.
//

#ifndef ROOT_BOARD_GAME_FRONTEND_SCHEDULER_H
#define ROOT_BOARD_GAME_FRONTEND_SCHEDULER_H
#include <mutex>
#include <vector>

#include "ScheduledCallback.h"


class Scheduler {
public:
    [[nodiscard]] std::weak_ptr<ScheduledCallback> schedule(const ScheduledCallback& scheduled_callback) {
        const std::unique_lock lock(update_mtx, std::try_to_lock);
        std::shared_ptr shared_callback = std::make_unique<ScheduledCallback>(scheduled_callback);

        if (lock) {
            std::lock_guard events_lock{events_mtx};
            events.push_back(shared_callback);
        } else {
            std::lock_guard events_buffer_lock{events_buffer_mtx};
            eventsBuffer.push_back(shared_callback);
        }

        return shared_callback;
    }

    void update(const float dt) {
        std::lock_guard update_lock{update_mtx};
        std::lock_guard events_lock{events_mtx};
        for (auto it = events.begin(); it != events.end(); ) {
            if (it->get()->is_cancelled()) {
                it = events.erase(it);
                continue;
            }
            it->get()->timeRemaining -= dt;
            it->get()->spentTime += dt;
            if (it->get()->timeRemaining <= 0.0f) {
                it->get()->call(dt);
                if (it->get()->isRepeating()) {
                    it->get()->timeRemaining = it->get()->getInterval();
                    ++it;
                } else {
                    it = events.erase(it);
                }
            } else {
                ++it;
            }
        }
        std::lock_guard events_buffer_lock{events_buffer_mtx};
        events.insert(events.end(), eventsBuffer.begin(), eventsBuffer.end());
        eventsBuffer.clear();
    }

    void cancelAllEvents() {
        std::lock_guard update_lock{update_mtx};
        std::lock_guard events_lock{events_mtx};
        std::lock_guard events_buffer_lock{events_buffer_mtx};
        events.clear();
        eventsBuffer.clear();
    }

private:
    std::vector<std::shared_ptr<ScheduledCallback>> eventsBuffer;
    std::vector<std::shared_ptr<ScheduledCallback>> events;
    mutable std::mutex events_buffer_mtx;
    mutable std::mutex events_mtx;
    mutable std::mutex update_mtx;
};

#endif //ROOT_BOARD_GAME_FRONTEND_SCHEDULER_H