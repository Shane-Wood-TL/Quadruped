#include "can_driver.h"

void canDriver::send_message(const uint8_t message_ID, const uint8_t *message_contents, uint8_t message_length, const bool data_returned)
{
    // max data size is 8 bytes
    if (message_length > max_message_size)
    {
        return;
    }
    if (message_contents == nullptr)
    {
        message_length = 0;
    }

    // use odrive standard for command + node id
    uint32_t can_ID = (node_id << node_id_offset) | (message_ID & lower_five_bits);

    tx_msg.header.id = can_ID;
    tx_msg.header.ide = false;
    tx_msg.header.rtr = data_returned;

    if (message_length > 0) {
        (void)memcpy(tx_buffer, message_contents, message_length);
    }
    tx_msg.buffer = tx_buffer;
    tx_msg.buffer_len = message_length;

    esp_err_t result =
    twai_node_transmit(twai_handle, &tx_msg, 200);

    vTaskDelay(pdMS_TO_TICKS(10));

    if (result != ESP_OK) {
        ESP_LOGE("ODESC",
                "Transmit failed for command 0x%02X: %s",
                message_ID,
                esp_err_to_name(result));
    }
}