#ifndef _TIMER_H
#define _TIMER_H

#include <stdint.h>
/*******************************************************************************
* PUBLIC TYPEDEFS
*******************************************************************************/
typedef struct timer_attr_t {
  // register locations
  uint8_t *tccr;
  uint8_t *tcnt;
  uint8_t *tifr;
  uint8_t *timsk;

  uint8_t prescale;
  uint8_t timer_id;
} timer_attr_t;

typedef uint8_t timer_d_t;
/*******************************************************************************
* PUBLIC FUNCTION DECLARATIONS
*******************************************************************************/
void timer_construct(timer_attr_t config, timer_d_t *handle);
#endif // _TIMER_H
