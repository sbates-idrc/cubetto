/* Copyright (c) 2014, Nordic Semiconductor ASA
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/** @file
@brief Implementation of the ACI transport layer module
*/



#include <SPI.h>
#include "hal_platform.h"
#include "hal_aci_tl.h"
#include "aci_queue.h"


static void m_aci_event_check(void);
static void m_aci_pins_set(aci_pins_t *a_pins_ptr);
static inline void m_aci_reqn_disable (void);
static inline void m_aci_reqn_enable (void);
static void m_aci_q_flush(void);
static bool m_aci_spi_transfer(hal_aci_data_t * data_to_send, hal_aci_data_t * received_data);


static uint8_t        spi_readwrite(uint8_t aci_byte);
uint8_t 	spiByteRxFlag = 0;
uint8_t 	rxByte = 0;

aci_queue_t    aci_tx_q;
aci_queue_t    aci_rx_q;

static aci_pins_t	 *a_pins_local_ptr;

extern int8_t HAL_IO_RADIO_RESET, HAL_IO_RADIO_REQN, HAL_IO_RADIO_RDY, HAL_IO_RADIO_IRQ;
 
/** ************************************************************************************************************************************
 * @function 	
 * @brief 		
 ***************************************************************************************************************************************/
static void m_aci_event_check(void)
{
  hal_aci_data_t data_to_send;
  hal_aci_data_t received_data;

  // No room to store incoming messages
  if (aci_queue_is_full(&aci_rx_q))
  {
    return;
  }

  // If the ready line is disabled and we have pending messages outgoing we enable the request line
  if (digitalRead(HAL_IO_RADIO_RDY))
  {
    if (!aci_queue_is_empty(&aci_tx_q))
    {
      m_aci_reqn_enable();
    }
    return;
  }

  // Receive from queue
  if (!aci_queue_dequeue(&aci_tx_q, &data_to_send))
  {
    /* queue was empty, nothing to send */
    data_to_send.status_byte = 0;
    data_to_send.buffer[0] = 0;
  }

  // Receive and/or transmit data
  m_aci_spi_transfer(&data_to_send, &received_data);

  /* If there are messages to transmit, and we can store the reply, we request a new transfer */
  if (!aci_queue_is_full(&aci_rx_q) && !aci_queue_is_empty(&aci_tx_q))
  {
    m_aci_reqn_enable();
  }

  // Check if we received data
  if (received_data.buffer[0] > 0)
  {
    if (!aci_queue_enqueue(&aci_rx_q, &received_data))
    {
      /* Receive Buffer full.
         Should never happen.
         Spin in a while loop.
      */
      while(1);
    }
  }

  return;
}

/** ************************************************************************************************************************************
 * @function 	
 * @brief 		
 ***************************************************************************************************************************************/
static void m_aci_pins_set(aci_pins_t *a_pins_ptr)
{
  a_pins_local_ptr = a_pins_ptr;
}

/** ************************************************************************************************************************************
 * @function 	
 * @brief 		
 ***************************************************************************************************************************************/
static inline void m_aci_reqn_disable (void)
{
	digitalWrite(HAL_IO_RADIO_REQN, HIGH); 
}

/** ************************************************************************************************************************************
 * @function 	
 * @brief 		
 ***************************************************************************************************************************************/
static inline void m_aci_reqn_enable (void)
{
	digitalWrite(HAL_IO_RADIO_REQN, LOW);  
}

/** ************************************************************************************************************************************
 * @function 	
 * @brief 		
 ***************************************************************************************************************************************/
static void m_aci_q_flush(void)
{
  aci_queue_init(&aci_tx_q);
  aci_queue_init(&aci_rx_q);
}

/** ************************************************************************************************************************************
 * @function 	
 * @brief 		
 ***************************************************************************************************************************************/
static bool m_aci_spi_transfer(hal_aci_data_t * data_to_send, hal_aci_data_t * received_data)
{
  uint8_t byte_cnt;
  uint8_t byte_sent_cnt;
  uint8_t max_bytes;

	
	
	//SPI.beginTransaction(SPISettings(1000000, LSBFIRST, SPI_MODE0));  // gain control of SPI bus
  m_aci_reqn_enable();

  // Send length, receive header
  byte_sent_cnt = 0;
  received_data->status_byte = spi_readwrite(data_to_send->buffer[byte_sent_cnt++]);
  // Send first byte, receive length from slave
  received_data->buffer[0] = spi_readwrite(data_to_send->buffer[byte_sent_cnt++]);
  if (0 == data_to_send->buffer[0])
  {
    max_bytes = received_data->buffer[0];
  }
  else
  {
    // Set the maximum to the biggest size. One command byte is already sent
    max_bytes = (received_data->buffer[0] > (data_to_send->buffer[0] - 1))
                                          ? received_data->buffer[0]
                                          : (data_to_send->buffer[0] - 1);
  }

  if (max_bytes > HAL_ACI_MAX_LENGTH)
  {
    max_bytes = HAL_ACI_MAX_LENGTH;
  }

  // Transmit/receive the rest of the packet
  for (byte_cnt = 0; byte_cnt < max_bytes; byte_cnt++)
  {
    received_data->buffer[byte_cnt+1] =  spi_readwrite(data_to_send->buffer[byte_sent_cnt++]);
  }

  // RDYN should follow the REQN line in approx 100ns
  m_aci_reqn_disable();
	// SPI.endTransaction();              // release the SPI bus

  return (max_bytes > 0);
}

/** ************************************************************************************************************************************
 * @function 	
 * @brief 		
 ***************************************************************************************************************************************/

void hal_aci_tl_pin_reset(void)
{
	
	digitalWrite(HAL_IO_RADIO_RESET, 1);
  delay(100);
  digitalWrite(HAL_IO_RADIO_RESET, 0);
	delay(100);
	digitalWrite(HAL_IO_RADIO_RESET, 1);
}

/** ************************************************************************************************************************************
 * @function 	
 * @brief 		
 ***************************************************************************************************************************************/

bool hal_aci_tl_event_peek(hal_aci_data_t *p_aci_data)
{
  m_aci_event_check();

  if (aci_queue_peek(&aci_rx_q, p_aci_data))
  {
    return true;
  }
  return false;
}

/** ************************************************************************************************************************************
 * @function 	
 * @brief 		
 ***************************************************************************************************************************************/

bool hal_aci_tl_event_get(hal_aci_data_t *p_aci_data)
{
  bool was_full;

  if (!aci_queue_is_full(&aci_rx_q))
  {
    m_aci_event_check();
  }

  was_full = aci_queue_is_full(&aci_rx_q);

  if (aci_queue_dequeue(&aci_rx_q, p_aci_data))
  {
    /* Attempt to pull REQN LOW since we've made room for new messages */
    if (!aci_queue_is_full(&aci_rx_q) && !aci_queue_is_empty(&aci_tx_q))
    {
      m_aci_reqn_enable();
    }
    return true;
  }

  return false;
}

/** ************************************************************************************************************************************
 * @function 	
 * @brief 		
 ***************************************************************************************************************************************/
void hal_aci_tl_init(aci_pins_t *a_pins, bool debug)
{
 
	// extern int8_t HAL_IO_RADIO_RESET, HAL_IO_RADIO_REQN, HAL_IO_RADIO_RDY, HAL_IO_RADIO_IRQ;
	
	// Config Interfaces
	pinMode (HAL_IO_RADIO_RESET, OUTPUT); 
	pinMode (HAL_IO_RADIO_REQN, OUTPUT); 
	pinMode (HAL_IO_RADIO_RDY, INPUT_PULLUP); 

	
	digitalWrite(HAL_IO_RADIO_REQN,   1);
	delay(10); 
	
	
  /* Needs to be called as the first thing for proper intialization*/
  m_aci_pins_set(a_pins);

  /* Initialize the ACI Command queue. This must be called after the delay above. */
  aci_queue_init(&aci_tx_q);
  aci_queue_init(&aci_rx_q);

  /* Pin reset the nRF8001, required when the nRF8001 setup is being changed */
  hal_aci_tl_pin_reset();
	
	delay(50); 
}

/** ************************************************************************************************************************************
 * @function 	
 * @brief 		
 ***************************************************************************************************************************************/
bool hal_aci_tl_send(hal_aci_data_t *p_aci_cmd)
{
  const uint8_t length = p_aci_cmd->buffer[0];
  bool ret_val = false;

  if (length > HAL_ACI_MAX_LENGTH)
  {
    return false;
  }

  ret_val = aci_queue_enqueue(&aci_tx_q, p_aci_cmd);
  if (ret_val)
  {
    if(!aci_queue_is_full(&aci_rx_q))
    {
      // Lower the REQN only when successfully enqueued
      m_aci_reqn_enable();
    }
  }

  return ret_val;
}

/** ************************************************************************************************************************************
 * @function 	
 * @brief 		
 ***************************************************************************************************************************************/
uint8_t spi_readwrite(const uint8_t aci_byte)
{
	uint8_t data = SPI.transfer(aci_byte);
	return data; 
}


/** ************************************************************************************************************************************
 * @function 	
 * @brief 		
 ***************************************************************************************************************************************/
bool hal_aci_tl_rx_q_empty (void)
{
  return aci_queue_is_empty(&aci_rx_q);
}
/** ************************************************************************************************************************************
 * @function 	
 * @brief 		
 ***************************************************************************************************************************************/
bool hal_aci_tl_rx_q_full (void)
{
  return aci_queue_is_full(&aci_rx_q);
}
/** ************************************************************************************************************************************
 * @function 	
 * @brief 		
 ***************************************************************************************************************************************/
bool hal_aci_tl_tx_q_empty (void)
{
  return aci_queue_is_empty(&aci_tx_q);
}
/** ************************************************************************************************************************************
 * @function 	
 * @brief 		
 ***************************************************************************************************************************************/
bool hal_aci_tl_tx_q_full (void)
{
  return aci_queue_is_full(&aci_tx_q);
}
/** ************************************************************************************************************************************
 * @function 	
 * @brief 		
 ***************************************************************************************************************************************/
void hal_aci_tl_q_flush (void)
{
  m_aci_q_flush();
}


