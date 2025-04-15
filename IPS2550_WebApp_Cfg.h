/******************************************************************************
 *  File: IPS2550_WebApp_Cfg.h
 *  Description: Reads analog signals (sin and cos) from rotary encoder and
 *               calculates angle of rotation in degrees.
 *  Target: ESP32-S3-DEV-KIT-N16R8-M
 *  Date: 2025/XX/XX
 *  License: MinebeaMitsumi Inc.
 ******************************************************************************/

#ifndef IPS2550_WEBAPP_CFG_H
#define IPS2550_WEBAPP_CFG_H

//==============================================================================
// Feature Configuration Flags
//==============================================================================

#define ESP32_DEBUG                  (0U)       // Enable debug output
#define ESP32_SERIAL                 (0U)       // Enable serial communication
#define ESP32_BLUETOOTH              (0U)       // Enable Bluetooth support
#define ESP32_WIFI                   (1U)       // Enable Wi-Fi support

//==============================================================================
// Test Parameters
//==============================================================================

#define ESP32_CFG_TEST_RANGE         (2398)     // Analog input range (max ADC value)

//==============================================================================
// System Settings
//==============================================================================

#define ESP32_CFG_SETUP_DELAY        (15000U)   // Delay before setup() starts (in ms)
#define ESP32_CFG_SERIAL_BAUDRATE    (115200U)  // Baudrate for Serial.begin()

//==============================================================================
// GPIO Configuration for Rotary Encoder (IPS2550)
//==============================================================================

#define ESP32_CFG_PORT_GPIO_COS      (4U)       // GPIO pin for COS signal input
#define ESP32_CFG_PORT_GPIO_COSN     ()         // Not assigned or unused
#define ESP32_CFG_PORT_GPIO_SIN      (5U)       // GPIO pin for SIN signal input
#define ESP32_CFG_PORT_GPIO_SINN     ()         // Not assigned or unused

#endif // IPS2550_WEBAPP_CFG_H
