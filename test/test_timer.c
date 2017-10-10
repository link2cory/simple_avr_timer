#include "unity.h"

/*******************************************************************************
* MODULE UNDER TEST
*******************************************************************************/
#include "timer.h"

/*******************************************************************************
* Local Data
*******************************************************************************/
timer_d_t timer_handle;

/*******************************************************************************
* Setup and Teardown
*******************************************************************************/
void setUp(void)
{
  timer_attr_t config;
  timer_construct(config, &timer_handle);
}

void tearDown(void)
{
}

void test_timer_NeedToImplement(void)
{
    TEST_IGNORE_MESSAGE("Need to Implement timer");
}
