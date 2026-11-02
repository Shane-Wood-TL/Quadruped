#ifndef __can_manager__
#define __can_manager__

#include "esp_twai.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "../can_manager/can_driver.h"

class canDriver;

class canManager {
private:
    static constexpr size_t RX_QUEUE_LENGTH = 30;
    static constexpr size_t MAX_DRIVERS = 8;

    twai_node_handle_t twai_handle;
    QueueHandle_t rx_queue = nullptr;
    TaskHandle_t rx_task_handle = nullptr;

    canDriver* queues[MAX_DRIVERS] = {};
    size_t queue_count = 0;

    static void receive_task(void* arg);

    static bool rx_callback(
        twai_node_handle_t handle,
        const twai_rx_done_event_data_t* edata,
        void* user_ctx
    );

    void dispatch_frame(const RxQueueItem& item);

public:
    explicit canManager(twai_node_handle_t handle);

    bool register_driver(canDriver* driver);
};

#endif // __can_manager__