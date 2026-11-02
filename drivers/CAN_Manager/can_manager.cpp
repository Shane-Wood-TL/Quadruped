#include "can_manager.h"

#include <cstring>

#include "esp_log.h"

static const char* TAG = "CAN_MANAGER";

canManager::canManager(twai_node_handle_t handle)
    : twai_handle(handle)
{
    rx_queue = xQueueCreate(
        RX_QUEUE_LENGTH,
        sizeof(RxQueueItem)
    );

    configASSERT(rx_queue != nullptr);


    // Create the task to handle incoming data
    BaseType_t task_result = xTaskCreate(
        receive_task,
        "can_rx_task",
        4096,
        this,
        5,
        &rx_task_handle
    );

    configASSERT(task_result == pdPASS);

    // Register the callback
    twai_event_callbacks_t callbacks = {};
    callbacks.on_rx_done = rx_callback;

    ESP_ERROR_CHECK(
        twai_node_register_event_callbacks(
            twai_handle,
            &callbacks,
            this
        )
    );
}


bool canManager::register_driver(canDriver* driver)
{
    if (driver == nullptr) {
        return false;
    }

    if (queue_count >= MAX_DRIVERS) {
        ESP_LOGE(TAG, "Maximum number of drivers reached");
        return false;
    }

    // do not add if id is already in use
    for (size_t i = 0; i < queue_count; ++i) {
        if (queues[i]->get_node_ID() == driver->get_node_ID()) {
            ESP_LOGE(
                TAG,
                "Duplicate driver node ID: %u",
                static_cast<unsigned>(driver->get_node_ID())
            );
            return false;
        }
    }

    // add to the queeue 
    queues[queue_count++] = driver;

    ESP_LOGI(
        TAG,
        "Registered driver with node ID %u",
        static_cast<unsigned>(driver->get_node_ID())
    );

    return true;
}


bool canManager::rx_callback(
    twai_node_handle_t handle,
    const twai_rx_done_event_data_t* edata,
    void* user_ctx
)
{
    (void)edata;

    auto* self = static_cast<canManager*>(user_ctx);

    uint8_t buffer[8] = {};

    twai_frame_t frame{};
    frame.buffer = buffer;
    frame.buffer_len = sizeof(buffer);

    // Get CAN data
    if (twai_node_receive_from_isr(handle, &frame) != ESP_OK) {
        return false;
    }

    if (frame.buffer_len > sizeof(buffer)) {
        return false;
    }

    RxQueueItem item{};
    item.id = frame.header.id;
    item.len = frame.buffer_len;

    if (item.len > 0) {
        memcpy(item.data, buffer, item.len);
    }

    BaseType_t higher_priority_task_woken = pdFALSE;

    // Queue the formatted struct
    if (xQueueSendFromISR(
            self->rx_queue,
            &item,
            &higher_priority_task_woken
        ) != pdTRUE) {
        return higher_priority_task_woken == pdTRUE;
    }

    return higher_priority_task_woken == pdTRUE;
}


void canManager::receive_task(void* arg)
{
    auto* self = static_cast<canManager*>(arg);

    for (;;) {
        RxQueueItem item{};

        // recieve the dat from the ISR in a normal task
        if (xQueueReceive(
                self->rx_queue,
                &item,
                portMAX_DELAY
            ) != pdTRUE) {
            continue;
        }

        // call function to handle sending to can device class
        // instances
        self->dispatch_frame(item);
    }
}


void canManager::dispatch_frame(const RxQueueItem& item)
{
    // bail if message is too long for buffer
    if (item.len > 8) {
        return;
    }

    // CAN ID = (node_ID << 5) | command_ID
    const uint8_t node_id = (item.id >> 5) & 0x3F;
    
    // go through the drivers and find the correct one,
    // pass the message to this driver
    for (size_t i = 0; i < queue_count; ++i) {
        if (queues[i]->get_node_ID() == node_id) {
            queues[i]->process_rx_frame(item);
            return;
        }
    }
}