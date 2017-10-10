#include "timer.h"

/*******************************************************************************
* Private Typedefs
*******************************************************************************/
typedef struct timer_t {
  uint8_t *tccr;
  uint8_t *tcnt;
  uint8_t *tifr;
  uint8_t *timsk;

  timer_prescale_t prescale;
  timer_isr_trigger_t isr_trigger;
  timer_status_t status;
} timer_t;
/*******************************************************************************
* Private Data
*******************************************************************************/
static timer_t timers[3];
/*******************************************************************************
* Private Function Declarations
*******************************************************************************/
/*******************************************************************************
* Public Function Definitions
*******************************************************************************/
timer_err_t timer_construct(timer_attr_t config, timer_d_t *handle) {
  timer_err_t err = TIMER_ERR_NONE;
  if (timers[config.timer_id].status == TIMER_STATUS_READY) {
    timers[config.timer_id].tccr = config.tccr;
    timers[config.timer_id].tcnt = config.tcnt;
    timers[config.timer_id].tifr = config.tifr;
    timers[config.timer_id].timsk = config.timsk;

    timers[config.timer_id].prescale = config.prescale;
    timers[config.timer_id].isr_trigger = config.isr_trigger;

    *timers[config.timer_id].tccr = 0 | (config.prescale);
    *timers[config.timer_id].tcnt = 0;
    *timers[config.timer_id].timsk = 0 | config.isr_trigger;

    timers[config.timer_id].status = TIMER_STATUS_BUSY;
  } else {
    err = TIMER_ERR_TIMER_IN_USE;
  }
  return err;
}

void timer_destruct(void) {
  timers[0].status = TIMER_STATUS_READY;
}
