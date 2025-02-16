/*
 * sensor_handler.c
 *
 *  Created on: Feb 13, 2025
 *      Author: titod
 */

#include "main.h"             // Ensures access to HAL functions and system config
#include "sensor_handler.h"
#include "stdio.h"            // For debugging with printf()

VL53L8CX_Configuration sensor_config;

// Store previous values for gesture detection
uint16_t previous_distance = 0;
int16_t previous_x = 0;  // Placeholder for X-axis tracking
int16_t previous_y = 0;  // Placeholder for Y-axis tracking

/**
 * @brief Initializes the VL53L8CX sensor.
 */
void Sensor_Init(void) {
    if (vl53l8cx_init(&sensor_config) != 0) {
        printf("VL53L8CX Initialization Failed!\n");
    } else {
        printf("VL53L8CX Sensor Initialized.\n");
    }
}

/**
 * @brief Detects hand gestures based on distance and movement changes.
 * @return Character representing detected gesture.
 */
char DetectGesture(void) {
    VL53L8CX_ResultsData results;
    vl53l8cx_get_ranging_data(&sensor_config, &results);

    uint16_t current_distance = results.distance_mm;
    int16_t current_x = 0;  // Placeholder for X-axis tracking
    int16_t current_y = 0;  // Placeholder for Y-axis tracking

    int16_t distance_diff = current_distance - previous_distance;
    int16_t x_diff = current_x - previous_x;
    int16_t y_diff = current_y - previous_y;

    // Update previous values for next cycle
    previous_distance = current_distance;
    previous_x = current_x;
    previous_y = current_y;

    // Fine-tuned thresholds for gesture detection
    int threshold_swipe = 60;  // Detect swipe gestures (adjust based on testing)
    int threshold_push_pull = 25;  // Detect push/pull movements

    // Detect Push and Pull gestures
    if (distance_diff > threshold_push_pull) return 'P';  // Push
    if (distance_diff < -threshold_push_pull) return 'p'; // Pull

    // Detect Swipe Gestures (Left, Right, Up, Down)
    if (x_diff > threshold_swipe) return 'R';  // Swipe Right
    if (x_diff < -threshold_swipe) return 'L'; // Swipe Left
    if (y_diff > threshold_swipe) return 'U';  // Swipe Up
    if (y_diff < -threshold_swipe) return 'D'; // Swipe Down

    return 'N'; // No gesture detected
}
