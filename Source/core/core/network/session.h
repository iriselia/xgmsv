#pragma once

#include "core/network/timer.h"
#include "core/network/packet_queue.h"

namespace core
{
    struct session
    {
        uint32 id;
        bool marked_for_delete;
        std::chrono::time_point<steady_clock> last_active;

        packet_queue rqueue;
        packet_queue wqueue;
        async_signal signal;

        session() :
            id(-1),
            marked_for_delete(false),
            last_active(std::chrono::steady_clock::now())
        {}

        void activate()
        {
            last_active = core::server_time;
        }
    };
}
