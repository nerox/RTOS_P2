#REALIZA LOS DOS MAKEFILES

PATHLIBS = ./lib
PATHEJEC = ./src
Archivo = project2_scheduling_rtos


All: MakeLibs MakeEjec

MakeEjec:
	$(MAKE) -C $(PATHEJEC)

MakeLibs:
	$(MAKE) -C $(PATHLIBS)



clean:
	rm -rf ./lib/*.o ./lib/*.so $(Archivo)
