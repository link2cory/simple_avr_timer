#ifndef _TIMER_H
#define _TIMER_H

#include <stdint.h>

#ifndef TEST
  #include <avr/interrupt.h>
#endif
/*******************************************************************************
* PUBLIC TYPEDEFS
*******************************************************************************/
typedef enum timer_err_t {
  TIMER_ERR_NONE,
  TIMER_ERR_TIMER_IN_USE
} timer_err_t;

typedef enum timer_status_t {
  TIMER_STATUS_READY,
  TIMER_STATUS_BUSY
} timer_status_t;

typedef enum timer_prescale_t {
  TIMER_PRESCALE_0,
  TIMER_PRESCALE_1,
  TIMER_PRESCALE_8,
  TIMER_PRESCALE_64,
  TIMER_PRESCALE_256,
  TIMER_PRESCALE_1024,
  TIMER_PRESCALE_EXTERNAL
} timer_prescale_t;

typedef enum timer_isr_trigger_t {
  TIMER_ISR_TRIGGER_NONE,
  TIMER_ISR_TRIGGER_OVERFLOW,
  TIMER_ISR_TRIGGER_OUTPUT_COMPARE
} timer_isr_trigger_t;

typedef enum timer_id_t {
  TIMER_ID_0,
  TIMER_ID_1,
  TIMER_ID_2
} timer_id_t;

typedef enum timer_mode_t {
  TIMER_MODE_NORMAL = 0,
  TIMER_MODE_CTC = 2
} timer_mode_t;

typedef struct timer_attr_t {
  // register locations
  uint8_t *tccra;
  uint8_t *tccrb;
  uint8_t *tcnt;
  uint8_t *tifr;
  uint8_t *timsk;
  uint8_t *ocra_8;
  uint8_t *ocrb_8;
  uint16_t *ocra_16;
  uint16_t *ocrb_16;

  timer_prescale_t prescale;
  timer_id_t timer_id;
  timer_isr_trigger_t isr_trigger;
  timer_mode_t timer_mode;
  uint8_t ocr_val_a_8;
  uint8_t ocr_val_b_8;
  uint16_t ocr_val_a_16;
  uint16_t ocr_val_b_16;
  void *callback;
} timer_attr_t;


/*******************************************************************************
* PUBLIC FUNCTION DECLARATIONS
*******************************************************************************/
void timer_init(uint8_t *timsk);
timer_err_t timer_construct(timer_attr_t config);
void timer_destruct(timer_id_t timer_id);
#endif // _TIMER_H
