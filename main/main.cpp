#include <cstdio>
#include "../setup/common_includes.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../drivers/ODESC_Driver/odesc_driver.h"
#include "../drivers/can_manager/can_manager.h"
#include "../drivers/can_manager/can_driver.h"
#include "esp_log.h"

twai_node_handle_t node_handle;

void print_odesc_data_task(void *pv);


twai_node_handle_t create_twai_node(gpio_num_t tx_pin, gpio_num_t rx_pin, uint32_t bitrate){
    twai_node_handle_t twai_handle = nullptr;
    twai_onchip_node_config_t node_config = {};
    node_config.io_cfg.tx = tx_pin;
    node_config.io_cfg.rx = rx_pin;
    node_config.bit_timing.bitrate = bitrate;
    node_config.tx_queue_depth  = 10;

    ESP_ERROR_CHECK(twai_new_node_onchip(&node_config, &twai_handle));

    return twai_handle;
}

extern "C" void app_main(void)
{
    node_handle = create_twai_node(GPIO_NUM_25, GPIO_NUM_24, 500000);

    (void)xTaskCreate(print_odesc_data_task, "print_odesc_data_task", 4096, NULL, 6, NULL);

    while (1) {
    twai_node_status_t status{};

    ESP_ERROR_CHECK(
        twai_node_get_info(node_handle, &status, nullptr)
    );


        vTaskDelay(pdMS_TO_TICKS(1000));
    
    }
}


void print_odesc_data_task(void *pv){

    canManager can_manager(node_handle);
    
    // create all CAN bus objects
    odesc front_left_hip(1, node_handle);
    odesc front_left_knee(3, node_handle);
    odesc front_left_ankle(5, node_handle);

    // register the can objects to the manager
    can_manager.register_driver(&front_left_hip);
    can_manager.register_driver(&front_left_knee);
    can_manager.register_driver(&front_left_ankle);

    ESP_ERROR_CHECK(twai_node_enable(node_handle));

    for (;;) {
       

        front_left_hip.update_bus_voltage_and_current(); ///< update the battery voltage of the odesc
        front_left_hip.update_current_measured_and_setpoint(); ///< update the current measured and setpoint of the odesc
        front_left_hip.update_encoder_position_and_velocity_estimate(); ///< update the encoder position and velocity estimate of
        
        front_left_knee.update_bus_voltage_and_current(); ///< update the battery voltage of the odesc
        front_left_knee.update_current_measured_and_setpoint(); ///< update the current measured and setpoint of the odesc
        front_left_knee.update_encoder_position_and_velocity_estimate();

        front_left_ankle.update_bus_voltage_and_current(); ///< update the battery voltage of the odesc
        front_left_ankle.update_current_measured_and_setpoint(); ///< update the current measured and setpoint of the odesc
        front_left_ankle.update_encoder_position_and_velocity_estimate();

        printf("Motor ID: %d\n", front_left_hip.get_node_ID());
        printf("Battery Voltage: %.2f V\n", front_left_hip.get_battery_voltage());
        printf("Bus Current: %.2f A\n", front_left_hip.get_bus_current());
        printf("Current Setpoint: %.2f A\n", front_left_hip.get_current_setpoint());
        printf("Current Measured: %.2f A\n", front_left_hip.get_current_measured());
        printf("Encoder Position Estimate: %.2f rad\n", front_left_hip.get_encoder_position_estimate());
        printf("Input Velocity Estimate: %.2f rad/s\n", front_left_hip.get_encoder_velocity_estimate());

        printf("Motor ID: %d\n", front_left_knee.get_node_ID());
        printf("Battery Voltage: %.2f V\n", front_left_knee.get_battery_voltage());
        printf("Bus Current: %.2f A\n", front_left_knee.get_bus_current());
        printf("Current Setpoint: %.2f A\n", front_left_knee.get_current_setpoint());
        printf("Current Measured: %.2f A\n", front_left_knee.get_current_measured());
        printf("Encoder Position Estimate: %.2f rad\n", front_left_knee.get_encoder_position_estimate());
        printf("Input Velocity Estimate: %.2f rad/s\n", front_left_knee.get_encoder_velocity_estimate());

        printf("Motor ID: %d\n", front_left_ankle.get_node_ID());
        printf("Battery Voltage: %.2f V\n", front_left_ankle.get_battery_voltage());
        printf("Bus Current: %.2f A\n", front_left_ankle.get_bus_current());
        printf("Current Setpoint: %.2f A\n", front_left_ankle.get_current_setpoint());
        printf("Current Measured: %.2f A\n", front_left_ankle.get_current_measured());
        printf("Encoder Position Estimate: %.2f rad\n", front_left_ankle.get_encoder_position_estimate());
        printf("Input Velocity Estimate: %.2f rad/s\n", front_left_ankle.get_encoder_velocity_estimate());

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}