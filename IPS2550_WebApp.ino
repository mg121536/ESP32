/******************************************************************************
 *  File: IPS2550_WebApp.ino
 *  Description: Reads analog signals (sin and cos) from rotary encoder and
 *               calculates angle of rotation in degrees.
 *  Target: ESP32-S3-DEV-KIT-N16R8-M
 *  Date: 2025/XX/XX
 *  License: MinebeaMitsumi Inc.
 ******************************************************************************/

#include <ESP_I2S.h>
#include <wav_header.h>
#include <angle_reader_cfg.h>

/* Constants for analog signal range and pins */
#define VAL_ANALOG_RANGE        (ESP32_CFG_TEST_RANGE)     // Analog range
#define VAL_ANALOG_RANGE_MID    (VAL_ANALOG_RANGE / 2)     // Midpoint for centering

const int A_pin = ESP32_CFG_PORT_GPIO_COS;  // GPIO for Cos signal input
const int B_pin = ESP32_CFG_PORT_GPIO_SIN;  // GPIO for Sin signal input

/* Variables to store analog values */
int A_val = 0U;         // Cos analog value
int B_val = 0U;         // Sin analog value
#if ESP32_DEBUG
int A_val_max = 0U;     // Max recorded value for Cos
int B_val_max = 0U;     // Max recorded value for Sin
int A_val_min = 4095U;  // Min recorded value for Cos
int B_val_min = 4095U;  // Min recorded value for Sin
#endif /* ESP32_DEBUG */

/* Angle calculations */
float angle          = 0.0;  // Current angle (in degrees)
float pre_angle      = 0.0;  // Previous angle
float rota_dif_angle = 0.0;  // Angle difference between reads
float mult_angle     = 0.0;  // Accumulated angle
float Angle          = 0.0;  // Final output angle

/**
 * @brief Setup function for ESP32
 *        Initializes serial communication and ADC resolution
 */
void setup()
{
    delay(ESP32_CFG_SETUP_DELAY);              // Wait for 15 second
    Serial.begin(ESP32_CFG_SERIAL_BAUDRATE);   // Start serial communication
    analogReadResolution(12);                  // Set ADC resolution to 12 bits
}

/**
 * @brief Main loop function
 *        Reads analog signals, calculates angle using atan2, 
 *        detects direction of rotation, and calculates cumulative angle.
 */
void loop()
{
    // Read analog values from A and B pins
    A_val = analogRead(A_pin);
    B_val = analogRead(B_pin);
#if ESP32_DEBUG
    if (A_val > A_val_max){A_val_max = A_val;}
    if (A_val < A_val_min){A_val_min = A_val;}
    if (B_val > B_val_max){B_val_max = B_val;}
    if (B_val < B_val_min){B_val_min = B_val;}
#endif /* ESP32_DEBUG */

    // Calculate angle in degrees (centered around 0)
    angle = atan2((float)(B_val - VAL_ANALOG_RANGE_MID), (float)(A_val - VAL_ANALOG_RANGE_MID));
    angle = angle * 180.0 / 3.141592653589793;  // Convert radians to degrees

    // Calculate rotational difference based on previous angle
    if ((angle > 0) && (angle < 45) && (pre_angle > 135) && (pre_angle < 180))
    {
        // Wrap-around positive rotation
        rota_dif_angle = angle + (180 - pre_angle);
    }
    else if ((angle > 135) && (angle < 180) && (pre_angle > 0) && (pre_angle < 45))
    {
        // Wrap-around negative rotation
        rota_dif_angle = -angle - (180 - pre_angle);
    }
    else if (angle > pre_angle)
    {
        // Normal forward rotation
        rota_dif_angle = angle - pre_angle;
    }
    else if (angle < pre_angle)
    {
        // Normal reverse rotation
        rota_dif_angle = pre_angle - angle;
    }
    else
    {
        // No change
        rota_dif_angle = 0;
    }

    mult_angle += rota_dif_angle;
    pre_angle   = angle;

    // Normalize accumulated angle
    // In a 4x system, 1 cycle = 90 deg, 4 cycles = 360 deg
    if (mult_angle > 640)
    {
        mult_angle -= 640;
    }
    else if (mult_angle < 0)
    {
        mult_angle += 180;
    }

    Angle = mult_angle / 4;  // Final angle output

    // Debug output to serial
#if ESP32_DEBUG
    Serial.print("A_max:");
    Serial.print(A_val_max);
    Serial.print(", A_min:");
    Serial.print(A_val_min);
    Serial.print(", B_max:");
    Serial.print(B_val_max);
    Serial.print(", B_min:");
    Serial.print(B_val_min);
    Serial.print(", Angle:");
    Serial.print(Angle);
    Serial.print(", ");
#endif /* ESP32_DEBUG */
    Serial.print("A:");
    Serial.print(A_val);
    Serial.print(" B:");
    Serial.println(B_val);

    delay(10);
}
