/******************************************************************************
 *  File: angle_reader_cfg.h
 *  Description: Reads analog signals (sin and cos) from rotary encoder and
 *               calculates angle of rotation in degrees.
 *  Target: ESP32-S3-DEV-KIT-N16R8-M
 *  Date: 2025/XX/XX
 *  License: MinebeaMitsumi Inc.
 ******************************************************************************/

#define ESP32_DEBUG                  (0U)

#define ESP32_CFG_TEST_RANGE         (2398)

#define ESP32_CFG_SETUP_DELAY        (15000U)
#define ESP32_CFG_SERIAL_BAUDRATE    (115200U)

#define ESP32_CFG_PORT_GPIO_COS      (4U)
#define ESP32_CFG_PORT_GPIO_COSN     ()
#define ESP32_CFG_PORT_GPIO_SIN      (5U)
#define ESP32_CFG_PORT_GPIO_SINN     ()
