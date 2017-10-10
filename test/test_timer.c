#include "unity.h"
#include <time.h>
#include <stdlib.h>

/*******************************************************************************
* MODULE UNDER TEST
*******************************************************************************/
#include "timer.h"

/*******************************************************************************
* Local Data
*******************************************************************************/
timer_d_t timer_handle;

// fake registers
uint8_t tccr = 0;
uint8_t tcnt = 0;
uint8_t tifr = 0;
uint8_t timsk = 0;

/*******************************************************************************
* Setup and Teardown
*******************************************************************************/
void setUp(void)
{
  srand(time(NULL));

  tccr = (uint8_t)rand();
  tcnt = (uint8_t)rand();
  tifr = (uint8_t)rand();
  timsk = (uint8_t)rand();

  timer_attr_t config;
  config.tccr = &tccr;
  config.tcnt = &tcnt;
  config.tifr = &tifr;
  config.timsk = &timsk;
  config.timer_id = 0;
  config.prescale = TIMER_PRESCALE_0;
  config.isr_trigger = TIMER_ISR_TRIGGER_OVERFLOW;
  timer_construct(config, &timer_handle);
}

void tearDown(void)
{
}

void test_construct_initializes_tccr(void)
{
  TEST_ASSERT(tccr == 0);
}

void test_construct_initializes_tcnt(void)
{
  TEST_ASSERT(tcnt == 0);
}

void test_construct_initializes_timsk(void)
{
  TEST_ASSERT(timsk == 1);
}
