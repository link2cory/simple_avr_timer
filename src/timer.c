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
} timer_t;
/*******************************************************************************
* Private Data
*******************************************************************************/
timer_t timer;
/*******************************************************************************
* Private Function Declarations
*******************************************************************************/
/*******************************************************************************
* Public Function Definitions
*******************************************************************************/
void timer_construct(timer_attr_t config, timer_d_t *handle) {
  timer.tccr = config.tccr;
  timer.tcnt = config.tcnt;
  timer.tifr = config.tifr;
  timer.timsk = config.timsk;
  timer.prescale = config.prescale;

  *timer.tccr = 0 | (timer.prescale);
  *timer.tcnt = 0;
  *timer.timsk = 0;
}
