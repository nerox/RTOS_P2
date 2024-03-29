#ifndef ALGORITMOS_H
#define ALGORITMOS_H

typedef struct {
 uint id;
 uint computation_time;
 uint period_time;
 uint deadline; // this variable would be needed for the 3 algorithms
 int arrival_time;
 int laxity; //use this for laxity calculation laxity= deadline-(current_period + total_computed)
 int total_computed; //use this to evaluate how much of the task has been executed
 int status;
}task_information;

#define MAX_CMM_LEN 6000

extern uint NUMBER_OF_TASKS;
extern uint ALGORITHMS_TO_EXECUTE;
extern uint AMOUNT_OF_PERIODS;
extern int CRASH;
extern int current_period;
extern task_information *TASKS_TO_SCHEDULE;
extern task_information *WAIT_LIST;
extern int * scheduled_Matrix;

void EDF();
void RM();
void LLF();
void deploy();
void create_tasks();
void EDF_schedulability_calculation();
void RM_schedulability_calculation();
void LLF_schedulability_calculation();
double summation_EDF_RM();
double u_n_calculation();
void addTasktInWait(int id);
void Verify_Crash();
void resetValues();
void RM_Select_Task();
void EDF_Select_Task();
void undeploy(int position);
void LLF_Select_Task();
void latex_rm();
void latex_edf();
void latex_llf();
void latex_rm_edf();
void latex_rm_lff();
void latex_edf_lff();
void latex_rm_edf_lff();
void latex_table_slide_header(char algorithm []);
void end_latex_slide();
void latex_table_slide(char algorithm[]);
void cleanOutPutMatrix();
void endlatex();
void latex_rm_evaluation(int result, float mu,float u);
void latex_EDF_evaluation(int result, float mu);
void latex_LLF_evaluation(int result, float mu);
#endif
