/**
 * @file all_includes.h
 * @brief Contains the system wide includes for esp-idf and c++, along with structs and enums
 * @author Shane Wood
 */
#ifndef __allIncludes__
#define __allIncludes__
#include <string>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "esp_now.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_mac.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/timers.h"

#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "esp_twai.h"
#include "esp_twai_onchip.h"
#include "driver/ledc.h"


#include "esp_system.h"
#include "esp_wifi.h"
//#include "espnow.h"
#include "esp_event.h"
#include "nvs_flash.h"
//#include "espnow_ctrl.h"
//#include "espnow_utils.h"

#include "esp_timer.h"
//#include "led_strip.h"

/**
 * @brief macro to enable the toggling of the gpio pin for debugging time between esp-now messages
 */
#define TIME_BETWEEN_MESSAGES




/**
 * @enum motor_status_list
 * @brief Contains the different types of states for the Odrive motor controllers
 *
 * @details motor_status_list has the following options: ENABLED, DISABLED, ERRORLESS, CALIBRATING, IDLE
 * ENABLED = motor working as normal
 * DISABLED = motor set to idle (not by choice, some error has occurred)
 * ERRORLESS = clears motor errors
 * CALIBRATING = motor is running it's calibration sequence (SHOULD NOT BE USED AS MOTORS ARE PRE-CALIBRATED)
 * IDLE = motor is not moving / has no power going through it
 */
enum motor_status_list{ENABLED, DISABLED, ERRORLESS, CALIBRATING, IDLE};


/**
 * @enum controller_status_list
 * @brief Contains the different types of states for the controller
 * CONNECTED = controller is connected
 * DISCONNECTED = controller is disconnected
 */
enum controller_status_list{CONNECTED, DISCONNECTED};

#endif
