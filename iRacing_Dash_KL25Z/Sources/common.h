/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "USB1.h"
#include "CDC1.h"
#include "Tx1.h"
#include "Rx1.h"
#include "USB0.h"
#include "MCUC1.h"
#include "CLS1.h"
#include "TMOUT1.h"
#include "WAIT1.h"
#include "CS1.h"
#include "CS2.h"
#include "UTIL1.h"
#include "XF1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include <stdio.h>
/* Include the derivative-specific header file */
#include <MKL25Z4.h>

#define ledRed() GPIOB_PDOR &= ~0x40000
#define ledGreen() GPIOB_PDOR &= ~0x80000
#define ledBlue() GPIOD_PDOR &= ~0x80002
#define BLINK_LEN 20
#define CONN_BLINK_LEN 100
