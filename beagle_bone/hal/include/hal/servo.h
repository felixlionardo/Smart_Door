#ifndef _SERVO_H_
#define _SERVO_H_

/* Module to initialize and use servo motor.

*/

#include <stdbool.h>

void initializeServo();
void unlockServo();
void lockServo();
bool isLocked();
void setIsLocked(bool a);
#endif