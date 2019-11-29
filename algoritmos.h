#ifndef ALGORITMOS_H
#define ALGORITMOS_H

typedef struct {
 int id;
 int computation_time;
 int period_time;
 int deadline; // this variable would be needed for the 3 algorithms
 int arrival_time;
 int laxity; //use this for laxity calculation laxity= deadline-(current_period + total_computed)
 int total_computed; //use this to evaluate how much of the task has been executed
 int status;
}task_information;

extern int NUMBER_OF_TASKS;
extern int ALGORITHMS_TO_EXECUTE;
extern int AMOUNT_OF_PERIODS;
extern int CRASH;
extern int current_period;
extern task_information *TASKS_TO_SCHEDULE;
extern task_information *WAIT_LIST;


void EDF();
void RM();
void LLF();
void deploy();
void create_tasks();
void EDF_schedulability_calculation();
void RM_schedulability_calculation();
double summation_EDF_RM();
double u_n_calculation();
void addTasktInWait(int id);
void Verify_Crash();
void resetValues();
void RM_Select_Task();
void EDF_Select_Task();
void undeploy(int position);
void LLF_Select_Task();


#endif
