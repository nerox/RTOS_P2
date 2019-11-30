
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
		RM_schedulability_calculation();
		RM();
		latex_table_slide_header("RM");
		latex_table_slide("RM");
		end_latex_slide();
		break;
	case 2:
		printf("User selected to execute EDF\n");
		printf("Performing the EDF simulation \n");
		latex_edf();
		EDF_schedulability_calculation();
		EDF();
		latex_table_slide_header("EDF");
		latex_table_slide("EDF");
		end_latex_slide();
		break;
	case 3:
		printf("User selected to execute LLF\n");
		printf("Performing the LLF simulation \n");
		latex_llf();
		LLF_schedulability_calculation();
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
			RM_schedulability_calculation();
			EDF_schedulability_calculation();
			latex_table_slide_header("All");
			latex_table_slide("RM");
		}
		else{
			RM_schedulability_calculation();
			latex_table_slide_header("RM");
			latex_table_slide("RM");
			end_latex_slide();
			EDF_schedulability_calculation();
		}
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
			RM_schedulability_calculation();
			LLF_schedulability_calculation();
			latex_table_slide_header("All");
			latex_table_slide("RM");
		}
		else{
			RM_schedulability_calculation();
			latex_table_slide_header("RM");
			latex_table_slide("RM");
			end_latex_slide();
			LLF_schedulability_calculation();
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
			EDF_schedulability_calculation();
			LLF_schedulability_calculation();
			latex_table_slide_header("All");
			latex_table_slide("EDF");
		}
		else{
			EDF_schedulability_calculation();
			latex_table_slide_header("EDF");
			latex_table_slide("EDF");
			end_latex_slide();
			LLF_schedulability_calculation();
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
			RM_schedulability_calculation();
			EDF_schedulability_calculation();
			LLF_schedulability_calculation();
			latex_table_slide_header("All");
			latex_table_slide("RM");
		}
		else{
			RM_schedulability_calculation();
			latex_table_slide_header("RM");
			latex_table_slide("RM");
			end_latex_slide();
			EDF_schedulability_calculation();
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
			LLF_schedulability_calculation();
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
		latex_EDF_evaluation( 1,  u_value);
	}
	else{
		printf("Dear user, the selected values are not canditates for EDF scheduling. Be careful thousands can die today!!!!. Perhaps you should reconsider other values, the following simulation shows the schedulability testing\n");
		latex_EDF_evaluation( 2,  u_value);
	}
}

void LLF_schedulability_calculation(){
	double u_value=summation_EDF_RM();
	if (u_value<=1){
		printf("Dear user, the selected values are canditates for LLF scheduling, you can take a deep breath no one will die today, the following simulation shows the schedulability testing\n");
		latex_LLF_evaluation( 1,  u_value);
	}
	else{
		printf("Dear user, the selected values are not canditates for LLF scheduling. Be careful thousands can die today!!!!. Perhaps you should reconsider other values, the following simulation shows the schedulability testing\n");
		latex_LLF_evaluation( 2,  u_value);
	}
}
void RM_schedulability_calculation(){
	double u_value=summation_EDF_RM();
	double u_n_value=u_n_calculation();
	if (u_value>1){
		printf("Dear user, the selected values are not canditates for RM scheduling. Perhaps you should reconsider other values; the future of endangered species might be on you hands. The following simulation shows the schedulability testing\n");
		latex_rm_evaluation(3, u_value,1);
	}
	else if(u_value<1 && u_value>=u_n_value){
		printf("Dear user, the selected values MIGHT be canditates for RM scheduling. Perhaps you should reconsider other values for better certainty; people could potentially die. The following simulation shows the schedulability testing\n");
		latex_rm_evaluation(2, u_value,u_n_value);
	}
	else{
		printf("Dear user, the selected values are canditates for RM scheduling, be grateful 0 casualties will be driven by this selection, the following simulation shows the schedulability testing\n");
		latex_rm_evaluation(1, u_value,u_n_value);
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
	fprintf(out,"%s"," Resultados de Simulador}");
	fclose(out);
}
void end_latex_slide(){
	FILE *out = fopen("./docs/simulacion.tex", "a");
	fprintf(out, "%s","\\end{frame}");
	fclose(out);
}
void latex_table_slide(char algorithm []){
	int crashPosition=AMOUNT_OF_PERIODS+1;
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
		for (j=1;j<=AMOUNT_OF_PERIODS;j++){
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
				crashPosition=j-1;
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
	int crash=0;
	printf("%d Crash!!!!!\n",crashPosition);
	for (j=0;j<AMOUNT_OF_PERIODS+1;j++){
			int COLOR=-1;
			if (crashPosition!=j){
						
				for (i=0;i<NUMBER_OF_TASKS;i++){
					
					if(j%TASKS_TO_SCHEDULE[i].period_time==0 && crash!=1){
						COLOR=TASKS_TO_SCHEDULE[i].id;
						
					}
				}
			}
			else{
				crash=1;
				COLOR=6;
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
	// system("cd docs");
	// system("ls");
	// system("cd docs");
	system("pdflatex ./docs/simulacion.tex");
	system("mv *.pdf ./docs");
	system("rm simulacion.*");
	system("xdg-open ./docs/simulacion.pdf");
	// system("cd ..");
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
	char intstr[200]="";
	FILE *out = fopen("./docs/simulacion.tex", "a");  
	char cmd[MAX_CMM_LEN] = 
"\\documentclass{beamer}\\mode<presentation> {\\usetheme{Madrid}}\\usepackage{booktabs,adjustbox}\\usepackage{xcolor,colortbl}\\title[RM, EDF, LLF]{Proyecto 2: Calendarizaci\\'on en Tiempo Real}\\author{Vargas A, Camacho A, Morales V}\\institute[TEC]{Tecnol\\'ogico de Costa Rica \\\\ \n \\medskip\\textit{avargas@gmail.com, acamacho@gmail.com, verny.morales@gmail.com}\\\\ \n \\textit{3er Cuatrimestre}}\\date{\\today}\\begin{document}\\begin{frame}\\titlepage\\end{frame}\\begin{frame}\\frametitle{Rate Monotonic}\\begin{block}{Tipo}Algoritmo de Scheduling Din\\'amico, utilizado para la resoluci\\'on de problemas ca\\'oticos, como por ejemplo el problema de los carros aut\\'onomos.\\end{block}\\begin{block}{Manejo de prioridades}Algoritmo de prioridades est\\'aticas, esto quiere decir que ninguna tarea puede cambiar su prioridad. Donde la prioridad de una tarea siempre es igual a su per\\'iodo. Per\\'iodo mas corto, mayor la prioridad.\\end{block}\\begin{block}{Supuestos}Todas las tareas cr\\'iticas son peri\\'odicas, e independientes. El tiempo de computaci\\'on se conoce a priori, y el cambio de contexto es igual a cero, o ya esta considerado en el tiempo de computaci\\'on.\\end{block}\\end{frame}";
	printf("%s\n",cmd);

	fprintf(out, "%s",cmd);
	fclose(out); 
	//system(cmd);
}
void latex_edf(){
	char intstr[200]="";
	FILE *out = fopen("./docs/simulacion.tex", "a");  
	char cmd[MAX_CMM_LEN] = "\\documentclass{beamer}\\mode<presentation> {\\usetheme{Madrid}}\\usepackage{booktabs,adjustbox}\\usepackage{xcolor,colortbl}\\title[RM, EDF, LLF]{Proyecto 2: Calendarizaci\\'on en Tiempo Real} \\author{Vargas A, Camacho A, Morales V} \\institute[TEC] {Tecnol\\'ogico de Costa Rica \\medskip\\\\ \n \\textit{avargas@gmail.com, acamacho@gmail.com, verny.morales@gmail.com}\\\\ \n \\textit{3er Cuatrimestre}}\\date{\\today} \\begin{document}\\begin{frame}\\titlepage\\end{frame}\\begin{frame}\\frametitle{Early Deadline First}\\begin{block}{Tipo}Algoritmo de Scheduling Din\\'amico, en donde las tareas son peri\\'odicas. Se considera un algoritmo \\'optimo para algoritmos de prioridades din\\'amicas. Es un algoritmo de tipo expropiativo.\\end{block}\\begin{block}{Manejo de prioridades}El nombre del algoritmo indica la pol\\'itica de prioridad. La prioridad es inversamente proporcional al tiempo que falta para el deadline. El deadline de cada tarea es igual al per\\'iodo de la misma. \\end{block}\\begin{block}{Supuestos}Todas las tareas cr\\'iticas son peri\\'odicas, e independientes. El tiempo de computaci\\'on se conoce a priori, y el cambio de contexto es igual a cero, o ya esta considerado en el tiempo de computaci\\'on.\\end{block}\\end{frame}";
	printf("%s\n",cmd);

	fprintf(out, "%s",cmd);
	fclose(out); 
}

void latex_llf(){
	char intstr[200]="";
	FILE *out = fopen("./docs/simulacion.tex", "a"); 
	char cmd[MAX_CMM_LEN] = "\\documentclass{beamer}\\mode<presentation> {\\usetheme{Madrid}}\\usepackage{booktabs,adjustbox}\\usepackage{xcolor,colortbl}\\title[RM, EDF, LLF]{Proyecto 2: Calendarizaci\\'on en Tiempo Real} \\author{Vargas A, Camacho A, Morales V} \\institute[TEC] {Tecnol\\'ogico de Costa Rica\\\\ \n \\medskip\\textit{avargas@gmail.com, acamacho@gmail.com, verny.morales@gmail.com} \\\\ \n \\textit{3er Cuatrimestre}}\\date{\\today} \\begin{document}\\begin{frame}\\titlepage\\end{frame}\\begin{frame}\\frametitle{Least Laxity First}\\begin{block}{Tipo y manejo de prioridades}Es un algoritmo donde las prioridades se manejan de forma din\\'amica. Por cada unidad de tiempo se deben de evaluar las prioridades para as\\'i conocer el Laxity de cada tarea, y tomar una decisi\\'on sobre el scheduling de las mismas.\\end{block}\\end{frame}";
	printf("%s\n",cmd);

	fprintf(out, "%s",cmd);
	fclose(out); 
}
void latex_rm_edf(){
	char intstr[200]="";
	FILE *out = fopen("./docs/simulacion.tex", "a"); 
	char cmd[MAX_CMM_LEN] = "\\documentclass{beamer}\\mode<presentation> {\\usetheme{Madrid}}\\usepackage{booktabs,adjustbox}\\usepackage{xcolor,colortbl}\\title[RM, EDF, LLF]{Proyecto 2: Calendarizaci\\'on en Tiempo Real}\\author{Vargas A, Camacho A, Morales V}\\institute[TEC]{Tecnol\\'ogico de Costa Rica \\\\ \n \\medskip\\textit{avargas@gmail.com, acamacho@gmail.com, verny.morales@gmail.com}\\\\ \n \\textit{3er Cuatrimestre}}\\date{\\today}\\begin{document}\\begin{frame}\\titlepage\\end{frame}\\begin{frame}\\frametitle{Rate Monotonic}\\begin{block}{Tipo}Algoritmo de Scheduling Din\\'amico, utilizado para la resoluci\\'on de problemas ca\\'oticos, como por ejemplo el problema de los carros aut\\'onomos.\\end{block}\\begin{block}{Manejo de prioridades}Algoritmo de prioridades est\\'aticas, esto quiere decir que ninguna tarea puede cambiar su prioridad. Donde la prioridad de una tarea siempre es igual a su per\\'iodo. Per\\'iodo mas corto, mayor la prioridad.\\end{block}\\begin{block}{Supuestos}Todas las tareas cr\\'iticas son peri\\'odicas, e independientes. El tiempo de computaci\\'on se conoce a priori, y el cambio de contexto es igual a cero, o ya esta considerado en el tiempo de computaci\\'on.\\end{block}\\end{frame}\\begin{frame}\\frametitle{Teoremas de Scheduling}\\begin{theorem}[Par\\'ametros a tomar en cuenta]$\\mu = \\Sigma \\frac{C_{i}}{P_{i}}$ Utilizaci\\'on del CPU $ U(n) = n (2^\\frac{1}{n} - 1) $ donde n es la cantidad de tareas\\end{theorem}\\begin{theorem}[Condiciones de suficiencia]$\\mu \\leq Un $ Tareas calendarizables $\\mu \\geq Un$ Debido a que es una condici\\'on de suficiencia podr\\'ia ser calendarizable $\\mu > 1$ Tareas no calendarizables\\end{theorem}\\end{frame}\\begin{frame}\\frametitle{Early Deadline First}\\begin{block}{Tipo}Algoritmo de Scheduling Din\\'amico, en donde las tareas son peri\\'odicas. Se considera un algoritmo \\'optimo para algoritmos de prioridades din\\'amicas. Es un algoritmo de tipo expropiativo.\\end{block}\\begin{block}{Manejo de prioridades}El nombre del algoritmo indica la pol\\'itica de prioridad. La prioridad es inversamente proporcional al tiempo que falta para el deadline. El deadline de cada tarea es igual al per\\'iodo de la misma. \\end{block}\\begin{block}{Supuestos}Todas las tareas cr\\'iticas son peri\\'odicas, e independientes. El tiempo de computaci\\'on se conoce a priori, y el cambio de contexto es igual a cero, o ya esta considerado en el tiempo de computaci\\'on.\\end{block}\\end{frame}\\begin{frame}\\frametitle{Teoremas de Scheduling}\\begin{theorem}[Par\\'ametros a tomar en cuenta]$\\mu = \\Sigma \\frac{C_{i}}{P_{i}}$ Utilizaci\\'on del CPU\\end{theorem}\\begin{theorem}[Condiciones de suficiencia]$\\mu \\leq 1 $ Tareas calendarizables, ya que es una condici\\'on necesario y de suficiencia\\end{theorem}\\end{frame}";
	printf("%s\n",cmd);

	fprintf(out, "%s",cmd);
	fclose(out); 
}
void latex_rm_lff(){
	char intstr[200]="";
	FILE *out = fopen("./docs/simulacion.tex", "a"); 
	char cmd[MAX_CMM_LEN] = "\\documentclass{beamer}\\mode<presentation> {\\usetheme{Madrid}}\\usepackage{booktabs,adjustbox}\\usepackage{xcolor,colortbl}\\title[RM, EDF, LLF]{Proyecto 2: Calendarizaci\\'on en Tiempo Real}\\author{Vargas A, Camacho A, Morales V}\\institute[TEC]{Tecnol\\'ogico de Costa Rica\\\\ \n \\medskip\\textit{avargas@gmail.com, acamacho@gmail.com, verny.morales@gmail.com}\\\\ \n \\textit{3er Cuatrimestre}}\\date{\\today}\\begin{document}\\begin{frame}\\titlepage\\end{frame}\\begin{frame}\\frametitle{Rate Monotonic}\\begin{block}{Tipo}Algoritmo de Scheduling Din\\'amico, utilizado para la resoluci\\'on de problemas ca\\'oticos, como por ejemplo el problema de los carros aut\\'onomos.\\end{block}\\begin{block}{Manejo de prioridades}Algoritmo de prioridades est\'aticas, esto quiere decir que ninguna tarea puede cambiar su prioridad. Donde la prioridad de una tarea siempre es igual a su per\\'iodo. Per\\'iodo mas corto, mayor la prioridad.\\end{block}\\begin{block}{Supuestos}Todas las tareas cr\\'iticas son peri\\'odicas, e independientes. El tiempo de computaci\\'on se conoce a priori, y el cambio de contexto es igual a cero, o ya esta considerado en el tiempo de computaci\\'on.\\end{block}\\end{frame}\\begin{frame}\\frametitle{Teoremas de Scheduling}\\begin{theorem}[Par\\'ametros a tomar en cuenta]$\\mu = \\Sigma \\frac{C_{i}}{P_{i}}$ Utilizaci\\'on del CPU $ U(n) = n (2^\\frac{1}{n} - 1) $ donde n es la cantidad de tareas\\end{theorem}\\begin{theorem}[Condiciones de suficiencia]$\\mu \\leq Un $ Tareas calendarizables $\\mu \\geq Un$ Debido a que es una condici\\'on de suficiencia podr\\'ia ser calendarizable $\\mu > 1$ Tareas no calendarizables\\end{theorem}\\end{frame}\\begin{frame}\\frametitle{Least Laxity First}\\begin{block}{Tipo y manejo de prioridades}Es un algoritmo donde las prioridades se manejan de forma din\\'amica. Por cada unidad de tiempo se deben de evaluar las prioridades para as\\'i conocer el Laxity de cada tarea, y tomar una decisi\\'on sobre el scheduling de las mismas.\\end{block}\\end{frame}\\begin{frame}\\frametitle{Teoremas de Scheduling}\\begin{theorem}[Par\\'ametros a tomar en cuenta]$ Laxity = D_{i} - T_{i} + C_{i} $ Donde D es el deadline pr\\'oximo de la tarea, T es el tiempo actual de ejecuci\\'on y C es el tiempo computaci\\'on al faltante de la tarea \\end{theorem}\\begin{theorem}[Criterio de Scheduling]Se toma el $ L_{i} $ menor entre todas las tareas en la cola de Ready y se ejecuta la tarea para cada tiempo de ejecuci\\'on\\end{theorem}\\end{frame}";
	printf("%s\n",cmd);

	fprintf(out, "%s",cmd);
	fclose(out); 
}
void latex_edf_lff(){
	char intstr[200]="";
	FILE *out = fopen("./docs/simulacion.tex", "a"); 
	char cmd[MAX_CMM_LEN] = "\\documentclass{beamer}\\mode<presentation> {\\usetheme{Madrid}}\\usepackage{booktabs,adjustbox}\\usepackage{xcolor,colortbl}\\title[RM, EDF, LLF]{Proyecto 2: Calendarizaci\\'on en Tiempo Real} \\author{Vargas A, Camacho A, Morales V} \\institute[TEC] {Tecnol\\'ogico de Costa Rica\\\\ \n \\medskip\\textit{avargas@gmail.com, acamacho@gmail.com, verny.morales@gmail.com}\\\\ \n \\textit{3er Cuatrimestre}}\\date{\\today} \\begin{document}\\begin{frame}\\titlepage\\end{frame}\\begin{frame}\\frametitle{Early Deadline First}\\begin{block}{Tipo}Algoritmo de Scheduling Din\\'amico, en donde las tareas son peri\\'odicas. Se considera un algoritmo \\'optimo para algoritmos de prioridades din\\'amicas. Es un algoritmo de tipo expropiativo.\\end{block}\\begin{block}{Manejo de prioridades}El nombre del algoritmo indica la pol\\'itica de prioridad. La prioridad es inversamente proporcional al tiempo que falta para el deadline. El deadline de cada tarea es igual al per\\'iodo de la misma. \\end{block}\\begin{block}{Supuestos}Todas las tareas cr\\'iticas son peri\\'odicas, e independientes. El tiempo de computaci\\'on se conoce a priori, y el cambio de contexto es igual a cero, o ya esta considerado en el tiempo de computaci\\'on.\\end{block}\\end{frame}\\begin{frame}\\frametitle{Teoremas de Scheduling}\\begin{theorem}[Par\\'ametros a tomar en cuenta]$\\mu = \\Sigma \\frac{C_{i}}{P_{i}}$ Utilizaci\\'on del CPU\\end{theorem}\\begin{theorem}[Condiciones de suficiencia]$\\mu \\leq 1 $ Tareas calendarizables, ya que es una condici\\'on necesario y de suficiencia\\end{theorem}\\end{frame}\\begin{frame}\\frametitle{Least Laxity First}\\begin{block}{Tipo y manejo de prioridades}Es un algoritmo donde las prioridades se manejan de forma din\\'amica. Por cada unidad de tiempo se deben de evaluar las prioridades para as\\'i conocer el Laxity de cada tarea, y tomar una decisi\\'on sobre el scheduling de las mismas.\\end{block}\\end{frame}\\begin{frame}\\frametitle{Teoremas de Scheduling}\\begin{theorem}[Par\\'ametros a tomar en cuenta]$ Laxity = D_{i} - T_{i} + C_{i} $ Donde D es el deadline pr\\'oximo de la tarea, T es el tiempo actual de ejecuci\\'on y C es el tiempo computaci\\'on al faltante de la tarea \\end{theorem}\\begin{theorem}[Criterio de Scheduling]Se toma el $ L_{i} $ menor entre todas las tareas en la cola de Ready y se ejecuta la tarea para cada tiempo de ejecuci\\'on\\end{theorem}\\end{frame}";
	printf("%s\n",cmd);

	fprintf(out, "%s",cmd);
	fclose(out); 
}
void latex_rm_edf_lff(){
	char intstr[200]="";
	FILE *out = fopen("./docs/simulacion.tex", "a"); 
	char cmd[MAX_CMM_LEN] = "\\documentclass{beamer}\\mode<presentation> {\\usetheme{Madrid}}\\usepackage{booktabs,adjustbox}\\usepackage{xcolor,colortbl}\\title[RM, EDF, LLF]{Proyecto 2: Calendarizaci\\'on en Tiempo Real}\\author{Vargas A, Camacho A, Morales V}\\institute[TEC]{Tecnol\\'ogico de Costa Rica \\\\ \n \\medskip\\textit{avargas@gmail.com, acamacho@gmail.com, verny.morales@gmail.com}\\\\ \n \\textit{3er Cuatrimestre}}\\date{\\today}\\begin{document}\\begin{frame}\\titlepage\\end{frame}\\begin{frame}\\frametitle{Rate Monotonic}\\begin{block}{Tipo}Algoritmo de Scheduling Din\\'amico, utilizado para la resoluci\\'on de problemas ca\\'oticos, como por ejemplo el problema de los carros aut\\'onomos.\\end{block}\\begin{block}{Manejo de prioridades}Algoritmo de prioridades est\\'aticas, esto quiere decir que ninguna tarea puede cambiar su prioridad. Donde la prioridad de una tarea siempre es igual a su per\\'iodo. Per\\'iodo mas corto, mayor la prioridad.\\end{block}\\begin{block}{Supuestos}Todas las tareas cr\\'iticas son peri\\'odicas, e independientes. El tiempo de computaci\\'on se conoce a priori, y el cambio de contexto es igual a cero, o ya esta considerado en el tiempo de computaci\\'on.\\end{block}\\end{frame}\\begin{frame}\\frametitle{Teoremas de Scheduling}\\begin{theorem}[Par\\'ametros a tomar en cuenta]$\\mu = \\Sigma \\frac{C_{i}}{P_{i}}$ Utilizaci\\'on del CPU $ U(n) = n(2^\\frac{1}{n} - 1) $ donde n es la cantidad de tareas\\end{theorem}\\begin{theorem}[Condiciones de suficiencia]$\\mu \\leq U(n) $ Tareas calendarizables $\\mu > U(n)$ Debido a que es una condici\\'on de suficiencia podr\\'ia ser calendarizable $\\mu > 1$ Tareas no calendarizables\\end{theorem}\\end{frame}\\begin{frame}\\frametitle{Early Deadline First}\\begin{block}{Tipo}Algoritmo de Scheduling Din\\'amico, en donde las tareas son peri\\'odicas. Se considera un algoritmo \\'optimo para algoritmos de prioridades din\\'amicas. Es un algoritmo de tipo expropiativo.\\end{block}\\begin{block}{Manejo de prioridades}El nombre del algoritmo indica la pol\\'itica de prioridad. La prioridad es inversamente proporcional al tiempo que falta para el deadline. El deadline de cada tarea es igual al per\\'iodo de la misma. \\end{block}\\begin{block}{Supuestos}Todas las tareas cr\\'iticas son peri\\'odicas, e independientes. El tiempo de computaci\\'on se conoce a priori, y el cambio de contexto es igual a cero, o ya esta considerado en el tiempo de computaci\\'on.\\end{block}\\end{frame}\\begin{frame}\\frametitle{Teoremas de Scheduling}\\begin{theorem}[Par\\'ametros a tomar en cuenta]$\\mu = \\Sigma \\frac{C_{i}}{P_{i}}$ Utilizaci\\'on del CPU\\end{theorem}\\begin{theorem}[Condiciones de suficiencia]$\\mu \\leq 1 $ Tareas calendarizables, ya que es una condici\\'on necesario y de suficiencia\\end{theorem}\\end{frame}\\begin{frame}\\frametitle{Least Laxity First}\\begin{block}{Tipo y manejo de prioridades}Es un algoritmo donde las prioridades se manejan de forma din\\'amica. Por cada unidad de tiempo se deben de evaluar las prioridades para as\\'i conocer el Laxity de cada tarea, y tomar una decisi\\'on sobre el scheduling de las mismas.\\end{block}\\end{frame}\\begin{frame}\\frametitle{Teoremas de Scheduling}\\begin{theorem}[Par\\'ametros a tomar en cuenta]$ Laxity = D_{i} - T_{i} + C_{i} $ Donde D es el deadline pr\\'oximo de la tarea, T es el tiempo actual de ejecuci\'on y C es el tiempo computaci\\'on al faltante de la tarea \\end{theorem}\\begin{theorem}[Criterio de Scheduling]Se toma el $ L_{i} $ menor entre todas las tareas en la cola de Ready y se ejecuta la tarea para cada tiempo de ejecuci\\'on\\end{theorem}\\end{frame}";
	printf("%s\n",cmd);

	fprintf(out, "%s",cmd);
	fclose(out); 
}

void latex_rm_evaluation(int result, float mu,float u){
	char intstr[200]="";
	FILE *out = fopen("./docs/simulacion.tex", "a");  
	fprintf(out, "%s", "\\begin{frame}\\frametitle{");
	fprintf(out,"%s","RM Prueba de Calendarizabilidad}");
	if(result ==1){
	sprintf(intstr,  "\\begin{block}{Prueba de Schedulability} $\\mu=%f \\leq U(n)=%f$\\end{block}",mu,u);
	fprintf(out, "%s",intstr);	
	fprintf(out, "%s", "Las pruebas son calendarizables");	
	}
	else if (result==2){
		sprintf(intstr, "\\begin{block}{Prueba de Schedulability} $U(n)=%f \\leq  \\mu=%f$ \\end{block}",mu,u);
		fprintf(out, "%s",intstr);		
		fprintf(out, "%s", "Las pruebas podrían ser calendarizables, se recomienda simular extensamente");	
	}
	else{
		sprintf(intstr, "\\begin{block}{Prueba de Schedulability} $\\mu=%f \\leq 1 $ \\end{block}",mu);	
		fprintf(out, "%s",intstr);	
		fprintf(out, "%s", "Las pruebas NO son calendarizables");	
	}	
	fprintf(out, "%s","\\end{frame}");
	fclose(out);   
}


void latex_EDF_evaluation(int result, float mu){
	char intstr[200]="";
	FILE *out = fopen("./docs/simulacion.tex", "a");  
	fprintf(out, "%s", "\\begin{frame}\\frametitle{");
	fprintf(out,"%s","EDF Prueba de Calendarizabilidad}");
	if(result ==1){
	sprintf(intstr,  "\\begin{block}{Prueba de Schedulability} $\\mu=%f \\leq 1 $\\end{block}",mu);
	fprintf(out, "%s",intstr);	
	fprintf(out, "%s", "Las pruebas son calendarizables");	
	}
	else{
		sprintf(intstr, "\\begin{block}{Prueba de Schedulability} $\\mu=%f \\leq 1 $ \\end{block}",mu);	
		fprintf(out, "%s",intstr);	
		fprintf(out, "%s", "Las pruebas NO son calendarizables");	
	}	
	fprintf(out, "%s","\\end{frame}");
	fclose(out);   
}

void latex_LLF_evaluation(int result, float mu){
	char intstr[200]="";
	FILE *out = fopen("./docs/simulacion.tex", "a");  
	fprintf(out, "%s", "\\begin{frame}\\frametitle{");
	fprintf(out,"%s","LLF Prueba de Calendarizabilidad}");
	if(result ==1){
	sprintf(intstr,  "\\begin{block}{Prueba de Schedulability} $\\mu=%f \\leq 1 $\\end{block}",mu);
	fprintf(out, "%s",intstr);	
	fprintf(out, "%s", "Las pruebas son calendarizables");	
	}
	else{
		sprintf(intstr, "\\begin{block}{Prueba de Schedulability} $\\mu=%f \\leq 1 $ \\end{block}",mu);	
		fprintf(out, "%s",intstr);	
		fprintf(out, "%s", "Las pruebas NO son calendarizables");	
	}	
	fprintf(out, "%s","\\end{frame}");
	fclose(out);   
}
