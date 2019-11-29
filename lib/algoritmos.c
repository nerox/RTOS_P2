
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include <interfaz.h>
#include <algoritmos.h>


uint NUMBER_OF_TASKS = 0;
uint ALGORITHMS_TO_EXECUTE;
uint AMOUNT_OF_PERIODS;
int CRASH;
int current_period;
task_information *TASKS_TO_SCHEDULE;
task_information *WAIT_LIST;
int * scheduled_Matrix;
void EDF(){
	cleanOutPutMatrix();
	resetValues();
	EDF_schedulability_calculation();
	CRASH=0;
	current_period=0;
	deploy();
	while(current_period<AMOUNT_OF_PERIODS){
		current_period++;
		Verify_Crash();
		if  (CRASH==1){
			int i;
			for(i=0;i<NUMBER_OF_TASKS;i++){
				scheduled_Matrix[i*AMOUNT_OF_PERIODS + (current_period)]=6;
			}

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
	cleanOutPutMatrix();
	resetValues();
	RM_schedulability_calculation();
	CRASH=0;
	current_period=0;
	deploy();
	while(current_period<AMOUNT_OF_PERIODS){
		current_period++;
		Verify_Crash();
		if  (CRASH==1){
			int i;
			for(i=0;i<NUMBER_OF_TASKS;i++){
				scheduled_Matrix[i*AMOUNT_OF_PERIODS + (current_period)]=6;
			}

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
		scheduled_Matrix[WAIT_LIST[position].id*AMOUNT_OF_PERIODS + (current_period)]=WAIT_LIST[position].id;
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
		scheduled_Matrix[WAIT_LIST[position].id*AMOUNT_OF_PERIODS + (current_period)]=WAIT_LIST[position].id;
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
	cleanOutPutMatrix();
	resetValues();
	//EDF_schedulability_calculation(); Vernal agregue un metodo que diga si el algoritmo es calendarizable o no
	CRASH=0;
	current_period=0;
	deploy();
	while(current_period<AMOUNT_OF_PERIODS){
		current_period++;
		Verify_Crash();
		if  (CRASH==1){
			int i;
			for(i=0;i<NUMBER_OF_TASKS;i++){
				scheduled_Matrix[i*AMOUNT_OF_PERIODS + current_period]=6;
			}

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
		scheduled_Matrix[WAIT_LIST[position].id*AMOUNT_OF_PERIODS + current_period]=WAIT_LIST[position].id;
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
void create_tasks(){
	AMOUNT_OF_PERIODS=24;

	WAIT_LIST =  (task_information *)malloc(NUMBER_OF_TASKS*2 * sizeof(task_information));

  for (uint i = 0; i < NUMBER_OF_TASKS; i++) {
    printf("Task ID: %d Ci = %d Di = %d\n", TASKS_TO_SCHEDULE[i].id, TASKS_TO_SCHEDULE[i].computation_time,TASKS_TO_SCHEDULE[i].period_time);
  }
	scheduled_Matrix= (int *)malloc((NUMBER_OF_TASKS * AMOUNT_OF_PERIODS)* sizeof(int));
	remove("./docs/simulacion.tex");
	switch (ALGORITHMS_TO_EXECUTE)
	{
	case 1:
		printf("User selected to execute RM\n");
		printf("Performing the RM simulation \n");
		latex_rm();
		RM();
		latex_table_slide_header("RM");
		latex_table_slide("RM");
		end_latex_slide();
		break;
	case 2:
		printf("User selected to execute EDF\n");
		printf("Performing the EDF simulation \n");
		latex_edf();
		EDF();
		latex_table_slide_header("EDF");
		latex_table_slide("EDF");
		end_latex_slide();
		break;
	case 3:
		printf("User selected to execute LLF\n");
		printf("Performing the LLF simulation \n");
		latex_llf();
		LLF();
		latex_table_slide_header("LLF");
		latex_table_slide("LLF");
		end_latex_slide();
		break;
	case 4:
		printf("User selected to execute RM & EDF\n");
		printf("Performing the RM simulation \n");
		latex_rm_edf();
		RM();
		if(slide==0){
			latex_table_slide_header("All");
			latex_table_slide("RM");
		}
		else{
			latex_table_slide_header("RM");
			latex_table_slide("RM");
			end_latex_slide();
		}
		printf("Performing the EDF simulation \n");
		EDF();
		if(slide==0){
			latex_table_slide("EDF");
			end_latex_slide();
		}
		else{
			latex_table_slide_header("EDF");
			latex_table_slide("EDF");
			end_latex_slide();
		}
		break;
	case 5:
		printf("User selected to execute RM & LLF\n");
		printf("Performing the RM simulation \n");
		latex_rm_lff();
		RM();
		if(slide==0){
			latex_table_slide_header("All");
			latex_table_slide("RM");
		}
		else{
			latex_table_slide_header("RM");
			latex_table_slide("RM");
			end_latex_slide();
		}
		printf("Performing the LLF simulation \n");
		LLF();
		if(slide==0){
			latex_table_slide("LLF");
			end_latex_slide();
		}
		else{
			latex_table_slide_header("LLF");
			latex_table_slide("LLF");
			end_latex_slide();
		}
		break;
	case 6:
		printf("User selected to execute EDF & LLF\n");
		printf("Performing the EDF simulation \n");
		latex_edf_lff();
		EDF();
		if(slide==0){
			latex_table_slide_header("All");
			latex_table_slide("EDF");
		}
		else{
			latex_table_slide_header("EDF");
			latex_table_slide("EDF");
			end_latex_slide();
		}
		printf("Performing the LLF simulation \n");
		LLF();
		if(slide==0){
			latex_table_slide("LLF");
			end_latex_slide();
		}
		else{
			latex_table_slide_header("LLF");
			latex_table_slide("LLF");
			end_latex_slide();
		}
		break;
	case 7:
		printf("User selected to execute RM & EDF & LLF\n");
		printf("Performing the RM simulation \n");
		latex_rm_edf_lff();
		RM();
		if(slide==0){
			latex_table_slide_header("All");
			latex_table_slide("RM");
		}
		else{
			latex_table_slide_header("RM");
			latex_table_slide("RM");
			end_latex_slide();
		}
		printf("Performing the EDF simulation \n");
		EDF();
		if(slide==0){
			latex_table_slide("EDF");
		}
		else{
			latex_table_slide_header("EDF");
			latex_table_slide("EDF");
			end_latex_slide();
		}
		printf("Performing the LLF simulation \n");
		LLF();
		if(slide==0){
			latex_table_slide("LLF");
			end_latex_slide();
		}
		else{
			latex_table_slide_header("LLF");
			latex_table_slide("LLF");
			end_latex_slide();
		}
		break;
	default: // code to be executed if n doesn't match any cases
		printf("Not a valid option selected\n");
		break;
	}
	endlatex();
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
void latex_table_slide_header(char algorithm []){
	FILE *out = fopen("./docs/simulacion.tex", "a");  
	fprintf(out, "%s", "\\begin{frame}\\frametitle{");
	fprintf(out,"%s",algorithm);
	fprintf(out,"%s"," Scheduling Results}");
	fclose(out); 
}
void end_latex_slide(){
	FILE *out = fopen("./docs/simulacion.tex", "a");  
	fprintf(out, "%s","\\end{frame}");
	fclose(out);   
}
void latex_table_slide(char algorithm []){
	char tables[MAX_CMM_LEN]="";
	FILE *out = fopen("./docs/simulacion.tex", "a");  
	fprintf(out, "%s","\\begin{table}\\adjustbox{max height=\\dimexpr\\textheight-5.5cm\\relax,max width=\\textwidth}{\\begin{tabular}");  
	strcat(tables, "{l");
	int i,j;
	for (i=0;i<AMOUNT_OF_PERIODS+2;i++){
		strcat(tables, "|c");
	}
	strcat(tables, "}\n");
	strcat(tables, " ");
	char intstr[30];
	for (i=0;i<AMOUNT_OF_PERIODS+1;i++){
		sprintf(intstr, "&%d", i);
		strcat(tables, intstr);
	}
	strcat(tables, "\\\\ \n \\hline \n");
	for (i=0;i<NUMBER_OF_TASKS;i++){
		sprintf(intstr, "Task %d", i);
		strcat(tables, intstr);
		for (j=1;j<=AMOUNT_OF_PERIODS+1;j++){
			int COLOR=scheduled_Matrix[i*AMOUNT_OF_PERIODS-1 + j];
			switch (COLOR)
			{
			case 0:
				strcat(tables, "&\\cellcolor{blue}");
				break;
			case 1:
				strcat(tables, "&\\cellcolor{green}");
				break;
			case 2:
				strcat(tables, "&\\cellcolor{cyan}");
				break;
			case 3:
				strcat(tables, "&\\cellcolor{magenta}");
				break;
			case 4:
				strcat(tables, "&\\cellcolor{yellow}");
				break;
			case 5:
				strcat(tables, "&\\cellcolor{orange}");
				break;
			case 6:
				strcat(tables, "&\\cellcolor{red}");
				break;
			default: // code to be executed if n doesn't match any cases
				strcat(tables, "& ");
				break;
			}
		}
		strcat(tables, "&\\\\ \n");
	}
	strcat(tables, "\\hline \n");
	sprintf(intstr, "Task Arrivals");
	strcat(tables, intstr);
	for (j=0;j<AMOUNT_OF_PERIODS+1;j++){
			int COLOR=-1;
			for (i=0;i<NUMBER_OF_TASKS;i++){
				if(j%TASKS_TO_SCHEDULE[i].period_time==0){
					COLOR=TASKS_TO_SCHEDULE[i].id;
				}
			}
			switch (COLOR)
			{
			case 0:
				strcat(tables, "&\\cellcolor{blue}");
				break;
			case 1:
				strcat(tables, "&\\cellcolor{green}");
				break;
			case 2:
				strcat(tables, "&\\cellcolor{cyan}");
				break;
			case 3:
				strcat(tables, "&\\cellcolor{magenta}");
				break;
			case 4:
				strcat(tables, "&\\cellcolor{yellow}");
				break;
			case 5:
				strcat(tables, "&\\cellcolor{orange}");
				break;
			case 6:
				strcat(tables, "&\\cellcolor{red}");
				break;
			default: // code to be executed if n doesn't match any cases
				strcat(tables, "& ");
				break;
			}
		}
	strcat(tables, "&\\\\ \n");
	fprintf(out, "%s", tables);  
	fprintf(out, "%s", "\\end{tabular}\n}\n\\caption{");
	fprintf(out,"%s",algorithm);
	fprintf(out, "%s"," Simulation results}\\vspace{-1.5em}\\end{table}");
	fclose(out); 
}
void endlatex(){
	char cmd[MAX_CMM_LEN] = "echo \\\\\\\\\\end{document} >> ./docs/simulacion.tex";
	system(cmd);
}
void cleanOutPutMatrix(){

	int i,j;
	for (i = 0; i < NUMBER_OF_TASKS; i++)
	{
	for (j = 0; j < AMOUNT_OF_PERIODS; j++)
	{
		scheduled_Matrix[i*AMOUNT_OF_PERIODS + j]=-1;
	}
	}
}

void latex_rm(){
	char cmd[MAX_CMM_LEN] = "echo \\\\\\\\\\documentclass{beamer}\\\\\\\\\\mode'<'presentation'>' {\\\\\\\\\\usetheme{Madrid}}\\\\\\\\\\usepackage{booktabs,adjustbox}\\\\\\\\\\usepackage{xcolor,colortbl}\\\\\\\\\\title[RM, EDF, LLF]{Proyecto\\ 2:\\ Calendarizacion\\ en\\ Tiempo\\ Real}\\\\\\\\\\author{Vargas A,\\ Camacho A,\\ Morales V}\\\\\\\\\\institute[TEC]{Tecnologico\\ de\\ Costa Rica \\\\\\\\\\medskip\\\\\\\\\\textit{avargas@gmail.com,\\ acamacho@gmail.com,\\ verny.morales@gmail.com}\\\\\\\\\\textit{3er Cuatrimestre}}\\\\\\\\\\date{\\\\\\\\\\today}\\\\\\\\\\begin{document}\\\\\\\\\\begin{frame}\\\\\\\\\\titlepage\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Rate\\ Monotonic}\\\\\\\\\\begin{block}{Tipo}Algoritmo\\ de\\ Scheduling \\Dinamico,\\ utilizado\\ para\\ la\\ reslucion\\ de\\ problemas\\ caoticos,\\ como\\ por\\ ejemplo\\ el\\ problema\\ de\\ los\\ carros\\ autonomos.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Manejo\\ de\\ prioridades}Algoritmo\\ de\\ prioridades\\ estaticas,\\ esto\\ quiere\\ decir\\ que\\ ninguna\\ tarea\\ puede\\ cambiar\\ su\\ prioridad.\\ Donde\\ la\\ prioridad\\ de\\ una\\ tarea\\ siempre\\ es\\ igual\\ a\\ su\\ periodo.\\ Periodo\\ mas\\ corto,\\ mayor\\ la\\ prioridad.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Supuestos}Todas\\ las\\ tareas\\ criticas\\ son\\ periodicas,\\ e\\ independientes.\\ El\\ tiempo\\ de\\ computacion\\ se\\ conoce\\ a\\ priori,\\ y\\ el\\ cambio\\ de\\ contexto\\ es\\ igual\\ a\\ cero,\\ o\\ ya\\ esta\\ considerado\\ en\\ el\\ tiempo\\ de\\ computacion.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas\\ de\\ Scheduling}\\\\\\\\\\begin{theorem}[Parametros\\ a\\ tomar\\ en\\ cuenta]$\\\\\\\\\\mu = \\\\\\\\\\Sigma \\\\\\\\\\frac{Ci}{Pi}$ Utilizacion\\ del\\ CPU\\$\\ 'Un' = n \'2''^'\\\\\\\\\\frac{'1'}{n} - '1' $ donde\\ n\\ es\\ la\\ cantidad\\ de\\ tareas\\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Condiciones\\ de\\ suficiencia]$\\\\\\\\\\mu \\\\\\\\\\leq Un $ Tareas\\ calendarizables\\ $\\\\\\\\\\mu \\\\\\\\\\geq Un$ Debido\\ a\\ que\\ es\\ una\\ condicion\\ de\\ sufuciencia\\ podria\\ ser\\ calendarizable\\$\\\\\\\\\\mu \\\\\\\\\\succ '1'$ Tareas\\ no\\ calendarizables\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame} >> ./docs/simulacion.tex";
	printf("%s\n",cmd);

	system(cmd);
}
void latex_edf(){
	char cmd[MAX_CMM_LEN] = "echo \\\\\\\\\\documentclass{beamer}\\\\\\\\\\mode'<'presentation'>' {\\\\\\\\\\usetheme{Madrid}}\\\\\\\\\\usepackage{booktabs,adjustbox}\\\\\\\\\\usepackage{xcolor,colortbl}\\\\\\\\\\title[RM, EDF, LLF]{Proyecto\\ 2:\\ Calendarizacion\\ en\\ Tiempo\\ Real} \\\\\\\\\\author{Vargas A,\\ Camacho A,\\ Morales V} \\\\\\\\\\institute[TEC] {Tecnologico\\ de\\ Costa\\ Rica \\\\\\\\\\medskip\\\\\\\\\\textit{avargas@gmail.com,\\ acamacho@gmail.com,\\ verny.morales@gmail.com}\\\\\\\\\\textit{3er\\ Cuatrimestre}}\\\\\\\\\\date{\\\\\\\\\\today} \\\\\\\\\\begin{document}\\\\\\\\\\begin{frame}\\\\\\\\\\titlepage\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Early\\ Deadline\\ First}\\\\\\\\\\begin{block}{Tipo}Algoritmo\\ de\\ Scheduling\\ Dinamico,\\ en\\ donde\\ las\\ tareas\\ son\\ periodicas.\\ Se\\ considera\\ un\\ algoritmo\\ optimo\\ para\\ algoritmos\\ de\\ prioridades\\ dinamicas.\\ Es\\ un\\ algoritmo\\ de\\ tipo\\ expropiativo.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Manejo\\ de\\ prioridades}El\\ nombre\\ del\\ algoritmo\\ indica\\ la\\ politica\\ de\\ prioridad.\\ La\\ prioridad\\ es\\ inversamente\\ proporcional\\ al\\ tiempo\\ que\\ falta\\ para\\ el\\ deadline.\\ El\\ deadline\\ de\\ cada\\ tarea\\ es\\ igual\\ al\\ periodo\\ de\\ la\\ misma. \\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Supuestos}Todas\\ las\\ tareas\\ criticas\\ son\\ periodicas,\\ e\\ independientes.\\ El\\ tiempo\\ de\\ computacion\\ se\\ conoce\\ a\\ priori,\\ y\\ el\\ cambio\\ de\\ contexto\\ es\\ igual\\ a\\ cero,\\ o\\ ya\\ esta\\ considerado\\ en\\ el\\ tiempo\\ de\\ computacion.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas\\ de\\ Scheduling}\\\\\\\\\\begin{theorem}[Parametros\\ a\\ tomar\\ en\\ cuenta]$\\\\\\\\\\mu = \\\\\\\\\\Sigma \\\\\\\\\\frac{Ci}{Pi}$ Utilizacion\\ del\\ CPU\\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Condiciones\\ de\\ suficiencia]$\\\\\\\\\\mu \\\\\\\\\\leq 1 $ Tareas\\ calendarizables,\\ ya\\ que\\ es\\ una\\ condicion\\ necesario\\ y\\ de\\ suficiencia\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame} >> ./docs/simulacion.tex";
	printf("%s\n",cmd);

	system(cmd);
}

void latex_llf(){
	char cmd[MAX_CMM_LEN] = "echo \\\\\\\\\\documentclass{beamer}\\\\\\\\\\mode'<'presentation'>' {\\\\\\\\\\usetheme{Madrid}}\\\\\\\\\\usepackage{booktabs,adjustbox}\\\\\\\\\\usepackage{xcolor,colortbl}\\\\\\\\\\title[RM, EDF, LLF]{Proyecto 2: Calendarizacion en Tiempo Real} \\\\\\\\\\author{Vargas A, Camacho A, Morales V} \\\\\\\\\\institute[TEC] {Tecnologico de Costa Rica  \\\\\\\\\\medskip\\\\\\\\\\textit{avargas@gmail.com, acamacho@gmail.com, verny.morales@gmail.com} \\\\\\\\\\textit{3er Cuatrimestre}}\\\\\\\\\\date{\\\\\\\\\\today} \\\\\\\\\\begin{document}\\\\\\\\\\begin{frame}\\\\\\\\\\titlepage\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Least Laxity First}\\\\\\\\\\begin{block}{Tipo y manejo dr prioridades}Es un algoritmo donde las prioridades se manejan de forma dinamica. Por cada unidad de tiempo se deben de evaluar las prioridades para asi conocer el Laxity de cada tarea, y tomar una decision sobre el scheduling de las mismas.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas de Scheduling}\\\\\\\\\\begin{theorem}[Parametros a tomar en cuenta]$ Laxity = Di - Ti + Ci $ Donde D es el deadline proximo de la tarea, T es el tiempo actual de ejecucion y C es el tiempo computacional faltante de la tarea \\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Criterio de Scheduling]Se toma el L menor entre todas las tareas en la cola de Ready y se ejecuta la tarea para cada tiempo de ejecucion\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame} >> ./docs/simulacion.tex";
	printf("%s\n",cmd);

	system(cmd);
}
void latex_rm_edf(){
	char cmd[MAX_CMM_LEN] = "echo \\\\\\\\\\documentclass{beamer}\\\\\\\\\\mode'<'presentation'>' {\\\\\\\\\\usetheme{Madrid}}\\\\\\\\\\usepackage{booktabs,adjustbox}\\\\\\\\\\usepackage{xcolor,colortbl}\\\\\\\\\\title[RM, EDF, LLF]{Proyecto\\ 2:\\ Calendarizacion\\ en\\ Tiempo\\ Real}\\\\\\\\\\author{Vargas A,\\ Camacho A,\\ Morales V}\\\\\\\\\\institute[TEC]{Tecnologico\\ de\\ Costa Rica \\\\\\\\\\medskip\\\\\\\\\\textit{avargas@gmail.com,\\ acamacho@gmail.com,\\ verny.morales@gmail.com}\\\\\\\\\\textit{3er Cuatrimestre}}\\\\\\\\\\date{\\\\\\\\\\today}\\\\\\\\\\begin{document}\\\\\\\\\\begin{frame}\\\\\\\\\\titlepage\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Rate\\ Monotonic}\\\\\\\\\\begin{block}{Tipo}Algoritmo\\ de\\ Scheduling \\Dinamico,\\ utilizado\\ para\\ la\\ reslucion\\ de\\ problemas\\ caoticos,\\ como\\ por\\ ejemplo\\ el\\ problema\\ de\\ los\\ carros\\ autonomos.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Manejo\\ de\\ prioridades}Algoritmo\\ de\\ prioridades\\ estaticas,\\ esto\\ quiere\\ decir\\ que\\ ninguna\\ tarea\\ puede\\ cambiar\\ su\\ prioridad.\\ Donde\\ la\\ prioridad\\ de\\ una\\ tarea\\ siempre\\ es\\ igual\\ a\\ su\\ periodo.\\ Periodo\\ mas\\ corto,\\ mayor\\ la\\ prioridad.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Supuestos}Todas\\ las\\ tareas\\ criticas\\ son\\ periodicas,\\ e\\ independientes.\\ El\\ tiempo\\ de\\ computacion\\ se\\ conoce\\ a\\ priori,\\ y\\ el\\ cambio\\ de\\ contexto\\ es\\ igual\\ a\\ cero,\\ o\\ ya\\ esta\\ considerado\\ en\\ el\\ tiempo\\ de\\ computacion.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas\\ de\\ Scheduling}\\\\\\\\\\begin{theorem}[Parametros\\ a\\ tomar\\ en\\ cuenta]$\\\\\\\\\\mu = \\\\\\\\\\Sigma \\\\\\\\\\frac{Ci}{Pi}$ Utilizacion\\ del\\ CPU\\$\\ 'Un' = n \'2''^'\\\\\\\\\\frac{'1'}{n} - '1' $ donde\\ n\\ es\\ la\\ cantidad\\ de\\ tareas\\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Condiciones\\ de\\ suficiencia]$\\\\\\\\\\mu \\\\\\\\\\leq Un $ Tareas\\ calendarizables\\ $\\\\\\\\\\mu \\\\\\\\\\geq Un$ Debido\\ a\\ que\\ es\\ una\\ condicion\\ de\\ sufuciencia\\ podria\\ ser\\ calendarizable\\$\\\\\\\\\\mu \\\\\\\\\\succ '1'$ Tareas\\ no\\ calendarizables\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Early\\ Deadline\\ First}\\\\\\\\\\begin{block}{Tipo}Algoritmo\\ de\\ Scheduling\\ Dinamico,\\ en\\ donde\\ las\\ tareas\\ son\\ periodicas.\\ Se\\ considera\\ un\\ algoritmo\\ optimo\\ para\\ algoritmos\\ de\\ prioridades\\ dinamicas.\\ Es\\ un\\ algoritmo\\ de\\ tipo\\ expropiativo.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Manejo\\ de\\ prioridades}El\\ nombre\\ del\\ algoritmo\\ indica\\ la\\ politica\\ de\\ prioridad.\\ La\\ prioridad\\ es\\ inversamente\\ proporcional\\ al\\ tiempo\\ que\\ falta\\ para\\ el\\ deadline.\\ El\\ deadline\\ de\\ cada\\ tarea\\ es\\ igual\\ al\\ periodo\\ de\\ la\\ misma. \\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Supuestos}Todas\\ las\\ tareas\\ criticas\\ son\\ periodicas,\\ e\\ independientes.\\ El\\ tiempo\\ de\\ computacion\\ se\\ conoce\\ a\\ priori,\\ y\\ el\\ cambio\\ de\\ contexto\\ es\\ igual\\ a\\ cero,\\ o\\ ya\\ esta\\ considerado\\ en\\ el\\ tiempo\\ de\\ computacion.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas\\ de\\ Scheduling}\\\\\\\\\\begin{theorem}[Parametros\\ a\\ tomar\\ en\\ cuenta]$\\\\\\\\\\mu = \\\\\\\\\\Sigma \\\\\\\\\\frac{Ci}{Pi}$ Utilizacion\\ del\\ CPU\\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Condiciones\\ de\\ suficiencia]$\\\\\\\\\\mu \\\\\\\\\\leq 1 $ Tareas\\ calendarizables,\\ ya\\ que\\ es\\ una\\ condicion\\ necesario\\ y\\ de\\ suficiencia\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame} >> ./docs/simulacion.tex";
	printf("%s\n",cmd);

	system(cmd);
}
void latex_rm_lff(){
	char cmd[MAX_CMM_LEN] = "echo \\\\\\\\\\documentclass{beamer}\\\\\\\\\\mode'<'presentation'>' {\\\\\\\\\\usetheme{Madrid}}\\\\\\\\\\usepackage{booktabs,adjustbox}\\\\\\\\\\usepackage{xcolor,colortbl}\\\\\\\\\\title[RM, EDF, LLF]{Proyecto\\ 2:\\ Calendarizacion\\ en\\ Tiempo\\ Real}\\\\\\\\\\author{Vargas A,\\ Camacho A,\\ Morales V}\\\\\\\\\\institute[TEC]{Tecnologico\\ de\\ Costa Rica \\\\\\\\\\medskip\\\\\\\\\\textit{avargas@gmail.com,\\ acamacho@gmail.com,\\ verny.morales@gmail.com}\\\\\\\\\\textit{3er Cuatrimestre}}\\\\\\\\\\date{\\\\\\\\\\today}\\\\\\\\\\begin{document}\\\\\\\\\\begin{frame}\\\\\\\\\\titlepage\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Rate\\ Monotonic}\\\\\\\\\\begin{block}{Tipo}Algoritmo\\ de\\ Scheduling \\Dinamico,\\ utilizado\\ para\\ la\\ reslucion\\ de\\ problemas\\ caoticos,\\ como\\ por\\ ejemplo\\ el\\ problema\\ de\\ los\\ carros\\ autonomos.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Manejo\\ de\\ prioridades}Algoritmo\\ de\\ prioridades\\ estaticas,\\ esto\\ quiere\\ decir\\ que\\ ninguna\\ tarea\\ puede\\ cambiar\\ su\\ prioridad.\\ Donde\\ la\\ prioridad\\ de\\ una\\ tarea\\ siempre\\ es\\ igual\\ a\\ su\\ periodo.\\ Periodo\\ mas\\ corto,\\ mayor\\ la\\ prioridad.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Supuestos}Todas\\ las\\ tareas\\ criticas\\ son\\ periodicas,\\ e\\ independientes.\\ El\\ tiempo\\ de\\ computacion\\ se\\ conoce\\ a\\ priori,\\ y\\ el\\ cambio\\ de\\ contexto\\ es\\ igual\\ a\\ cero,\\ o\\ ya\\ esta\\ considerado\\ en\\ el\\ tiempo\\ de\\ computacion.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas\\ de\\ Scheduling}\\\\\\\\\\begin{theorem}[Parametros\\ a\\ tomar\\ en\\ cuenta]$\\\\\\\\\\mu = \\\\\\\\\\Sigma \\\\\\\\\\frac{Ci}{Pi}$ Utilizacion\\ del\\ CPU\\$\\ 'Un' = n \'2''^'\\\\\\\\\\frac{'1'}{n} - '1' $ donde\\ n\\ es\\ la\\ cantidad\\ de\\ tareas\\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Condiciones\\ de\\ suficiencia]$\\\\\\\\\\mu \\\\\\\\\\leq Un $ Tareas\\ calendarizables\\ $\\\\\\\\\\mu \\\\\\\\\\geq Un$ Debido\\ a\\ que\\ es\\ una\\ condicion\\ de\\ sufuciencia\\ podria\\ ser\\ calendarizable\\$\\\\\\\\\\mu \\\\\\\\\\succ '1'$ Tareas\\ no\\ calendarizables\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Least Laxity First}\\\\\\\\\\begin{block}{Tipo y manejo dr prioridades}Es un algoritmo donde las prioridades se manejan de forma dinamica. Por cada unidad de tiempo se deben de evaluar las prioridades para asi conocer el Laxity de cada tarea, y tomar una decision sobre el scheduling de las mismas.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas de Scheduling}\\\\\\\\\\begin{theorem}[Parametros a tomar en cuenta]$ Laxity = Di - Ti + Ci $ Donde D es el deadline proximo de la tarea, T es el tiempo actual de ejecucion y C es el tiempo computacional faltante de la tarea \\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Criterio de Scheduling]Se toma el L menor entre todas las tareas en la cola de Ready y se ejecuta la tarea para cada tiempo de ejecucion\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame} >> ./docs/simulacion.tex";
	printf("%s\n",cmd);

	system(cmd);
}
void latex_edf_lff(){
	char cmd[MAX_CMM_LEN] = "echo \\\\\\\\\\documentclass{beamer}\\\\\\\\\\mode'<'presentation'>' {\\\\\\\\\\usetheme{Madrid}}\\\\\\\\\\usepackage{booktabs,adjustbox}\\\\\\\\\\usepackage{xcolor,colortbl}\\\\\\\\\\title[RM, EDF, LLF]{Proyecto\\ 2:\\ Calendarizacion\\ en\\ Tiempo\\ Real} \\\\\\\\\\author{Vargas A,\\ Camacho A,\\ Morales V} \\\\\\\\\\institute[TEC] {Tecnologico\\ de\\ Costa\\ Rica \\\\\\\\\\medskip\\\\\\\\\\textit{avargas@gmail.com,\\ acamacho@gmail.com,\\ verny.morales@gmail.com}\\\\\\\\\\textit{3er\\ Cuatrimestre}}\\\\\\\\\\date{\\\\\\\\\\today} \\\\\\\\\\begin{document}\\\\\\\\\\begin{frame}\\\\\\\\\\titlepage\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Early\\ Deadline\\ First}\\\\\\\\\\begin{block}{Tipo}Algoritmo\\ de\\ Scheduling\\ Dinamico,\\ en\\ donde\\ las\\ tareas\\ son\\ periodicas.\\ Se\\ considera\\ un\\ algoritmo\\ optimo\\ para\\ algoritmos\\ de\\ prioridades\\ dinamicas.\\ Es\\ un\\ algoritmo\\ de\\ tipo\\ expropiativo.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Manejo\\ de\\ prioridades}El\\ nombre\\ del\\ algoritmo\\ indica\\ la\\ politica\\ de\\ prioridad.\\ La\\ prioridad\\ es\\ inversamente\\ proporcional\\ al\\ tiempo\\ que\\ falta\\ para\\ el\\ deadline.\\ El\\ deadline\\ de\\ cada\\ tarea\\ es\\ igual\\ al\\ periodo\\ de\\ la\\ misma. \\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Supuestos}Todas\\ las\\ tareas\\ criticas\\ son\\ periodicas,\\ e\\ independientes.\\ El\\ tiempo\\ de\\ computacion\\ se\\ conoce\\ a\\ priori,\\ y\\ el\\ cambio\\ de\\ contexto\\ es\\ igual\\ a\\ cero,\\ o\\ ya\\ esta\\ considerado\\ en\\ el\\ tiempo\\ de\\ computacion.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas\\ de\\ Scheduling}\\\\\\\\\\begin{theorem}[Parametros\\ a\\ tomar\\ en\\ cuenta]$\\\\\\\\\\mu = \\\\\\\\\\Sigma \\\\\\\\\\frac{Ci}{Pi}$ Utilizacion\\ del\\ CPU\\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Condiciones\\ de\\ suficiencia]$\\\\\\\\\\mu \\\\\\\\\\leq 1 $ Tareas\\ calendarizables,\\ ya\\ que\\ es\\ una\\ condicion\\ necesario\\ y\\ de\\ suficiencia\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Least Laxity First}\\\\\\\\\\begin{block}{Tipo y manejo dr prioridades}Es un algoritmo donde las prioridades se manejan de forma dinamica. Por cada unidad de tiempo se deben de evaluar las prioridades para asi conocer el Laxity de cada tarea, y tomar una decision sobre el scheduling de las mismas.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas de Scheduling}\\\\\\\\\\begin{theorem}[Parametros a tomar en cuenta]$ Laxity = Di - Ti + Ci $ Donde D es el deadline proximo de la tarea, T es el tiempo actual de ejecucion y C es el tiempo computacional faltante de la tarea \\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Criterio de Scheduling]Se toma el L menor entre todas las tareas en la cola de Ready y se ejecuta la tarea para cada tiempo de ejecucion\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame} >> ./docs/simulacion.tex";
	printf("%s\n",cmd);

	system(cmd);
}
void latex_rm_edf_lff(){
	char cmd[MAX_CMM_LEN] = "echo \\\\\\\\\\documentclass{beamer}\\\\\\\\\\mode'<'presentation'>' {\\\\\\\\\\usetheme{Madrid}}\\\\\\\\\\usepackage{booktabs,adjustbox}\\\\\\\\\\usepackage{xcolor,colortbl}\\\\\\\\\\title[RM, EDF, LLF]{Proyecto\\ 2:\\ Calendarizacion\\ en\\ Tiempo\\ Real}\\\\\\\\\\author{Vargas A,\\ Camacho A,\\ Morales V}\\\\\\\\\\institute[TEC]{Tecnologico\\ de\\ Costa Rica \\\\\\\\\\medskip\\\\\\\\\\textit{avargas@gmail.com,\\ acamacho@gmail.com,\\ verny.morales@gmail.com}\\\\\\\\\\textit{3er Cuatrimestre}}\\\\\\\\\\date{\\\\\\\\\\today}\\\\\\\\\\begin{document}\\\\\\\\\\begin{frame}\\\\\\\\\\titlepage\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Rate\\ Monotonic}\\\\\\\\\\begin{block}{Tipo}Algoritmo\\ de\\ Scheduling \\Dinamico,\\ utilizado\\ para\\ la\\ reslucion\\ de\\ problemas\\ caoticos,\\ como\\ por\\ ejemplo\\ el\\ problema\\ de\\ los\\ carros\\ autonomos.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Manejo\\ de\\ prioridades}Algoritmo\\ de\\ prioridades\\ estaticas,\\ esto\\ quiere\\ decir\\ que\\ ninguna\\ tarea\\ puede\\ cambiar\\ su\\ prioridad.\\ Donde\\ la\\ prioridad\\ de\\ una\\ tarea\\ siempre\\ es\\ igual\\ a\\ su\\ periodo.\\ Periodo\\ mas\\ corto,\\ mayor\\ la\\ prioridad.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Supuestos}Todas\\ las\\ tareas\\ criticas\\ son\\ periodicas,\\ e\\ independientes.\\ El\\ tiempo\\ de\\ computacion\\ se\\ conoce\\ a\\ priori,\\ y\\ el\\ cambio\\ de\\ contexto\\ es\\ igual\\ a\\ cero,\\ o\\ ya\\ esta\\ considerado\\ en\\ el\\ tiempo\\ de\\ computacion.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas\\ de\\ Scheduling}\\\\\\\\\\begin{theorem}[Parametros\\ a\\ tomar\\ en\\ cuenta]$\\\\\\\\\\mu = \\\\\\\\\\Sigma \\\\\\\\\\frac{Ci}{Pi}$ Utilizacion\\ del\\ CPU\\$\\ 'Un' = n \'2''^'\\\\\\\\\\frac{'1'}{n} - '1' $ donde\\ n\\ es\\ la\\ cantidad\\ de\\ tareas\\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Condiciones\\ de\\ suficiencia]$\\\\\\\\\\mu \\\\\\\\\\leq Un $ Tareas\\ calendarizables\\ $\\\\\\\\\\mu \\\\\\\\\\geq Un$ Debido\\ a\\ que\\ es\\ una\\ condicion\\ de\\ sufuciencia\\ podria\\ ser\\ calendarizable\\$\\\\\\\\\\mu \\\\\\\\\\succ '1'$ Tareas\\ no\\ calendarizables\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Early\\ Deadline\\ First}\\\\\\\\\\begin{block}{Tipo}Algoritmo\\ de\\ Scheduling\\ Dinamico,\\ en\\ donde\\ las\\ tareas\\ son\\ periodicas.\\ Se\\ considera\\ un\\ algoritmo\\ optimo\\ para\\ algoritmos\\ de\\ prioridades\\ dinamicas.\\ Es\\ un\\ algoritmo\\ de\\ tipo\\ expropiativo.\\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Manejo\\ de\\ prioridades}El\\ nombre\\ del\\ algoritmo\\ indica\\ la\\ politica\\ de\\ prioridad.\\ La\\ prioridad\\ es\\ inversamente\\ proporcional\\ al\\ tiempo\\ que\\ falta\\ para\\ el\\ deadline.\\ El\\ deadline\\ de\\ cada\\ tarea\\ es\\ igual\\ al\\ periodo\\ de\\ la\\ misma. \\\\\\\\\\end{block}\\\\\\\\\\begin{block}{Supuestos}Todas\\ las\\ tareas\\ criticas\\ son\\ periodicas,\\ e\\ independientes.\\ El\\ tiempo\\ de\\ computacion\\ se\\ conoce\\ a\\ priori,\\ y\\ el\\ cambio\\ de\\ contexto\\ es\\ igual\\ a\\ cero,\\ o\\ ya\\ esta\\ considerado\\ en\\ el\\ tiempo\\ de\\ computacion.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas\\ de\\ Scheduling}\\\\\\\\\\begin{theorem}[Parametros\\ a\\ tomar\\ en\\ cuenta]$\\\\\\\\\\mu = \\\\\\\\\\Sigma \\\\\\\\\\frac{Ci}{Pi}$ Utilizacion\\ del\\ CPU\\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Condiciones\\ de\\ suficiencia]$\\\\\\\\\\mu \\\\\\\\\\leq 1 $ Tareas\\ calendarizables,\\ ya\\ que\\ es\\ una\\ condicion\\ necesario\\ y\\ de\\ suficiencia\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Least Laxity First}\\\\\\\\\\begin{block}{Tipo y manejo dr prioridades}Es un algoritmo donde las prioridades se manejan de forma dinamica. Por cada unidad de tiempo se deben de evaluar las prioridades para asi conocer el Laxity de cada tarea, y tomar una decision sobre el scheduling de las mismas.\\\\\\\\\\end{block}\\\\\\\\\\end{frame}\\\\\\\\\\begin{frame}\\\\\\\\\\frametitle{Teoremas de Scheduling}\\\\\\\\\\begin{theorem}[Parametros a tomar en cuenta]$ Laxity = Di - Ti + Ci $ Donde D es el deadline proximo de la tarea, T es el tiempo actual de ejecucion y C es el tiempo computacional faltante de la tarea \\\\\\\\\\end{theorem}\\\\\\\\\\begin{theorem}[Criterio de Scheduling]Se toma el L menor entre todas las tareas en la cola de Ready y se ejecuta la tarea para cada tiempo de ejecucion\\\\\\\\\\end{theorem}\\\\\\\\\\end{frame} >> ./docs/simulacion.tex";
	printf("%s\n",cmd);

	system(cmd);
}
