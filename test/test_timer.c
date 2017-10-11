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
uint8_t tccr0a;
uint8_t tccr0b;
uint8_t tcnt_0;
uint8_t ocr0a;
uint8_t ocr0b;

uint8_t tccr1a;
uint8_t tccr1b;
uint8_t tcnt_1;
uint16_t ocr1a;
uint16_t ocr1b;

uint8_t tccr2a;
uint8_t tccr2b;
uint8_t tcnt_2;
uint8_t ocr2a;
uint8_t ocr2b;

// fake common timer registers
uint8_t tifr;
static uint8_t timsk;

/*******************************************************************************
* Private Function Declarations
*******************************************************************************/
timer_err_t _make_timer(
  timer_id_t id,
  timer_prescale_t prescale,
  timer_isr_trigger_t isr_trigger,
  timer_mode_t timer_mode,
  void *callback,
  uint8_t ocra_8,
  uint8_t ocrb_8,
  uint16_t ocra_16,
  uint16_t ocrb_16
);

void _make_timers(
  timer_prescale_t prescale,
  timer_isr_trigger_t isr_trigger,
  timer_mode_t timer_mode,
  void *callback,
  uint8_t ocr0a,
  uint8_t ocr0b,
  uint16_t ocr1a,
  uint16_t ocr1b,
  uint8_t ocr2a,
  uint8_t ocr2b
);

void _test_tccr_registers(
  uint8_t expected_tccr0a,
  uint8_t expected_tccr0b,
  uint8_t expected_tccr1a,
  uint8_t expected_tccr1b,
  uint8_t expected_tccr2a,
  uint8_t expected_tccr2b
);

void _test_ocr_registers(
  uint8_t expected_ocr0a,
  uint8_t expected_ocr0b,
  uint16_t expected_ocr1a,
  uint16_t expected_ocr1b,
  uint8_t expected_ocr2a,
  uint8_t expected_ocr2b
);

void _cb();

/*******************************************************************************
* Setup and Teardown
*******************************************************************************/
void setUp(void)
{
  srand(time(NULL));

  tccr0a = (uint8_t)rand();
  tccr0b = (uint8_t)rand();
  tcnt_0 = (uint8_t)rand();
  tccr1a = (uint8_t)rand();
  tccr1b = (uint8_t)rand();
  tcnt_1 = (uint8_t)rand();
  tccr2a = (uint8_t)rand();
  tccr2b = (uint8_t)rand();
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

void test_construct_initializes_normal_mode(void)
{
  _make_timers(
    TIMER_PRESCALE_0,
    TIMER_ISR_TRIGGER_OVERFLOW,
    TIMER_MODE_NORMAL,
    _cb,
    0,0,0,0,0,0
  );

  _test_tccr_registers(0,0,0,0,0,0);
}

void test_construct_initializes_ctc_mode(void)
{
  _make_timers(
    TIMER_PRESCALE_0,
    TIMER_ISR_TRIGGER_OVERFLOW,
    TIMER_MODE_CTC,
    _cb,
    0,0,0,0,0,0
  );

  _test_tccr_registers(2,0,2,0,2,0);
}

void test_construct_initializes_tcnt(void)
{
  _make_timer(
    TIMER_ID_0,
    TIMER_PRESCALE_0,
    TIMER_ISR_TRIGGER_OVERFLOW,
    TIMER_MODE_NORMAL,
    _cb,
    0,0,0,0
  );
  TEST_ASSERT(tcnt_0 == 0);
}

void test_construct_initializes_timsk(void)
{
  // timer 0 uses bits 0 thru 1
  // timer 1 uses bits 2 thru 5
  // timer 2 uses bits 6 thru 8
  _make_timer(
    TIMER_ID_0,
    TIMER_PRESCALE_0,
    TIMER_ISR_TRIGGER_OVERFLOW,
    TIMER_MODE_NORMAL,
    _cb,
    0,0,0,0
  );
  TEST_ASSERT(timsk == 1);

  _make_timer(
    TIMER_ID_1,
    TIMER_PRESCALE_0,
    TIMER_ISR_TRIGGER_OVERFLOW,
    TIMER_MODE_NORMAL,
    _cb,
    0,0,0,0
  );
  TEST_ASSERT(timsk == 5);

  _make_timer(
    TIMER_ID_2,
    TIMER_PRESCALE_0,
    TIMER_ISR_TRIGGER_OVERFLOW,
    TIMER_MODE_NORMAL,
    _cb,
    0,0,0,0
  );
  TEST_ASSERT(timsk == 69);
}

void test_construct_fails_for_timer_already_in_use(void)
{
  _make_timer(
    TIMER_ID_0,
    TIMER_PRESCALE_0,
    TIMER_ISR_TRIGGER_OVERFLOW,
    TIMER_MODE_NORMAL,
    _cb,
    0,0,0,0
  );
  TEST_ASSERT(
    _make_timer(
      TIMER_ID_0,
      TIMER_PRESCALE_0,
      TIMER_ISR_TRIGGER_OVERFLOW,
      TIMER_MODE_NORMAL,
      _cb,
      0,0,0,0
    )
    == TIMER_ERR_TIMER_IN_USE
  );
}

void test_construct_succeeds_for_timer_not_already_in_use(void)
{
  TEST_ASSERT(
    _make_timer(
      TIMER_ID_1,
      TIMER_PRESCALE_0,
      TIMER_ISR_TRIGGER_OVERFLOW,
      TIMER_MODE_NORMAL,
      _cb,
      0,0,0,0
    )
    == TIMER_ERR_NONE
  );
}

void test_construct_initializes_ocr_registers(void)
{
  uint8_t _ocr0a = (uint8_t) rand();
  uint8_t _ocr0b = (uint8_t) rand();
  uint8_t _ocr1a = (uint8_t) rand();
  uint8_t _ocr1b = (uint8_t) rand();
  uint8_t _ocr2a = (uint8_t) rand();
  uint8_t _ocr2b = (uint8_t) rand();

  _make_timers(
    TIMER_PRESCALE_0,
    TIMER_ISR_TRIGGER_OVERFLOW,
    TIMER_MODE_NORMAL,
    _cb,
    _ocr0a, _ocr0b, _ocr1a, _ocr1b, _ocr2a, _ocr2b
  );

  _test_ocr_registers(_ocr0a, _ocr0b, _ocr1a, _ocr1b, _ocr2a, _ocr2b);
}


/*******************************************************************************
* Private Function Definitions
*******************************************************************************/
timer_err_t _make_timer(
  timer_id_t id,
  timer_prescale_t prescale,
  timer_isr_trigger_t isr_trigger,
  timer_mode_t timer_mode,
  void *callback,
  uint8_t ocra_8,
  uint8_t ocrb_8,
  uint16_t ocra_16,
  uint16_t ocrb_16
) {
  timer_attr_t config;

  // common registers
  config.tifr = &tifr;
  config.timsk = &timsk;

  config.timer_id = id;

  // specific registers
  switch(id) {
    case TIMER_ID_0:
      config.tccra = &tccr0a;
      config.tccrb = &tccr0b;
      config.tcnt = &tcnt_0;
      config.ocra_8 = &ocr0a;
      config.ocrb_8 = &ocr0b;
    break;
    case TIMER_ID_1:
      config.tccra = &tccr1a;
      config.tccrb = &tccr1b;
      config.tcnt = &tcnt_1;
      config.ocra_16 = &ocr1a;
      config.ocrb_16 = &ocr1b;
    break;
    case TIMER_ID_2:
      config.tccra = &tccr2a;
      config.tccrb = &tccr2b;
      config.tcnt = &tcnt_2;
      config.ocra_8 = &ocr2a;
      config.ocrb_8 = &ocr2b;
    break;
  }

  config.prescale = prescale;
  config.isr_trigger = isr_trigger;
  config.callback = callback;
  config.timer_mode = timer_mode;
  config.ocr_val_a_8 = ocra_8;
  config.ocr_val_b_8 = ocrb_8;
  config.ocr_val_a_16 = ocra_16;
  config.ocr_val_b_16 = ocrb_16;

  return timer_construct(config);
}

void _make_timers(
  timer_prescale_t prescale,
  timer_isr_trigger_t isr_trigger,
  timer_mode_t timer_mode,
  void *callback,
  uint8_t ocr0a,
  uint8_t ocr0b,
  uint16_t ocr1a,
  uint16_t ocr1b,
  uint8_t ocr2a,
  uint8_t ocr2b
) {
  _make_timer(
    TIMER_ID_0,
    prescale,
    isr_trigger,
    timer_mode,
    callback,
    ocr0a,
    ocr0b,
    0,0
  );

  _make_timer(
    TIMER_ID_1,
    prescale,
    isr_trigger,
    timer_mode,
    callback,
    0,0,
    ocr1a,
    ocr1b
  );

  _make_timer(
    TIMER_ID_2,
    prescale,
    isr_trigger,
    timer_mode,
    callback,
    ocr2a,
    ocr2b,
    0,0
  );
}

void _test_tccr_registers(
  uint8_t expected_tccr0a,
  uint8_t expected_tccr0b,
  uint8_t expected_tccr1a,
  uint8_t expected_tccr1b,
  uint8_t expected_tccr2a,
  uint8_t expected_tccr2b
) {
  TEST_ASSERT(tccr0a == expected_tccr0a);
  TEST_ASSERT(tccr0b == expected_tccr0b);
  TEST_ASSERT(tccr1a == expected_tccr1a);
  TEST_ASSERT(tccr1b == expected_tccr1b);
  TEST_ASSERT(tccr2a == expected_tccr2a);
  TEST_ASSERT(tccr2b == expected_tccr2b);
}

void _test_ocr_registers(
  uint8_t expected_ocr0a,
  uint8_t expected_ocr0b,
  uint16_t expected_ocr1a,
  uint16_t expected_ocr1b,
  uint8_t expected_ocr2a,
  uint8_t expected_ocr2b
) {
  TEST_ASSERT(ocr0a == expected_ocr0a);
  TEST_ASSERT(ocr0b == expected_ocr0b);
  TEST_ASSERT(ocr1a == expected_ocr1a);
  TEST_ASSERT(ocr1b == expected_ocr1b);
  TEST_ASSERT(ocr2a == expected_ocr2a);
  TEST_ASSERT(ocr2b == expected_ocr2b);
}

void _cb() {}
