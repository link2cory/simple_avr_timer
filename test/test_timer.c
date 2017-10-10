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
// fake individual timer registers
uint8_t tccr_0;
uint8_t tcnt_0;
uint8_t tccr_1;
uint8_t tcnt_1;
uint8_t tccr_2;
uint8_t tcnt_2;

// fake common timer registers
uint8_t tifr;
static uint8_t timsk;

/*******************************************************************************
* Private Function Declarations
*******************************************************************************/
timer_err_t _make_timer(
  timer_id_t id,
  timer_prescale_t prescale,
  timer_isr_trigger_t isr_trigger
);

/*******************************************************************************
* Setup and Teardown
*******************************************************************************/
void setUp(void)
{
  srand(time(NULL));

  tccr_0 = (uint8_t)rand();
  tcnt_0 = (uint8_t)rand();
  tccr_1 = (uint8_t)rand();
  tcnt_1 = (uint8_t)rand();
  tccr_2 = (uint8_t)rand();
  tcnt_2 = (uint8_t)rand();

  tifr = (uint8_t)rand();
  timsk = (uint8_t)rand();

  timer_init(&timsk);

  // _make_timer(TIMER_ID_0, TIMER_PRESCALE_0, TIMER_ISR_TRIGGER_OVERFLOW);
}

void tearDown(void)
{
  timer_destruct(TIMER_ID_0);
  timer_destruct(TIMER_ID_1);
  timer_destruct(TIMER_ID_2);
}

/*******************************************************************************
* Tests
*******************************************************************************/
void test_init_initializes_common_registers(void)
{
  TEST_ASSERT(timsk == 0);
}

void test_construct_initializes_tccr(void)
{
  _make_timer(TIMER_ID_0, TIMER_PRESCALE_0, TIMER_ISR_TRIGGER_OVERFLOW);
  TEST_ASSERT(tccr_0 == 0);
}

void test_construct_initializes_tcnt(void)
{
  _make_timer(TIMER_ID_0, TIMER_PRESCALE_0, TIMER_ISR_TRIGGER_OVERFLOW);
  TEST_ASSERT(tcnt_0 == 0);
}

void test_construct_initializes_timsk(void)
{
  // timer 0 uses bits 0 thru 1
  // timer 1 uses bits 2 thru 5
  // timer 2 uses bits 6 thru 8
  _make_timer(TIMER_ID_0, TIMER_PRESCALE_0, TIMER_ISR_TRIGGER_OVERFLOW);
  TEST_ASSERT(timsk == 1);

  _make_timer(TIMER_ID_1, TIMER_PRESCALE_0, TIMER_ISR_TRIGGER_OVERFLOW);
  TEST_ASSERT(timsk == 5);

  _make_timer(TIMER_ID_2, TIMER_PRESCALE_0, TIMER_ISR_TRIGGER_OVERFLOW);
  TEST_ASSERT(timsk == 69);
}

void test_construct_fails_for_timer_already_in_use(void)
{
  _make_timer(TIMER_ID_0, TIMER_PRESCALE_0, TIMER_ISR_TRIGGER_OVERFLOW);
  TEST_ASSERT(
    _make_timer(TIMER_ID_0, TIMER_PRESCALE_0, TIMER_ISR_TRIGGER_OVERFLOW)
    == TIMER_ERR_TIMER_IN_USE
  );
}

void test_construct_succeeds_for_timer_not_already_in_use(void)
{
  TEST_ASSERT(
    _make_timer(TIMER_ID_1, TIMER_PRESCALE_0, TIMER_ISR_TRIGGER_OVERFLOW)
    == TIMER_ERR_NONE
  );
}

/*******************************************************************************
* Private Function Definitions
*******************************************************************************/
timer_err_t _make_timer(
  timer_id_t id,
  timer_prescale_t prescale,
  timer_isr_trigger_t isr_trigger
) {
  timer_attr_t config;

  // common registers
  config.tifr = &tifr;
  config.timsk = &timsk;

  config.timer_id = id;

  // specific registers
  switch(id) {
    case TIMER_ID_0:
      config.tccr = &tccr_0;
      config.tcnt = &tcnt_0;
    break;
    case TIMER_ID_1:
      config.tccr = &tccr_1;
      config.tcnt = &tcnt_1;
    break;
    case TIMER_ID_2:
      config.tccr = &tccr_2;
      config.tcnt = &tcnt_2;
    break;

  }

  config.prescale = prescale;
  config.isr_trigger = isr_trigger;

  return timer_construct(config);
}
