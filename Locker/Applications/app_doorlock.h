#ifndef INC_APP_DOORLOCK_H_
#define INC_APP_DOORLOCK_H_

#include <stdint.h>
#include <stdbool.h>

void App_Init(void);
void App_Process(void);
void App_SignalLockTrigger(void); // Dipanggil oleh EXTI ISR

#endif /* INC_APP_DOORLOCK_H_ */
