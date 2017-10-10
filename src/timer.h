#ifndef _TIMER_H
#define _TIMER_H

#include <stdint.h>
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

typedef struct timer_attr_t {
  // register locations
  uint8_t *tccr;
  uint8_t *tcnt;
  uint8_t *tifr;
  uint8_t *timsk;

  timer_prescale_t prescale;
  uint8_t timer_id;
  timer_isr_trigger_t isr_trigger;
} timer_attr_t;

typedef uint8_t timer_d_t;


/*******************************************************************************
* PUBLIC FUNCTION DECLARATIONS
*******************************************************************************/
timer_err_t timer_construct(timer_attr_t config, timer_d_t *handle);
void timer_destruct(void);
#endif // _TIMER_H
