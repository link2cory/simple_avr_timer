#include "timer.h"

/*******************************************************************************
* Private Defines
*******************************************************************************/
#define NUM_TIMERS 3

/*******************************************************************************
* Private Typedefs
*******************************************************************************/
typedef struct timer_t {
  uint8_t *tccr;
  uint8_t *tcnt;
  uint8_t *tifr;

  timer_prescale_t prescale;
  timer_isr_trigger_t isr_trigger;
  timer_status_t status;
  void (*callback)(void);
} timer_t;
/*******************************************************************************
* Private Data
*******************************************************************************/
static timer_t timers[NUM_TIMERS];
static uint8_t *_timsk;
/*******************************************************************************
* Private Function Declarations
*******************************************************************************/
void _setInterruptTrigger(timer_id_t id, timer_isr_trigger_t trigger);

/*******************************************************************************
* Public Function Definitions
*******************************************************************************/
void timer_init(uint8_t *timsk) {
    _timsk = timsk;
    *_timsk = 0;
}

timer_err_t timer_construct(timer_attr_t config) {
  timer_err_t err = TIMER_ERR_NONE;
  if (timers[config.timer_id].status == TIMER_STATUS_READY) {
    timers[config.timer_id].tccr = config.tccr;
    timers[config.timer_id].tcnt = config.tcnt;
    timers[config.timer_id].tifr = config.tifr;
    _timsk = config.timsk;

    timers[config.timer_id].prescale = config.prescale;
    timers[config.timer_id].isr_trigger = config.isr_trigger;

    *timers[config.timer_id].tccr = 0 | (config.prescale);
    *timers[config.timer_id].tcnt = 0;

    _setInterruptTrigger(config.timer_id, config.isr_trigger);

    timers[config.timer_id].status = TIMER_STATUS_BUSY;

    timers[config.timer_id].callback = config.callback;
  } else {
    err = TIMER_ERR_TIMER_IN_USE;
  }
  return err;
}

void timer_destruct(timer_id_t timer_id) {
  timers[timer_id].status = TIMER_STATUS_READY;
}

/*******************************************************************************
* Private Function Definitions
*******************************************************************************/
void _setInterruptTrigger(timer_id_t id, timer_isr_trigger_t trigger) {
  switch(id) {
    case TIMER_ID_0:
      *_timsk |= (trigger << 0);
    break;
    case TIMER_ID_1:
      *_timsk |= (trigger << 2);
    break;
    case TIMER_ID_2:
      *_timsk |= (trigger << 6);
    break;
  }
}

/*******************************************************************************
* Interrupt Service Routines
*******************************************************************************/
#ifndef TEST

ISR (TIMER0_COMPA_vect) {
  if (timers[TIMER_ID_0].callback != 0) {
    timers[TIMER_ID_0].callback();
  }
}

ISR (TIMER1_COMPA_vect) {
  if (timers[TIMER_ID_1].callback != 0) {
    timers[TIMER_ID_1].callback();
  }
}

ISR (TIMER2_COMPA_vect) {
  if (timers[TIMER_ID_2].callback != 0) {
    timers[TIMER_ID_2].callback();
  }
}

ISR(TIMER0_OVF_vect) {
  if (timers[TIMER_ID_0].callback != 0) {
    timers[TIMER_ID_0].callback();
  }
}

ISR(TIMER1_0VF_vect) {
  if (timers[TIMER_ID_1].callback != 0) {
    timers[TIMER_ID_1].callback();
  }
}

ISR(TIMER2_0VF_vect) {
  if (timers[TIMER_ID_2].callback != 0) {
    timers[TIMER_ID_2].callback();
  }
}

#endif
