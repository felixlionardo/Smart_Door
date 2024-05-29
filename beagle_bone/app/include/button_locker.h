#ifndef BUTTON_LOCKER_H
#define BUTTON_LOCKER_H

/* Module to initialize and cleanup button locker thread. 
This allows the manual button to turn the led on and off, and lock/unlock using the servo motor.

*/

void createButtonLockerThread();
void shutdownButtonLocker();
void joinButtonLockerThread();

#endif
