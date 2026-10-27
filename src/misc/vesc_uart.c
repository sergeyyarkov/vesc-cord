#include "vesc_uart.h"
#include "hal/usart.h"
#include "utils/crc.h"
#include "utils/ring_buf.h"

typedef enum VescResponseParseStateEnum {
  STATE_START,
  STATE_LEN,
  STATE_LEN_2,
  STATE_PAYLOAD,
  STATE_CRC_H,
  STATE_CRC_L,
  STATE_STOP
} VescResponseParseStateEnum_t;

#define VESC_UART_PACKET_START_BYTE_SHORT 0x02
#define VESC_UART_PACKET_END_BYTE 0x03
#define VESC_UART_PACKET_COMM_GET_VALUES_ID 0x04

VescTelemetry_t VescTelemetryValues;

static void VescTelemetry_RequestDataPacket(void) {
  uint8_t payload = VESC_UART_PACKET_COMM_GET_VALUES_ID;
  uint16_t crc = crc16(&payload, 1);
  uint8_t packet[6];

  packet[0] = VESC_UART_PACKET_START_BYTE_SHORT;
  packet[1] = 0x01;  // packet length
  packet[2] = payload;
  packet[3] = crc >> 8;
  packet[4] = crc & 0xff;
  packet[5] = VESC_UART_PACKET_END_BYTE;

  for (uint8_t i = 0; i < 6; i++) {
    USART_SendChar(packet[i]);
  }
}

void VescTelemetry_Update(void) {
  RingBuffer_t *rx_buf = USART_GetRxBuffer();
  static VescResponseParseStateEnum_t response_parse_state = STATE_START;
  static uint8_t byte;
  static uint16_t response_crc;
  static uint8_t response_data[128] = {0};
  static uint8_t response_wr_idx = 0;
  static uint8_t response_length = 0;

  if (response_parse_state == STATE_START) {
    USART_SendStr("dbg: vesc_uart - request data packet.\r\n");
    VescTelemetry_RequestDataPacket();
  }

  while (USART_IsRxBufferAvailable()) {
    byte = RingBuffer_Read(rx_buf);

    switch (response_parse_state) {
      case STATE_START:
        if (byte == VESC_UART_PACKET_START_BYTE_SHORT) {
          response_parse_state = STATE_LEN;
        }

        break;
      case STATE_LEN:
        response_length = byte;
        response_parse_state = STATE_PAYLOAD;
        response_wr_idx = 0;
        break;
      case STATE_PAYLOAD:
        response_data[response_wr_idx++] = byte;

        if (response_wr_idx < response_length) {
          break;
        }

        response_parse_state = STATE_CRC_H;
        break;
      case STATE_CRC_H:
        response_crc = byte << 8;
        response_parse_state = STATE_CRC_L;
        break;
      case STATE_CRC_L:
        response_crc |= byte;
        response_parse_state = STATE_STOP;
        break;
      case STATE_STOP:
        if (byte == VESC_UART_PACKET_END_BYTE) {
          if (crc16(response_data, response_length) == response_crc) {
            if (response_data[0] == VESC_UART_PACKET_COMM_GET_VALUES_ID) {
              uint16_t vin_raw = (uint16_t)(((uint16_t)response_data[27] << 8) | response_data[28]);
              int32_t motor_curr_raw =
                  (int32_t)(((uint32_t)response_data[5] << 24) | ((uint32_t)response_data[6] << 16) |
                            ((uint32_t)response_data[7] << 8) | ((uint32_t)response_data[8]));
              int32_t input_curr_raw = 
                  (int32_t)(((uint32_t)response_data[9] << 24) | ((uint32_t)response_data[10] << 16) |
                            ((uint32_t)response_data[11] << 8) | ((uint32_t)response_data[12]));

              int16_t temp_mos_raw = (int16_t)(((uint16_t)response_data[1] << 8) | (uint16_t)response_data[2]);

              int32_t amp_hours_raw =
                  (int32_t)(((uint32_t)response_data[29] << 24) | ((uint32_t)response_data[30] << 16) |
                            ((uint32_t)response_data[31] << 8) | ((uint32_t)response_data[32]));

              VescTelemetryValues.current_in = (float)input_curr_raw / 100.0f;
              VescTelemetryValues.current_motor = (float)motor_curr_raw / 100.0f;
              VescTelemetryValues.v_in = (float)vin_raw / 10.0f;
              VescTelemetryValues.temp_mos = (float)temp_mos_raw / 10.0f;
              VescTelemetryValues.amp_hours = (float)amp_hours_raw / 10000.0f;
            }
          }
        }
        response_parse_state = STATE_START;
      default:
        break;
    }
  }
}

VescTelemetry_t *VescTelemetry_Get(void) { return &VescTelemetryValues; }