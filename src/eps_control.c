// src/eps_control.c
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "eps_control.h"
#include "state_manager.h"
#include "watchdog.h"
#include <stdio.h>

#define CRITICAL_BUS_VOLTAGE 2.5f

void vEPSMonitoringTask(void *pvParameters) {
    
    float current_bus_voltage = 3.2f;

    printf("EPS Monitoring Task initialized and running.\n");
    for(;;) {
        
        // 1. Simulate Reading Voltage
        // NOTE: We'll intentionally simulate a fault after 20 seconds.
        if(xTaskGetTickCount() > pdMS_TO_TICKS(20000)) {
            current_bus_voltage = 2.4f; // Injecting a simulated fault
        }
        // 2. FDIR (Fault Detection and Isolation) Logic

        if(current_bus_voltage < CRITICAL_BUS_VOLTAGE && get_system_mode() != MODE_CRITICAL){
            printf("EPS MON: !!! CRITICAL FAULT DETECTED (V: %.2f V) !!!\n", current_bus_voltage);
            
            // 3. Recovery Action (Highest Authority)
            // Force FSW into the safest state using the protected function
            set_system_mode(MODE_CRITICAL);
            
            printf("EPS MON: FDIR complete. System forced into MODE_CRITICAL.\n");   
        }
        else {
            // Nominal operation
            // printf("EPS MON: Voltage nominal (%.2f V).\n", current_bus_voltage);
        }
        
        watchdog_pet(WDT_TASK_EPS_MON);
        
        vTaskDelay(pdMS_TO_TICKS(10000)); // Check every 10 seconds
    }
}


void vEPS_SetSafeModePower(int mode_id) {
    printf("STUB CALLED: EPS received power command (Mode ID: %d).\n", mode_id);
}