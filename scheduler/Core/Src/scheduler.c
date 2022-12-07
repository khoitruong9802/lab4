/*
 * scheduler.c
 *
 *  Created on: Nov 11, 2022
 *      Author: DELL
 */

#include "scheduler.h"

sTasks SCH_tasks_G[SCH_MAX_TASKS];
uint8_t current_index_task = 0;
int sch_counter = 0;
//void SCH_Update(void){
//	for(int i = 0; i < current_index_task; i++){
//		if (SCH_tasks_G[i].Delay > 0){
//			SCH_tasks_G[i].Delay --;
//			if (SCH_tasks_G[i].Delay <= 0) {
//				SCH_tasks_G[i].Delay = SCH_tasks_G[i].Period;
//				SCH_tasks_G[i].RunMe += 1;
//			}
//		}
//	}
//}

//void SCH_Add_Task ( void (*pFunction)() , uint32_t DELAY, uint32_t PERIOD){
//	if (current_index_task < SCH_MAX_TASKS) {
//
//		SCH_tasks_G[current_index_task].pTask = pFunction;
//		SCH_tasks_G[current_index_task].Delay = DELAY;
//		SCH_tasks_G[current_index_task].Period =  PERIOD;
//		SCH_tasks_G[current_index_task].RunMe = 0;
//
//		SCH_tasks_G[current_index_task].TaskID = current_index_task;
//
//
//		current_index_task++;
//	}
//}
//

//
//void SCH_Dispatch_Tasks(void){
//	for(int i = 0; i < current_index_task; i++){
//		if(SCH_tasks_G[i].RunMe > 0){
//			SCH_tasks_G[i].RunMe--;
//			(*SCH_tasks_G[i].pTask)();
//		}
//	}
//}
//
//void SCH_Delete_Task(uint32_t taskID) {
////	SCH_tasks_G[taskID].pTask = 0;
////	SCH_tasks_G[taskID].Delay = 0;
////	SCH_tasks_G[taskID].Period =  0;
////	SCH_tasks_G[taskID].RunMe = 0;
//	for (int i = taskID; i < current_index_task - 1; i++) {
//		SCH_tasks_G[i] = SCH_tasks_G[i + 1];
//	}
//	current_index_task--;
//}

void SCH_Init(void) {
    unsigned char i;
    for (i = 0; i < SCH_MAX_TASKS; i++) {
        SCH_Delete_Task(i);
    }
}

void Add_Task_Suitable_Position(void (*pFunction)() , uint32_t DELAY, uint32_t PERIOD) {
	for (int i = 1; i < current_index_task; i++) {
		SCH_tasks_G[i].Delay -= sch_counter;
	}
	sch_counter = 0;
	int i = current_index_task - 1;
	for (; i >= 0; i--) {
		if (DELAY > SCH_tasks_G[i].Delay) {
			break;
		} else {
			SCH_tasks_G[i + 1] = SCH_tasks_G[i];
		}
	}
	i++;
	SCH_tasks_G[i].pTask = pFunction;
	SCH_tasks_G[i].Delay = DELAY;
	SCH_tasks_G[i].Period =  PERIOD;
	SCH_tasks_G[i].RunMe = 0;
	SCH_tasks_G[i].TaskID = current_index_task;
}

void Add_Task_Suitable_Position_For_Update(void (*pFunction)() , uint32_t DELAY, uint32_t PERIOD) {
	for (int i = 1; i < current_index_task; i++) {
		SCH_tasks_G[i].Delay -= sch_counter;
	}
	sch_counter = 0;
	int i = 1;
	sTasks tmp = SCH_tasks_G[0];
	for (; i < current_index_task; i++) {
		if (tmp.Delay > SCH_tasks_G[i].Delay) {
			SCH_tasks_G[i - 1] = SCH_tasks_G[i];
		} else {
			break;
		}
	}
	SCH_tasks_G[--i] = tmp;
}

void SCH_Add_Task (void (*pFunction)() , uint32_t DELAY, uint32_t PERIOD){
	if (current_index_task == 0) {
		SCH_tasks_G[current_index_task].pTask = pFunction;
		SCH_tasks_G[current_index_task].Delay = DELAY;
		SCH_tasks_G[current_index_task].Period =  PERIOD;
		SCH_tasks_G[current_index_task].RunMe = 0;

		SCH_tasks_G[current_index_task].TaskID = current_index_task;

		current_index_task++;
	} else if (current_index_task < SCH_MAX_TASKS) {
		Add_Task_Suitable_Position(pFunction, DELAY, PERIOD);
		current_index_task++;
	}

}

void SCH_Update(void){
	if (SCH_tasks_G[0].Delay > 0) {
		SCH_tasks_G[0].Delay--;
		sch_counter++; // NOTE NOTE
	}
	if (SCH_tasks_G[0].Delay <= 0) {
		SCH_tasks_G[0].Delay = SCH_tasks_G[0].Period;
		SCH_tasks_G[0].RunMe += 1;
		Add_Task_Suitable_Position_For_Update(SCH_tasks_G[0].pTask, SCH_tasks_G[0].Delay, SCH_tasks_G[0].Period);
	}
}

void SCH_Dispatch_Tasks(void){
	for(int i = 0; i < current_index_task; i++){
		if(SCH_tasks_G[i].RunMe > 0){
			SCH_tasks_G[i].RunMe--;
			(*SCH_tasks_G[i].pTask)();
			if (SCH_tasks_G[i].Period == 0) {
				SCH_Delete_Task(i);
			}
		}
	}
}

void SCH_Delete_Task(uint32_t taskID) {
	SCH_tasks_G[taskID].pTask = 0;
	SCH_tasks_G[taskID].Delay = 0;
	SCH_tasks_G[taskID].Period =  0;
	SCH_tasks_G[taskID].RunMe = 0;
	SCH_tasks_G[taskID].pTask = 0;
	for (int i = taskID; i < current_index_task - 1; i++) {
		SCH_tasks_G[i] = SCH_tasks_G[i + 1];
	}
	current_index_task--;
}
