#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Plazos Fijos.h"

int main(int argc, char* argv[])
{
    if(argc != 4)
	{
		puts("Cantidad incorrecta de parámetros.");
		return ERROR_ARG;
	}
    generarArchivos_ALU(argv[1],argv[2]);

	puts("Cuentas antes de actualizar:");
	mostrarCuentas_ALU(argv[1]);

	FILE*cuentasDat=fopen(argv[1],"r+b");
	if(!cuentasDat)
		return ERROR_ARCH;

    Fecha fechaProceso;
    int fechaOk=txtAFecha_ALU(argv[3],&fechaProceso);
	if(!fechaOk)
		return ERROR_FECHA;

    int ce=0;
    PlazoFijo*vecPF=cargarVectorPF_ALU(argv[2],&ce);

    ordenarVector_ALU(vecPF,ce,sizeof(PlazoFijo),compararPF_ALU);

    procesarPF_ALU(cuentasDat,vecPF,ce,&fechaProceso);

	puts("\nCuentas despues de actualizar:");
	mostrarCuentas_ALU(argv[1]);
    free(vecPF);
    return 0;
}





///////////////////////////////////////////////////////////////////////////////////

