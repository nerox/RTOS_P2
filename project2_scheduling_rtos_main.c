#include "Tasks.h"



int main (int argc, char **argv)
{
	create_tasks();
	return 0;
}

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
	//EDF_schedulability_calculation(); Vernal agregue un metodo que diga si el algoritmo es calendarizable o no
	CRASH=0;
	current_period=0;
	deploy();
	while(current_period<AMOUNT_OF_PERIODS){
		current_period++;
		Verify_Crash();
		if  (CRASH==1){
			break;
		}
		LLF_Select_Task(); //Modifique el codigo dentro de este metodo
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
		if(WAIT_LIST[i].status==1 && currentLowestLaxity>laxity){ // Vernal reemplazar WAIT_LIST[i].period_time por calculo de laxity
			position=i;
			currentLowestLaxity=laxity;// Vernal sustituir por calculo de laxity
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
			//nothing to do, no new events added
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
			WAIT_LIST[i].period_time=TASKS_TO_SCHEDULE[id].period_time;
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

//THE FOLLOWING SHOULD BE REPLACED BY GTK
void create_tasks(){
	NUMBER_OF_TASKS=3;
	ALGORITHMS_TO_EXECUTE=7; // THIS WOULD HOLD WHICH ALGORITHMS THE USER WANTS TO EXECUTE 1 RM, 2 EDF, 3 LLF, 4 RM & EDF, 5 RM LLF, 6 EDF & LLF, 7 RM & EDF & LLF
	AMOUNT_OF_PERIODS=24;

	/*
		The following should be replaced by GTK, this simulates the behavior of setting up the tasks and their settings
	*/
	TASKS_TO_SCHEDULE =  (task_information *)malloc(NUMBER_OF_TASKS * sizeof(task_information)); //this array will held all of the information about the tasks being executed
	WAIT_LIST =  (task_information *)malloc(NUMBER_OF_TASKS*2 * sizeof(task_information)); //this array will be the wait list
	//Set the information of task 1
	TASKS_TO_SCHEDULE[0].id=0;
	TASKS_TO_SCHEDULE[0].computation_time=2;
	TASKS_TO_SCHEDULE[0].period_time=6;

	//Set the information of task 2
	TASKS_TO_SCHEDULE[1].id=1;
	TASKS_TO_SCHEDULE[1].computation_time=2;
	TASKS_TO_SCHEDULE[1].period_time=8;

	//Set the information of task 3
	TASKS_TO_SCHEDULE[2].id=2;
	TASKS_TO_SCHEDULE[2].computation_time=3;
	TASKS_TO_SCHEDULE[2].period_time=10;

	//Set the information of task 4
/*	TASKS_TO_SCHEDULE[3].id=3;
	TASKS_TO_SCHEDULE[3].computation_time=1;
	TASKS_TO_SCHEDULE[3].period_time=6;
	//Set the information of task 5
	TASKS_TO_SCHEDULE[4].id=4;
	TASKS_TO_SCHEDULE[4].computation_time=1;
	TASKS_TO_SCHEDULE[4].period_time=6;*/

	switch (ALGORITHMS_TO_EXECUTE)
	{
	case 1:
		printf("User selected to execute RM\n");
		printf("Performing the RM simulation \n");
		latex_rm();
		RM();
		break;
	case 2:
		printf("User selected to execute EDF\n");
		printf("Performing the EDF simulation \n");
		latex_edf();
		EDF();
		break;
	case 3:
		printf("User selected to execute LLF\n");
		printf("Performing the LLF simulation \n");
		latex_llf();
		LLF();
		break;
	case 4:
		printf("User selected to execute RM & EDF\n");
		printf("Performing the RM simulation \n");
		latex_rm_edf();
		RM();
		printf("Performing the LLF simulation \n");
		EDF();
		break;
	case 5:
		printf("User selected to execute RM & LLF\n");
		printf("Performing the RM simulation \n");
		latex_rm_lff();
		RM();
		printf("Performing the LLF simulation \n");
		LLF();
		break;
	case 6:
		printf("User selected to execute EDF & LLF\n");
		printf("Performing the EDF simulation \n");
		latex_edf_lff();
		EDF();
		printf("Performing the LLF simulation \n");
		LLF();
		break;
	case 7:
		printf("User selected to execute RM & EDF & LLF\n");
		printf("Performing the RM simulation \n");
		latex_rm_edf_lff();
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

void latex_rm(){
	char cmd[MAX_CMM_LEN] = "echo \\\\\\\\\\documentclass{beamer}\\\\\\\\\\mode'<'presentation'>' {\\\\\\\\\\usetheme{Madrid}}\\\\\\\\\\usepackage{graphicx}\\\\\\\\\\usepackage{booktabs}\\\\\\\\\\title[RM, EDF, LLF]{Proyecto\ 2:\ Calendarizacion\ en\ Tiempo\ Real}\\\\\\\\\\author{Vargas A,\ Camacho A,\ Morales V}\\\\\\\\\\institute[TEC]{Tecnologico\ de\ Costa Rica \\\\\\\\\\medskip\\\\\\\\\\textit{avargas@gmail.com,\ acamacho@gmail.com,\ verny.morales@gmail.com}\\\\\\\\\\textit{3er Cuatrimestre}}\\\\\\\\\\date{\\\\\\\\\\today}\\\\\\\\\\begin{document}\\\\\\\\\\begin{frame}\\\\\\\\\\titlepage\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Rate\ Monotonic}\\\\\\\\\\begin{block}{Tipo}Algoritmo\ de\ Scheduling \Dinamico,\ utilizado\ para\ la\ reslucion\ de\ problemas\ caoticos,\ como\ por\ ejemplo\ el\ problema\ de\ los\ carros\ autonomos.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Manejo\ de\ prioridades}Algoritmo\ de\ prioridades\ estaticas,\ esto\ quiere\ decir\ que\ ninguna\ tarea\ puede\ cambiar\ su\ prioridad.\ Donde\ la\ prioridad\ de\ una\ tarea\ siempre\ es\ igual\ a\ su\ periodo.\ Periodo\ mas\ corto,\ mayor\ la\ prioridad.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Supuestos}Todas\ las\ tareas\ criticas\ son\ periodicas,\ e\ independientes.\ El\ tiempo\ de\ computacion\ se\ conoce\ a\ priori,\ y\ el\ cambio\ de\ contexto\ es\ igual\ a\ cero,\ o\ ya\ esta\ considerado\ en\ el\ tiempo\ de\ computacion.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas\ de\ Scheduling}\\\\\\\\\\begin{theorem}[Parametros\ a\ tomar\ en\ cuenta]$\\\\\\\\\\mu = \\\\\\\\\\Sigma \\\\\\\\\\frac{Ci}{Pi}$ Utilizacion\ del\ CPU\$\ 'Un' = n \'2''^'\\\\\\\\\\frac{'1'}{n} - '1' $ donde\ n\ es\ la\ cantidad\ de\ tareas\\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Condiciones\ de\ suficiencia]$\\\\\\\\\\mu \\\\\\\\\\leq Un $ Tareas\ calendarizables\ $\\\\\\\\\\mu \\\\\\\\\\geq Un$ Debido\ a\ que\ es\ una\ condicion\ de\ sufuciencia\ podria\ ser\ calendarizable\$\\\\\\\\\\mu \\\\\\\\\\succ '1'$ Tareas\ no\ calendarizables\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame}\\\\\\\\\\end{document} >> doc.tex";
	printf("%s\n",cmd);

	system(cmd);
}
void latex_edf(){
	char cmd[MAX_CMM_LEN] = "echo \\\\\\\\\\documentclass{beamer}\\\\\\\\\\mode'<'presentation'>' {\\\\\\\\\\usetheme{Madrid}}\\\\\\\\\\usepackage{graphicx}\\\\\\\\\\usepackage{booktabs} \\\\\\\\\\title[RM, EDF, LLF]{Proyecto\ 2:\ Calendarizacion\ en\ Tiempo\ Real} \\\\\\\\\\author{Vargas A,\ Camacho A,\ Morales V} \\\\\\\\\\institute[TEC] {Tecnologico\ de\ Costa\ Rica \\\\\\\\\\medskip\\\\\\\\\\textit{avargas@gmail.com,\ acamacho@gmail.com,\ verny.morales@gmail.com}\\\\\\\\\\textit{3er\ Cuatrimestre}}\\\\\\\\\\date{\\\\\\\\\\today} \\\\\\\\\\begin{document}\\\\\\\\\\begin{frame}\\\\\\\\\\titlepage\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Early\ Deadline\ First}\\\\\\\\\\begin{block}{Tipo}Algoritmo\ de\ Scheduling\ Dinamico,\ en\ donde\ las\ tareas\ son\ periodicas.\ Se\ considera\ un\ algoritmo\ optimo\ para\ algoritmos\ de\ prioridades\ dinamicas.\ Es\ un\ algoritmo\ de\ tipo\ expropiativo.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Manejo\ de\ prioridades}El\ nombre\ del\ algoritmo\ indica\ la\ politica\ de\ prioridad.\ La\ prioridad\ es\ inversamente\ proporcional\ al\ tiempo\ que\ falta\ para\ el\ deadline.\ El\ deadline\ de\ cada\ tarea\ es\ igual\ al\ periodo\ de\ la\ misma. \\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Supuestos}Todas\ las\ tareas\ criticas\ son\ periodicas,\ e\ independientes.\ El\ tiempo\ de\ computacion\ se\ conoce\ a\ priori,\ y\ el\ cambio\ de\ contexto\ es\ igual\ a\ cero,\ o\ ya\ esta\ considerado\ en\ el\ tiempo\ de\ computacion.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas\ de\ Scheduling}\\\\\\\\\\begin{theorem}[Parametros\ a\ tomar\ en\ cuenta]$\\\\\\\\\\mu = \\\\\\\\\\Sigma \\\\\\\\\\frac{Ci}{Pi}$ Utilizacion\ del\ CPU\\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Condiciones\ de\ suficiencia]$\\\\\\\\\\mu \\\\\\\\\\leq 1 $ Tareas\ calendarizables,\ ya\ que\ es\ una\ condicion\ necesario\ y\ de\ suficiencia\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame}\\\\\\\\\\end{document} >> doc.tex";
	printf("%s\n",cmd);

	system(cmd);
}

void latex_llf(){
	char cmd[MAX_CMM_LEN] = "echo \\\\\\\\\\documentclass{beamer}\\\\\\\\\\mode'<'presentation'>' {\\\\\\\\\\usetheme{Madrid}}\\\\\\\\\\usepackage{graphicx}\\\\\\\\\\usepackage{booktabs} \\\\\\\\\\title[RM, EDF, LLF]{Proyecto 2: Calendarizacion en Tiempo Real} \\\\\\\\\\author{Vargas A, Camacho A, Morales V} \\\\\\\\\\institute[TEC] {Tecnologico de Costa Rica  \\\\\\\\\\medskip\\\\\\\\\\textit{avargas@gmail.com, acamacho@gmail.com, verny.morales@gmail.com} \\\\\\\\\\textit{3er Cuatrimestre}}\\\\\\\\\\date{\\\\\\\\\\today} \\\\\\\\\\begin{document}\\\\\\\\\\begin{frame}\\\\\\\\\\titlepage\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Least Laxity First}\\\\\\\\\\begin{block}{Tipo y manejo dr prioridades}Es un algoritmo donde las prioridades se manejan de forma dinamica. Por cada unidad de tiempo se deben de evaluar las prioridades para asi conocer el Laxity de cada tarea, y tomar una decision sobre el scheduling de las mismas.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas de Scheduling}\\\\\\\\\\begin{theorem}[Parametros a tomar en cuenta]$ Laxity = Di - Ti + Ci $ Donde D es el deadline proximo de la tarea, T es el tiempo actual de ejecucion y C es el tiempo computacional faltante de la tarea \\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Criterio de Scheduling]Se toma el L menor entre todas las tareas en la cola de Ready y se ejecuta la tarea para cada tiempo de ejecucion\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame}\\\\\\\\\\end{document} >> doc.tex";
	printf("%s\n",cmd);

	system(cmd);
}
void latex_rm_edf(){
	char cmd[MAX_CMM_LEN] = "echo \\\\\\\\\\documentclass{beamer}\\\\\\\\\\mode'<'presentation'>' {\\\\\\\\\\usetheme{Madrid}}\\\\\\\\\\usepackage{graphicx}\\\\\\\\\\usepackage{booktabs}\\\\\\\\\\title[RM, EDF, LLF]{Proyecto\ 2:\ Calendarizacion\ en\ Tiempo\ Real}\\\\\\\\\\author{Vargas A,\ Camacho A,\ Morales V}\\\\\\\\\\institute[TEC]{Tecnologico\ de\ Costa Rica \\\\\\\\\\medskip\\\\\\\\\\textit{avargas@gmail.com,\ acamacho@gmail.com,\ verny.morales@gmail.com}\\\\\\\\\\textit{3er Cuatrimestre}}\\\\\\\\\\date{\\\\\\\\\\today}\\\\\\\\\\begin{document}\\\\\\\\\\begin{frame}\\\\\\\\\\titlepage\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Rate\ Monotonic}\\\\\\\\\\begin{block}{Tipo}Algoritmo\ de\ Scheduling \Dinamico,\ utilizado\ para\ la\ reslucion\ de\ problemas\ caoticos,\ como\ por\ ejemplo\ el\ problema\ de\ los\ carros\ autonomos.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Manejo\ de\ prioridades}Algoritmo\ de\ prioridades\ estaticas,\ esto\ quiere\ decir\ que\ ninguna\ tarea\ puede\ cambiar\ su\ prioridad.\ Donde\ la\ prioridad\ de\ una\ tarea\ siempre\ es\ igual\ a\ su\ periodo.\ Periodo\ mas\ corto,\ mayor\ la\ prioridad.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Supuestos}Todas\ las\ tareas\ criticas\ son\ periodicas,\ e\ independientes.\ El\ tiempo\ de\ computacion\ se\ conoce\ a\ priori,\ y\ el\ cambio\ de\ contexto\ es\ igual\ a\ cero,\ o\ ya\ esta\ considerado\ en\ el\ tiempo\ de\ computacion.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas\ de\ Scheduling}\\\\\\\\\\begin{theorem}[Parametros\ a\ tomar\ en\ cuenta]$\\\\\\\\\\mu = \\\\\\\\\\Sigma \\\\\\\\\\frac{Ci}{Pi}$ Utilizacion\ del\ CPU\$\ 'Un' = n \'2''^'\\\\\\\\\\frac{'1'}{n} - '1' $ donde\ n\ es\ la\ cantidad\ de\ tareas\\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Condiciones\ de\ suficiencia]$\\\\\\\\\\mu \\\\\\\\\\leq Un $ Tareas\ calendarizables\ $\\\\\\\\\\mu \\\\\\\\\\geq Un$ Debido\ a\ que\ es\ una\ condicion\ de\ sufuciencia\ podria\ ser\ calendarizable\$\\\\\\\\\\mu \\\\\\\\\\succ '1'$ Tareas\ no\ calendarizables\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Early\ Deadline\ First}\\\\\\\\\\begin{block}{Tipo}Algoritmo\ de\ Scheduling\ Dinamico,\ en\ donde\ las\ tareas\ son\ periodicas.\ Se\ considera\ un\ algoritmo\ optimo\ para\ algoritmos\ de\ prioridades\ dinamicas.\ Es\ un\ algoritmo\ de\ tipo\ expropiativo.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Manejo\ de\ prioridades}El\ nombre\ del\ algoritmo\ indica\ la\ politica\ de\ prioridad.\ La\ prioridad\ es\ inversamente\ proporcional\ al\ tiempo\ que\ falta\ para\ el\ deadline.\ El\ deadline\ de\ cada\ tarea\ es\ igual\ al\ periodo\ de\ la\ misma. \\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Supuestos}Todas\ las\ tareas\ criticas\ son\ periodicas,\ e\ independientes.\ El\ tiempo\ de\ computacion\ se\ conoce\ a\ priori,\ y\ el\ cambio\ de\ contexto\ es\ igual\ a\ cero,\ o\ ya\ esta\ considerado\ en\ el\ tiempo\ de\ computacion.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas\ de\ Scheduling}\\\\\\\\\\begin{theorem}[Parametros\ a\ tomar\ en\ cuenta]$\\\\\\\\\\mu = \\\\\\\\\\Sigma \\\\\\\\\\frac{Ci}{Pi}$ Utilizacion\ del\ CPU\\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Condiciones\ de\ suficiencia]$\\\\\\\\\\mu \\\\\\\\\\leq 1 $ Tareas\ calendarizables,\ ya\ que\ es\ una\ condicion\ necesario\ y\ de\ suficiencia\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame}\\\\\\\\\\end{document} >> doc.tex";
	printf("%s\n",cmd);

	system(cmd);
}
void latex_rm_lff(){
	char cmd[MAX_CMM_LEN] = "echo \\\\\\\\\\documentclass{beamer}\\\\\\\\\\mode'<'presentation'>' {\\\\\\\\\\usetheme{Madrid}}\\\\\\\\\\usepackage{graphicx}\\\\\\\\\\usepackage{booktabs}\\\\\\\\\\title[RM, EDF, LLF]{Proyecto\ 2:\ Calendarizacion\ en\ Tiempo\ Real}\\\\\\\\\\author{Vargas A,\ Camacho A,\ Morales V}\\\\\\\\\\institute[TEC]{Tecnologico\ de\ Costa Rica \\\\\\\\\\medskip\\\\\\\\\\textit{avargas@gmail.com,\ acamacho@gmail.com,\ verny.morales@gmail.com}\\\\\\\\\\textit{3er Cuatrimestre}}\\\\\\\\\\date{\\\\\\\\\\today}\\\\\\\\\\begin{document}\\\\\\\\\\begin{frame}\\\\\\\\\\titlepage\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Rate\ Monotonic}\\\\\\\\\\begin{block}{Tipo}Algoritmo\ de\ Scheduling \Dinamico,\ utilizado\ para\ la\ reslucion\ de\ problemas\ caoticos,\ como\ por\ ejemplo\ el\ problema\ de\ los\ carros\ autonomos.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Manejo\ de\ prioridades}Algoritmo\ de\ prioridades\ estaticas,\ esto\ quiere\ decir\ que\ ninguna\ tarea\ puede\ cambiar\ su\ prioridad.\ Donde\ la\ prioridad\ de\ una\ tarea\ siempre\ es\ igual\ a\ su\ periodo.\ Periodo\ mas\ corto,\ mayor\ la\ prioridad.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Supuestos}Todas\ las\ tareas\ criticas\ son\ periodicas,\ e\ independientes.\ El\ tiempo\ de\ computacion\ se\ conoce\ a\ priori,\ y\ el\ cambio\ de\ contexto\ es\ igual\ a\ cero,\ o\ ya\ esta\ considerado\ en\ el\ tiempo\ de\ computacion.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas\ de\ Scheduling}\\\\\\\\\\begin{theorem}[Parametros\ a\ tomar\ en\ cuenta]$\\\\\\\\\\mu = \\\\\\\\\\Sigma \\\\\\\\\\frac{Ci}{Pi}$ Utilizacion\ del\ CPU\$\ 'Un' = n \'2''^'\\\\\\\\\\frac{'1'}{n} - '1' $ donde\ n\ es\ la\ cantidad\ de\ tareas\\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Condiciones\ de\ suficiencia]$\\\\\\\\\\mu \\\\\\\\\\leq Un $ Tareas\ calendarizables\ $\\\\\\\\\\mu \\\\\\\\\\geq Un$ Debido\ a\ que\ es\ una\ condicion\ de\ sufuciencia\ podria\ ser\ calendarizable\$\\\\\\\\\\mu \\\\\\\\\\succ '1'$ Tareas\ no\ calendarizables\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Least Laxity First}\\\\\\\\\\begin{block}{Tipo y manejo dr prioridades}Es un algoritmo donde las prioridades se manejan de forma dinamica. Por cada unidad de tiempo se deben de evaluar las prioridades para asi conocer el Laxity de cada tarea, y tomar una decision sobre el scheduling de las mismas.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas de Scheduling}\\\\\\\\\\begin{theorem}[Parametros a tomar en cuenta]$ Laxity = Di - Ti + Ci $ Donde D es el deadline proximo de la tarea, T es el tiempo actual de ejecucion y C es el tiempo computacional faltante de la tarea \\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Criterio de Scheduling]Se toma el L menor entre todas las tareas en la cola de Ready y se ejecuta la tarea para cada tiempo de ejecucion\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame}\\\\\\\\\\end{document}>> doc.tex";
	printf("%s\n",cmd);

	system(cmd);
}
void latex_edf_lff(){
	char cmd[MAX_CMM_LEN] = "echo \\\\\\\\\\documentclass{beamer}\\\\\\\\\\mode'<'presentation'>' {\\\\\\\\\\usetheme{Madrid}}\\\\\\\\\\usepackage{graphicx}\\\\\\\\\\usepackage{booktabs} \\\\\\\\\\title[RM, EDF, LLF]{Proyecto\ 2:\ Calendarizacion\ en\ Tiempo\ Real} \\\\\\\\\\author{Vargas A,\ Camacho A,\ Morales V} \\\\\\\\\\institute[TEC] {Tecnologico\ de\ Costa\ Rica \\\\\\\\\\medskip\\\\\\\\\\textit{avargas@gmail.com,\ acamacho@gmail.com,\ verny.morales@gmail.com}\\\\\\\\\\textit{3er\ Cuatrimestre}}\\\\\\\\\\date{\\\\\\\\\\today} \\\\\\\\\\begin{document}\\\\\\\\\\begin{frame}\\\\\\\\\\titlepage\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Early\ Deadline\ First}\\\\\\\\\\begin{block}{Tipo}Algoritmo\ de\ Scheduling\ Dinamico,\ en\ donde\ las\ tareas\ son\ periodicas.\ Se\ considera\ un\ algoritmo\ optimo\ para\ algoritmos\ de\ prioridades\ dinamicas.\ Es\ un\ algoritmo\ de\ tipo\ expropiativo.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Manejo\ de\ prioridades}El\ nombre\ del\ algoritmo\ indica\ la\ politica\ de\ prioridad.\ La\ prioridad\ es\ inversamente\ proporcional\ al\ tiempo\ que\ falta\ para\ el\ deadline.\ El\ deadline\ de\ cada\ tarea\ es\ igual\ al\ periodo\ de\ la\ misma. \\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Supuestos}Todas\ las\ tareas\ criticas\ son\ periodicas,\ e\ independientes.\ El\ tiempo\ de\ computacion\ se\ conoce\ a\ priori,\ y\ el\ cambio\ de\ contexto\ es\ igual\ a\ cero,\ o\ ya\ esta\ considerado\ en\ el\ tiempo\ de\ computacion.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas\ de\ Scheduling}\\\\\\\\\\begin{theorem}[Parametros\ a\ tomar\ en\ cuenta]$\\\\\\\\\\mu = \\\\\\\\\\Sigma \\\\\\\\\\frac{Ci}{Pi}$ Utilizacion\ del\ CPU\\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Condiciones\ de\ suficiencia]$\\\\\\\\\\mu \\\\\\\\\\leq 1 $ Tareas\ calendarizables,\ ya\ que\ es\ una\ condicion\ necesario\ y\ de\ suficiencia\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Least Laxity First}\\\\\\\\\\begin{block}{Tipo y manejo dr prioridades}Es un algoritmo donde las prioridades se manejan de forma dinamica. Por cada unidad de tiempo se deben de evaluar las prioridades para asi conocer el Laxity de cada tarea, y tomar una decision sobre el scheduling de las mismas.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas de Scheduling}\\\\\\\\\\begin{theorem}[Parametros a tomar en cuenta]$ Laxity = Di - Ti + Ci $ Donde D es el deadline proximo de la tarea, T es el tiempo actual de ejecucion y C es el tiempo computacional faltante de la tarea \\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Criterio de Scheduling]Se toma el L menor entre todas las tareas en la cola de Ready y se ejecuta la tarea para cada tiempo de ejecucion\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame}\\\\\\\\\\end{document} >> doc.tex";
	printf("%s\n",cmd);

	system(cmd);
}
void latex_rm_edf_lff(){
	char cmd[MAX_CMM_LEN] = "echo \\\\\\\\\\documentclass{beamer}\\\\\\\\\\mode'<'presentation'>' {\\\\\\\\\\usetheme{Madrid}}\\\\\\\\\\usepackage{graphicx}\\\\\\\\\\usepackage{booktabs}\\\\\\\\\\title[RM, EDF, LLF]{Proyecto\ 2:\ Calendarizacion\ en\ Tiempo\ Real}\\\\\\\\\\author{Vargas A,\ Camacho A,\ Morales V}\\\\\\\\\\institute[TEC]{Tecnologico\ de\ Costa Rica \\\\\\\\\\medskip\\\\\\\\\\textit{avargas@gmail.com,\ acamacho@gmail.com,\ verny.morales@gmail.com}\\\\\\\\\\textit{3er Cuatrimestre}}\\\\\\\\\\date{\\\\\\\\\\today}\\\\\\\\\\begin{document}\\\\\\\\\\begin{frame}\\\\\\\\\\titlepage\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Rate\ Monotonic}\\\\\\\\\\begin{block}{Tipo}Algoritmo\ de\ Scheduling \Dinamico,\ utilizado\ para\ la\ reslucion\ de\ problemas\ caoticos,\ como\ por\ ejemplo\ el\ problema\ de\ los\ carros\ autonomos.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Manejo\ de\ prioridades}Algoritmo\ de\ prioridades\ estaticas,\ esto\ quiere\ decir\ que\ ninguna\ tarea\ puede\ cambiar\ su\ prioridad.\ Donde\ la\ prioridad\ de\ una\ tarea\ siempre\ es\ igual\ a\ su\ periodo.\ Periodo\ mas\ corto,\ mayor\ la\ prioridad.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Supuestos}Todas\ las\ tareas\ criticas\ son\ periodicas,\ e\ independientes.\ El\ tiempo\ de\ computacion\ se\ conoce\ a\ priori,\ y\ el\ cambio\ de\ contexto\ es\ igual\ a\ cero,\ o\ ya\ esta\ considerado\ en\ el\ tiempo\ de\ computacion.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas\ de\ Scheduling}\\\\\\\\\\begin{theorem}[Parametros\ a\ tomar\ en\ cuenta]$\\\\\\\\\\mu = \\\\\\\\\\Sigma \\\\\\\\\\frac{Ci}{Pi}$ Utilizacion\ del\ CPU\$\ 'Un' = n \'2''^'\\\\\\\\\\frac{'1'}{n} - '1' $ donde\ n\ es\ la\ cantidad\ de\ tareas\\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Condiciones\ de\ suficiencia]$\\\\\\\\\\mu \\\\\\\\\\leq Un $ Tareas\ calendarizables\ $\\\\\\\\\\mu \\\\\\\\\\geq Un$ Debido\ a\ que\ es\ una\ condicion\ de\ sufuciencia\ podria\ ser\ calendarizable\$\\\\\\\\\\mu \\\\\\\\\\succ '1'$ Tareas\ no\ calendarizables\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Early\ Deadline\ First}\\\\\\\\\\begin{block}{Tipo}Algoritmo\ de\ Scheduling\ Dinamico,\ en\ donde\ las\ tareas\ son\ periodicas.\ Se\ considera\ un\ algoritmo\ optimo\ para\ algoritmos\ de\ prioridades\ dinamicas.\ Es\ un\ algoritmo\ de\ tipo\ expropiativo.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Manejo\ de\ prioridades}El\ nombre\ del\ algoritmo\ indica\ la\ politica\ de\ prioridad.\ La\ prioridad\ es\ inversamente\ proporcional\ al\ tiempo\ que\ falta\ para\ el\ deadline.\ El\ deadline\ de\ cada\ tarea\ es\ igual\ al\ periodo\ de\ la\ misma. \\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Supuestos}Todas\ las\ tareas\ criticas\ son\ periodicas,\ e\ independientes.\ El\ tiempo\ de\ computacion\ se\ conoce\ a\ priori,\ y\ el\ cambio\ de\ contexto\ es\ igual\ a\ cero,\ o\ ya\ esta\ considerado\ en\ el\ tiempo\ de\ computacion.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas\ de\ Scheduling}\\\\\\\\\\begin{theorem}[Parametros\ a\ tomar\ en\ cuenta]$\\\\\\\\\\mu = \\\\\\\\\\Sigma \\\\\\\\\\frac{Ci}{Pi}$ Utilizacion\ del\ CPU\\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Condiciones\ de\ suficiencia]$\\\\\\\\\\mu \\\\\\\\\\leq 1 $ Tareas\ calendarizables,\ ya\ que\ es\ una\ condicion\ necesario\ y\ de\ suficiencia\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Least Laxity First}\\\\\\\\\\begin{block}{Tipo y manejo dr prioridades}Es un algoritmo donde las prioridades se manejan de forma dinamica. Por cada unidad de tiempo se deben de evaluar las prioridades para asi conocer el Laxity de cada tarea, y tomar una decision sobre el scheduling de las mismas.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas de Scheduling}\\\\\\\\\\begin{theorem}[Parametros a tomar en cuenta]$ Laxity = Di - Ti + Ci $ Donde D es el deadline proximo de la tarea, T es el tiempo actual de ejecucion y C es el tiempo computacional faltante de la tarea \\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Criterio de Scheduling]Se toma el L menor entre todas las tareas en la cola de Ready y se ejecuta la tarea para cada tiempo de ejecucion\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame}\\\\\\\\\\end{document} >> doc.tex";
	printf("%s\n",cmd);

	system(cmd);
}
