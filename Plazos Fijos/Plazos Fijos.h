#ifndef PLAZOS_FIJOS_H_INCLUDED
#define PLAZOS_FIJOS_H_INCLUDED

typedef struct
{
	int dia;
	int mes;
	int anio;
}
Fecha;


typedef struct
{
	int mes;
	int anio;
}
Mes;

typedef struct
{
	int nroPF;
	int cuenta;
	float monto;
	float interesAnual;
	Fecha fechaConstitucion;
	int dias;
}
PlazoFijo;


typedef struct
{
	int cuenta;
	char cliente[201];
	float saldo;
}
Cuenta;

#define ERROR_ARG 1
#define ERROR_ARCH 2
#define ERROR_FECHA 3
#define ERROR_MEMORIA 4
#define ERROR_LINEA 5


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TAM_VEC_MAX 10
#define TAM_LINEA 50
#define ANIO_BASE 1601

typedef int (*Cmp)(const void*, const void*);




int compararPF_ALU(const void* x,const void* y);
void ordenarVector_ALU(void* vec, int ce, size_t tamElem, Cmp cmp);
void* buscarMenor_ALU(void* ini,void* fin,size_t tamElem,Cmp cmp);
void intercambiar_ALU(void* x, void* y, size_t tamElem);


void generarArchivos_ALU(const char* nombreCuentas, const char* nombrePlazosFijos);
void mostrarCuentas_ALU(const char* nombreArchCuentas);

PlazoFijo* cargarVectorPF_ALU(const char* nombrePlazosFijos, int* ce);
void txtVarABin(char* linea,PlazoFijo* plazo);
void procesarPF_ALU(FILE* cuentasDat, PlazoFijo* plazosFijosVec, int ce, Fecha* fechaProceso);
bool insertarPFEnVectorOrd_ALU(PlazoFijo* plazosFijosVec, int* ce, PlazoFijo* plazoFijo);
PlazoFijo* buscarPosIns_ALU(PlazoFijo* plazosFijosVec, int ce, PlazoFijo* plazoFijo);



bool esFechaValida_ALU(const Fecha* f);
int cantDiasMes_ALU(int mes,int anio);
Fecha sumarDiasAFecha_ALU(const Fecha* f, unsigned dias);
void sumarDiasAFechaInSitu_ALU(Fecha* f, unsigned dias);
int cmpFechas_ALU(const Fecha* f1, const Fecha* f2);
int txtAFecha_ALU(const char* txt, Fecha* fecha);

#endif // PLAZOS_FIJOS_H_INCLUDED
