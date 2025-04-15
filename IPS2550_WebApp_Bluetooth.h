/******************************************************************************
 *  File: IPS2550_WebApp_Bluetooth.h
 *  Description: Reads analog signals (sin and cos) from rotary encoder and
 *               calculates angle of rotation in degrees.
 *  Target: ESP32-S3-DEV-KIT-N16R8-M
 *  Date: 2025/XX/XX
 *  License: MinebeaMitsumi Inc.
 ******************************************************************************/

#ifndef IPS2550_WEBAPP_BLUETOOTH_H
#define IPS2550_WEBAPP_BLUETOOTH_H

//==============================================================================
// Includes for Bluetooth (BLE) functionality
//==============================================================================

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <esp_bt.h>

//==============================================================================
// UUID Definitions for BLE Service and Characteristic
//==============================================================================

#define SERVICE_UUID           "12345678-1234-5678-1234-56789abcdef0"  // UUID for BLE service
#define CHARACTERISTIC_UUID    "12345678-1234-5678-1234-56789abcdef1"  // UUID for BLE characteristic

#endif // IPS2550_WEBAPP_BLUETOOTH_H
