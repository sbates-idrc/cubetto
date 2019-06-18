/**
* This file is autogenerated by nRFgo Studio 1.19.0.7
*/

#ifndef SETUP_MESSAGES_H__
#define SETUP_MESSAGES_H__

#include "hal_platform.h"
#include "aci.h"

// You have now chosen to upload the configuration to OTP on the device.
// This will result in a device that you can not modify afterwards. If this is your intention,
// remove this comment and the #error below
#error Generating configuration for OTP. Please verify usage by removing this error message from include file.

#define SETUP_ID 0
#define SETUP_FORMAT 3 /** nRF8001 D */
#define ACI_DYNAMIC_DATA_SIZE 195

/* Service: Gap - Characteristic: Device name - Pipe: SET */
#define PIPE_GAP_DEVICE_NAME_SET          1
#define PIPE_GAP_DEVICE_NAME_SET_MAX_SIZE 12

/* Service: Battery - Characteristic: Battery Level - Pipe: TX */
#define PIPE_BATTERY_BATTERY_LEVEL_TX          2
#define PIPE_BATTERY_BATTERY_LEVEL_TX_MAX_SIZE 1

/* Service: Battery - Characteristic: Battery Level - Pipe: SET */
#define PIPE_BATTERY_BATTERY_LEVEL_SET          3
#define PIPE_BATTERY_BATTERY_LEVEL_SET_MAX_SIZE 1

/* Service: Primo - Characteristic: Command - Pipe: RX_ACK_AUTO */
#define PIPE_PRIMO_COMMAND_RX_ACK_AUTO          4
#define PIPE_PRIMO_COMMAND_RX_ACK_AUTO_MAX_SIZE 4

/* Service: Primo - Characteristic: Response - Pipe: TX */
#define PIPE_PRIMO_RESPONSE_TX          5
#define PIPE_PRIMO_RESPONSE_TX_MAX_SIZE 4

/* Service: Primo - Characteristic: IO - Pipe: TX */
#define PIPE_PRIMO_IO_TX          6
#define PIPE_PRIMO_IO_TX_MAX_SIZE 7

/* Service: Primo - Characteristic: UID - Pipe: TX */
#define PIPE_PRIMO_UID_TX          7
#define PIPE_PRIMO_UID_TX_MAX_SIZE 20

/* Service: Primo - Characteristic: UID - Pipe: SET */
#define PIPE_PRIMO_UID_SET          8
#define PIPE_PRIMO_UID_SET_MAX_SIZE 20


#define NUMBER_OF_PIPES 8

#define SERVICES_PIPE_TYPE_MAPPING_CONTENT {\
  {ACI_STORE_LOCAL, ACI_SET},   \
  {ACI_STORE_LOCAL, ACI_TX},   \
  {ACI_STORE_LOCAL, ACI_SET},   \
  {ACI_STORE_LOCAL, ACI_RX_ACK_AUTO},   \
  {ACI_STORE_LOCAL, ACI_TX},   \
  {ACI_STORE_LOCAL, ACI_TX},   \
  {ACI_STORE_LOCAL, ACI_TX},   \
  {ACI_STORE_LOCAL, ACI_SET},   \
}

#define GAP_PPCP_MAX_CONN_INT 0xc8 /**< Maximum connection interval as a multiple of 1.25 msec , 0xFFFF means no specific value requested */
#define GAP_PPCP_MIN_CONN_INT  0xa0 /**< Minimum connection interval as a multiple of 1.25 msec , 0xFFFF means no specific value requested */
#define GAP_PPCP_SLAVE_LATENCY 0
#define GAP_PPCP_CONN_TIMEOUT 0xffff /** Connection Supervision timeout multiplier as a multiple of 10msec, 0xFFFF means no specific value requested */

/** @brief do a set_local_data for PIPE_GAP_DEVICE_NAME_SET
 *  @param src source buffer to send data from
 *  @param size the number of bytes to send. Maximum size is 12
 *  @details use this function to do a set_local_data for PIPE_GAP_DEVICE_NAME_SET. If no transaction are currently
 *  running, the set will be immediate, otherwise, it will be done at the end of the current transaction
 *  when services_update_pipes will be called.
 */
void services_set_gap_device_name(void *src, int size);

/** @brief send a new value for PIPE_BATTERY_BATTERY_LEVEL_TX
 *  @param src the value to send
 *  @param is_freshest_sample set it to true if you want to overwrite an eventual pending transaction on this pipe.
 *  @details use this function to send a new value for PIPE_BATTERY_BATTERY_LEVEL_TX. If no transaction are currently
 *  running, the send will be immediate, otherwise, it will be done at the end of the current transaction
 *  when services_update_pipes will be called. If a transaction on this pipe is already pending, then this function
 *  will not overwrite the data of the previous transaction and return false.
 *  @return : true if is_freshest_sample true, otherwise return false if a transaction on this pipe is already pending, true otherwise.
 */
bool services_send_battery_battery_level(uint8_t src, bool is_freshest_sample);

/** @brief do a set_local_data for PIPE_BATTERY_BATTERY_LEVEL_SET
 *  @param src the value to send
 *  @details use this function to do a set_local_data for PIPE_BATTERY_BATTERY_LEVEL_SET. If no transaction are currently
 *  running, the set will be immediate, otherwise, it will be done at the end of the current transaction
 *  when services_update_pipes will be called.
 */
void services_set_battery_battery_level(uint8_t src);

/** @brief send a new value for PIPE_PRIMO_RESPONSE_TX
 *  @param src source buffer to send data from
 *  @param size the number of bytes to send. Maximum size is 4
 *  @param is_freshest_sample set it to true if you want to overwrite an eventual pending transaction on this pipe.
 *  @details use this function to send a new value for PIPE_PRIMO_RESPONSE_TX. If no transaction are currently
 *  running, the send will be immediate, otherwise, it will be done at the end of the current transaction
 *  when services_update_pipes will be called. If a transaction on this pipe is already pending, then this function
 *  will not overwrite the data of the previous transaction and return false.
 *  @return : true if is_freshest_sample true, otherwise return false if a transaction on this pipe is already pending, true otherwise.
 */
bool services_send_primo_response(void *src, int size, bool is_freshest_sample);

/** @brief send a new value for PIPE_PRIMO_IO_TX
 *  @param src source buffer to send data from
 *  @param size the number of bytes to send. Maximum size is 7
 *  @param is_freshest_sample set it to true if you want to overwrite an eventual pending transaction on this pipe.
 *  @details use this function to send a new value for PIPE_PRIMO_IO_TX. If no transaction are currently
 *  running, the send will be immediate, otherwise, it will be done at the end of the current transaction
 *  when services_update_pipes will be called. If a transaction on this pipe is already pending, then this function
 *  will not overwrite the data of the previous transaction and return false.
 *  @return : true if is_freshest_sample true, otherwise return false if a transaction on this pipe is already pending, true otherwise.
 */
bool services_send_primo_io(void *src, int size, bool is_freshest_sample);

/** @brief send a new value for PIPE_PRIMO_UID_TX
 *  @param src source buffer to send data from
 *  @param size the number of bytes to send. Maximum size is 20
 *  @param is_freshest_sample set it to true if you want to overwrite an eventual pending transaction on this pipe.
 *  @details use this function to send a new value for PIPE_PRIMO_UID_TX. If no transaction are currently
 *  running, the send will be immediate, otherwise, it will be done at the end of the current transaction
 *  when services_update_pipes will be called. If a transaction on this pipe is already pending, then this function
 *  will not overwrite the data of the previous transaction and return false.
 *  @return : true if is_freshest_sample true, otherwise return false if a transaction on this pipe is already pending, true otherwise.
 */
bool services_send_primo_uid(void *src, int size, bool is_freshest_sample);

/** @brief do a set_local_data for PIPE_PRIMO_UID_SET
 *  @param src source buffer to send data from
 *  @param size the number of bytes to send. Maximum size is 20
 *  @details use this function to do a set_local_data for PIPE_PRIMO_UID_SET. If no transaction are currently
 *  running, the set will be immediate, otherwise, it will be done at the end of the current transaction
 *  when services_update_pipes will be called.
 */
void services_set_primo_uid(void *src, int size);

/** @brief function to trig pending transaction on pipes
 *  @details This function check for each pipe if it has a pending transaction (send/rx_request/ack)
 *   and if so executes this transaction.
 *   This function should be called in the APP_RUN state of the process function of the application.
 */
void services_update_pipes(void);

#define NB_SETUP_MESSAGES 26
#define SETUP_MESSAGES_CONTENT {\
    {0x00,\
        {\
            0x07,0x06,0x00,0x00,0x03,0x02,0x42,0x07,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x08,0x00,0x01,0x00,0x00,0x06,0x00,0x01,\
            0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x10,0x1c,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x03,0x90,0x01,0xff,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x10,0x38,0xff,0xff,0x02,0x58,0x0a,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,\
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x05,0x06,0x10,0x54,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x00,0x04,0x04,0x02,0x02,0x00,0x01,0x28,0x00,0x01,0x00,0x18,0x04,0x04,0x05,0x05,0x00,\
            0x02,0x28,0x03,0x01,0x0e,0x03,0x00,0x00,0x2a,0x04,0x14,0x0c,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x1c,0x07,0x00,0x03,0x2a,0x00,0x01,0x43,0x75,0x62,0x65,0x74,0x74,0x6f,0x65,0x6d,0x69,\
            0x2e,0x63,0x04,0x04,0x05,0x05,0x00,0x04,0x28,0x03,0x01,0x02,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x38,0x05,0x00,0x01,0x2a,0x06,0x04,0x03,0x02,0x00,0x05,0x2a,0x01,0x01,0x00,0x00,0x04,\
            0x04,0x05,0x05,0x00,0x06,0x28,0x03,0x01,0x02,0x07,0x00,0x04,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x54,0x2a,0x06,0x04,0x09,0x08,0x00,0x07,0x2a,0x04,0x01,0xa0,0x00,0xc8,0x00,0x00,0x00,\
            0xff,0xff,0x04,0x04,0x02,0x02,0x00,0x08,0x28,0x00,0x01,0x01,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x70,0x18,0x04,0x04,0x02,0x02,0x00,0x09,0x28,0x00,0x01,0x0f,0x18,0x04,0x04,0x05,0x05,\
            0x00,0x0a,0x28,0x03,0x01,0x12,0x0b,0x00,0x19,0x2a,0x16,0x04,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x8c,0x02,0x01,0x00,0x0b,0x2a,0x19,0x01,0x10,0x46,0x14,0x03,0x02,0x00,0x0c,0x29,0x02,\
            0x01,0x00,0x00,0x04,0x04,0x10,0x10,0x00,0x0d,0x28,0x00,0x01,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0xa8,0x13,0x15,0x5c,0xd6,0xda,0xbf,0x3f,0x8b,0x0a,0x46,0x52,0xff,0x00,0x00,0x84,0x02,\
            0x04,0x04,0x13,0x13,0x00,0x0e,0x28,0x03,0x01,0x0a,0x0f,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0xc4,0x13,0x15,0x5c,0xd6,0xda,0xbf,0x3f,0x8b,0x0a,0x46,0x52,0xff,0x01,0x00,0x84,0x02,\
            0x46,0x14,0x05,0x04,0x00,0x0f,0x00,0x01,0x02,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0xe0,0x00,0x04,0x04,0x13,0x13,0x00,0x10,0x28,0x03,0x01,0x12,0x11,0x00,0x13,0x15,0x5c,\
            0xd6,0xda,0xbf,0x3f,0x8b,0x0a,0x46,0x52,0xff,0x02,0x00,0x84,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0xfc,0x02,0x16,0x04,0x05,0x04,0x00,0x11,0x00,0x02,0x02,0x00,0x00,0x00,0x00,0x46,0x14,\
            0x03,0x02,0x00,0x12,0x29,0x02,0x01,0x00,0x00,0x04,0x04,0x13,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x21,0x18,0x13,0x00,0x13,0x28,0x03,0x01,0x12,0x14,0x00,0x13,0x15,0x5c,0xd6,0xda,0xbf,0x3f,\
            0x8b,0x0a,0x46,0x52,0xff,0x03,0x00,0x84,0x02,0x16,0x04,0x08,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x21,0x34,0x07,0x00,0x14,0x00,0x03,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x46,0x14,0x03,\
            0x02,0x00,0x15,0x29,0x02,0x01,0x00,0x00,0x04,0x04,0x13,0x13,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x21,0x50,0x00,0x16,0x28,0x03,0x01,0x12,0x17,0x00,0x13,0x15,0x5c,0xd6,0xda,0xbf,0x3f,0x8b,\
            0x0a,0x46,0x52,0xff,0x04,0x00,0x84,0x02,0x14,0x04,0x14,0x01,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x21,0x6c,0x00,0x17,0x00,0x04,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x46,0x14,0x03,\
        },\
    },\
    {0x00,\
        {\
            0x0c,0x06,0x21,0x88,0x02,0x00,0x18,0x29,0x02,0x01,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x40,0x00,0x2a,0x00,0x01,0x00,0x80,0x04,0x00,0x03,0x00,0x00,0x2a,0x19,0x01,0x00,0x82,0x04,\
            0x00,0x0b,0x00,0x0c,0x00,0x01,0x02,0x04,0x00,0x04,0x00,0x0f,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x40,0x1c,0x00,0x00,0x00,0x02,0x02,0x00,0x02,0x04,0x00,0x11,0x00,0x12,0x00,0x03,0x02,0x00,\
            0x02,0x04,0x00,0x14,0x00,0x15,0x00,0x04,0x02,0x00,0x82,0x04,\
        },\
    },\
    {0x00,\
        {\
            0x07,0x06,0x40,0x38,0x00,0x17,0x00,0x18,\
        },\
    },\
    {0x00,\
        {\
            0x13,0x06,0x50,0x00,0x13,0x15,0x5c,0xd6,0xda,0xbf,0x3f,0x8b,0x0a,0x46,0x52,0xff,0x00,0x00,0x84,0x02,\
        },\
    },\
    {0x00,\
        {\
            0x15,0x06,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
            0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x06,0x06,0xf0,0x00,0x83,0xc1,0x88,\
        },\
    },\
}

#endif