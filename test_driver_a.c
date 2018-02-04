#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "test_driver_a.h"

#if (defined TEST_DRIVER_INIT && TEST_DRIVER_VERSION <= 1)

void testDriver_init()
{
    int a=0;
    int b=0;
    int c;
    c=a+b;
}

#endif