#pragma once

#include <stdint.h>
#include "esp_twai.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define max_message_size 8 ///< the maximum size of a message
#define node_id_offset 5 ///< the offset of the node id in the message
#define lower_five_bits 0x1F ///< the lower five bits of a byte

struct RxQueueItem {
    uint32_t id;
    uint8_t len;
    uint8_t data[8];
};


class canDriver {
    uint8_t node_id;
    twai_node_handle_t twai_handle;

    twai_frame_t tx_msg {};
    uint8_t tx_buffer[max_message_size] {};

    public:
        canDriver(uint8_t id, twai_node_handle_t handle) : node_id(id), twai_handle(handle) {}

        uint8_t get_node_ID() const { return node_id; }

        /**
         * @brief send a command to the odesc
         *
         * @param message_ID : const uint8_t the ID of the message to be sent
         * @param message_contents : const uint8_t* the data to be sent with the message
         * @param message_length : uint8_t the length of the data
         * @param data_returned : const bool if data is returned (determines if receive_message is called or not)
         */
        void send_message(const uint8_t message_ID, const uint8_t* message_contents, uint8_t message_length, const bool data_returned);

        virtual void process_rx_frame(
            const RxQueueItem& item
        ) = 0;
};