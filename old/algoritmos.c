
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include "interfaz.h"
#include "algoritmos.h"


uint NUMBER_OF_TASKS = 0;
uint ALGORITHMS_TO_EXECUTE;
uint AMOUNT_OF_PERIODS;
int CRASH;
int current_period;
task_information *TASKS_TO_SCHEDULE;
task_information *WAIT_LIST;

void EDF(){
	resetValues();
	EDF_schedulability_calculation();
	CRASH=0;
	current_period=0;
	deploy();
	while(current_period<AMOUNT_OF_PERIODS){
		current_period++;
		Verify_Crash();
		if  (CRASH==1){
			break;
		}
		EDF_Select_Task();
		deploy();
	}
	if (CRASH==0){
		printf("Praise the lord!!!!!!, the simulation results have shown that no one has died YET, be sure to make enough simulations to ensure the safety of pregnant elders\n");
	}
}

void RM(){
	resetValues();
	RM_schedulability_calculation();
	CRASH=0;
	current_period=0;
	deploy();
	while(current_period<AMOUNT_OF_PERIODS){
		current_period++;
		Verify_Crash();
		if  (CRASH==1){
			break;
		}
		RM_Select_Task();
		deploy();
	}
	if (CRASH==0){
		printf("Praise the lord!!!!!!, the simulation results have shown that no one has died YET, be sure to make enough simulations to ensure the safety of pregnant elders\n");
	}
}
void EDF_Select_Task(){
	int position;
	double currentLowest=10000000000000000;
	int i;
	int found_candidate=0;
	for(i=0;i<NUMBER_OF_TASKS*2;i++){
		if(WAIT_LIST[i].status==1 && currentLowest>(WAIT_LIST[i].deadline-current_period)){
			position=i;
			currentLowest=WAIT_LIST[i].deadline-current_period;
			found_candidate=1;
		}
	}
	if(found_candidate==1){
		WAIT_LIST[position].total_computed++;
		printf("Currently executing task with id %d at time %d\n",WAIT_LIST[position].id,current_period);
		undeploy(position);
	}
}
void RM_Select_Task(){
	int position;
	double currentLowest=10000000000000000;
	int i;
	int found_candidate=0;
	for(i=0;i<NUMBER_OF_TASKS*2;i++){
		if(WAIT_LIST[i].status==1 && currentLowest>WAIT_LIST[i].period_time){
			position=i;
			currentLowest=WAIT_LIST[i].period_time;
			found_candidate=1;
		}
	}
	if(found_candidate==1){
		WAIT_LIST[position].total_computed++;
		printf("Currently executing task with id %d at time %d\n",WAIT_LIST[position].id,current_period);
		undeploy(position);
	}

}
void Verify_Crash(){
	int i;
	for(i=0;i<NUMBER_OF_TASKS*2;i++){
		if(WAIT_LIST[i].status==1 && current_period>WAIT_LIST[i].deadline){
			CRASH=1;
			printf("Sadly people have died, pay respect to those who lost their loved ones, we warned you about this outcome %d\n",WAIT_LIST[i].deadline);
		}
	}
}

void LLF(){
	resetValues();
	CRASH=0;
	current_period=0;
	deploy();
	while(current_period<AMOUNT_OF_PERIODS){
		current_period++;
		Verify_Crash();
		if  (CRASH==1){
			break;
		}
		LLF_Select_Task();
		deploy();
	}
	if (CRASH==0){
		printf("Praise the lord!!!!!!, the simulation results have shown that no one has died YET, be sure to make enough simulations to ensure the safety of pregnant elders\n");
	}
}

void LLF_Select_Task(){
	int position;
	double currentLowestLaxity=10000000000000000;
	int i;
	int found_candidate=0;
	int laxity;
	for(i=0;i<NUMBER_OF_TASKS*2;i++){
		laxity = WAIT_LIST[i].deadline - current_period + WAIT_LIST[i].computation_time;
		if(WAIT_LIST[i].status==1 && currentLowestLaxity>laxity){
			position=i;
			currentLowestLaxity=laxity;
			found_candidate=1;
		}
	}
	if(found_candidate==1){
		WAIT_LIST[position].computation_time--;
		printf("Currently executing task with id %d at time %d\n",WAIT_LIST[position].id,current_period);
		undeploy(position);
	}

}
void deploy(){
	int i;
	for(i=0;i<NUMBER_OF_TASKS;i++){
		if(current_period%TASKS_TO_SCHEDULE[i].period_time==0){
			printf("A new task entered at time %d\n",current_period);
			addTasktInWait(i);
		}
		else{
			//nothing to do, no new events added...codigo defensivo
		}
	}
}
void undeploy(int position){
	if(WAIT_LIST[position].total_computed==WAIT_LIST[position].computation_time){
		WAIT_LIST[position].status=0;
		printf("Currently task with id %d has completed it's task at time %d\n",WAIT_LIST[position].id,current_period);
	}

}
void addTasktInWait(int id){
	int i;
	int deployed=0;
	for(i=0;i<NUMBER_OF_TASKS*2;i++){
		if(WAIT_LIST[i].status==0){
			WAIT_LIST[i].id=TASKS_TO_SCHEDULE[id].id;
			WAIT_LIST[i].computation_time=TASKS_TO_SCHEDULE[id].computation_time;
			WAIT_LIST[i].deadline=TASKS_TO_SCHEDULE[id].period_time+current_period;
			WAIT_LIST[i].arrival_time=current_period;
			WAIT_LIST[i].total_computed=0;
			WAIT_LIST[i].status=1;
			deployed=1;
			break;
		}
	}
	if(deployed==0){
		CRASH=1;
		printf("Sadly people have died, pay respect to those who lost their loved ones, we warned you about this outcome\n");
	}
}


void create_tasks(){
	AMOUNT_OF_PERIODS=24;

	WAIT_LIST =  (task_information *)malloc(NUMBER_OF_TASKS*2 * sizeof(task_information));

  for (uint i = 0; i < NUMBER_OF_TASKS; i++) {
    printf("Task ID: %d Ci = %d Di = %d\n", TASKS_TO_SCHEDULE[i].id, TASKS_TO_SCHEDULE[i].computation_time,TASKS_TO_SCHEDULE[i].period_time);
  }

	switch (ALGORITHMS_TO_EXECUTE)
	{
	case 1:
		printf("User selected to execute RM\n");
		printf("Performing the RM simulation \n");
		RM();
		break;
	case 2:
		printf("User selected to execute EDF\n");
		printf("Performing the EDF simulation \n");
		EDF();
		break;
	case 3:
		printf("User selected to execute LLF\n");
		printf("Performing the LLF simulation \n");
		LLF();
		break;
	case 4:
		printf("User selected to execute RM & EDF\n");
		printf("Performing the RM simulation \n");
		RM();
		printf("Performing the LLF simulation \n");
		EDF();
		break;
	case 5:
		printf("User selected to execute RM & LLF\n");
		printf("Performing the RM simulation \n");
		RM();
		printf("Performing the LLF simulation \n");
		LLF();
		break;
	case 6:
		printf("User selected to execute EDF & LLF\n");
		printf("Performing the EDF simulation \n");
		EDF();
		printf("Performing the LLF simulation \n");
		LLF();
		break;
	case 7:
		printf("User selected to execute RM & EDF & LLF\n");
		printf("Performing the RM simulation \n");
		RM();
		printf("Performing the EDF simulation \n");
		EDF();
		printf("Performing the LLF simulation \n");
		LLF();
		break;
	default: // code to be executed if n doesn't match any cases
		printf("Not a valid option selected\n");
		break;
	}
}

void EDF_schedulability_calculation(){
	double u_value=summation_EDF_RM();
	if (u_value<=1){
		printf("Dear user, the selected values are canditates for EDF scheduling, you can take a deep breath no one will die today, the following simulation shows the schedulability testing\n");
	}
	else{
		printf("Dear user, the selected values are not canditates for EDF scheduling. Be careful thousands can die today!!!!. Perhaps you should reconsider other values, the following simulation shows the schedulability testing\n");
	}
}

void RM_schedulability_calculation(){
	double u_value=summation_EDF_RM();
	double u_n_value=u_n_calculation();
	if (u_value>1){
		printf("Dear user, the selected values are not canditates for RM scheduling. Perhaps you should reconsider other values; the future of endangered species might be on you hands. The following simulation shows the schedulability testing\n");
	}
	else if(u_value<1 && u_value>=u_n_value){
		printf("Dear user, the selected values MIGHT be canditates for RM scheduling. Perhaps you should reconsider other values for better certainty; people could potentially die. The following simulation shows the schedulability testing\n");
	}
	else{
		printf("Dear user, the selected values are canditates for RM scheduling, be grateful 0 casualties will be driven by this selection, the following simulation shows the schedulability testing\n");
	}
}
double u_n_calculation(){
	double u_n_result=NUMBER_OF_TASKS*(pow(2,1/((double)NUMBER_OF_TASKS))-1 );
	printf("The calculated u(n) is %f\n",u_n_result);
	return 	u_n_result;
}
double summation_EDF_RM(){
	int i;
	double u_result=0;
	printf("Performing the u calculation\n");
	for (i=0;i<NUMBER_OF_TASKS;i++){
		u_result+=((double)TASKS_TO_SCHEDULE[i].computation_time)/((double)TASKS_TO_SCHEDULE[i].period_time);
	}
	printf("The calculated u is %f\n",u_result);
	return u_result;
}
void resetValues(){
	int i;
	for (i=0;i<NUMBER_OF_TASKS*2 ;i++){
		WAIT_LIST[i].status=0;
	}

}
