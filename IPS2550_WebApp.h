/******************************************************************************
 *  File: IPS2550_WebApp.h
 *  Description: Reads analog signals (sin and cos) from rotary encoder and
 *               calculates angle of rotation in degrees.
 *  Target: ESP32-S3-DEV-KIT-N16R8-M
 *  Date: 2025/XX/XX
 *  License: MinebeaMitsumi Inc.
 ******************************************************************************/

#ifndef IPS2550_WEBAPP_H
#define IPS2550_WEBAPP_H

//==============================================================================
// Constants for analog signal processing
//==============================================================================

#define VAL_ANALOG_RANGE        (ESP32_CFG_TEST_RANGE)      // Full analog input range
#define VAL_ANALOG_RANGE_MID    (VAL_ANALOG_RANGE / 2)      // Midpoint of analog range (zero reference)

#endif // IPS2550_WEBAPP_H
