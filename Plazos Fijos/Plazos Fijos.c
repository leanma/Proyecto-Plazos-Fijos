#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Plazos Fijos.h"





int compararPF_ALU(const void*x,const void*y)
{
    PlazoFijo plazoX=*(PlazoFijo*)x;
    PlazoFijo plazoY=*(PlazoFijo*)y;
    if(plazoX.cuenta>plazoY.cuenta)
        return 1;
    if(plazoX.cuenta<plazoY.cuenta)
        return -1;
    return 0;
}


void ordenarVector_ALU(void* vec, int ce, size_t tamElem, Cmp cmp)
{
    void*m;
    void*ult=(char*)vec+(ce-1)*tamElem;
    for(char*i=vec;i<(char*)ult;i+=tamElem)
    {
        m=buscarMenor_ALU(i,ult,tamElem,cmp);
        if(i!=m)
        {
            intercambiar_ALU(i,m,tamElem);
        }
    }
}

void* buscarMenor_ALU(void* ini,void* fin,size_t tamElem,Cmp cmp)
{
    void*m=ini;
    for(char*j=(char*)ini+tamElem;j<=(char*)fin;j+=tamElem)
    {
        if(cmp(m,j)>0)
            m=j;
    }
    return m;
}

void intercambiar_ALU(void* x, void* y, size_t tamElem)
{
    void* xTemp=malloc(tamElem);
    memcpy(xTemp,x,tamElem);
    memcpy(x,y,tamElem);
    memcpy(y,xTemp,tamElem);
    free(xTemp);
}



void generarArchivos_ALU(const char* nombreCuentas, const char* nombrePlazosFijos)
{
    FILE* archCuentas=fopen(nombreCuentas,"wb");
    if(!archCuentas)
        exit(ERROR_ARCH);
    Cuenta vecCuentas[]={
                        {1000,"Perez, Juan",1200.00},
                        {2000,"Gonzalez, Pedro",2340.00},
                        {3000,"Alvarez, Agustin",5400.00},
                        {4000,"Beraldi, Bernardo",2540.00},
                        {5000,"Casares, Carlos",1984.00},
                        {6000,"Dalesandro, Daniel",12350.00},
                        {7000,"Estevez, Erica",15860.00},
                        };

    fwrite(vecCuentas,sizeof(Cuenta),7,archCuentas);
    fclose(archCuentas);

    FILE* archPF=fopen(nombrePlazosFijos,"wt");
    if(!archPF)
        exit(ERROR_ARCH);

    PlazoFijo vecPF[]={
                    {1,2000,2500.00,40.00,{5,3,2019},30},
                    {2,5000,2000.00,54.00,{24,5,2018},90},
                    {3,1000,3400.00,50.00,{3,1,2019},60},
                    {4,7000,10000.00,48.00,{12,4,2019},180},
                    {5,3000,5350.00,55.00,{16,6,2018},180},
                    {6,2000,6250.00,65.00,{12,5,2018},90},
                    };

    for(PlazoFijo*i=vecPF;i<vecPF+6;i++)
    {
        fprintf(archPF,"%d|%d|%.2f|%.2f|%d/%d/%d|%d\n",i->nroPF,i->cuenta,i->monto,i->interesAnual,i->fechaConstitucion.dia,i->fechaConstitucion.mes,i->fechaConstitucion.anio,i->dias);
    }
    fclose(archPF);
}




void mostrarCuentas_ALU(const char* nombreArchCuentas)
{
    FILE* arch=fopen(nombreArchCuentas,"rb");
    if(!arch)
        exit(ERROR_ARCH);

    Cuenta cuentaAct;
    fread(&cuentaAct,sizeof(Cuenta),1,arch);
    while(!feof(arch))
    {
        printf("%-8d%-25s%.2f\n",cuentaAct.cuenta,cuentaAct.cliente,cuentaAct.saldo);
        fread(&cuentaAct,sizeof(Cuenta),1,arch);
    }
    fclose(arch);
}


PlazoFijo* cargarVectorPF_ALU(const char* nombrePlazosFijos, int* ce)
{
    FILE* archPF=fopen(nombrePlazosFijos,"rt");
    if(!archPF)
        exit(ERROR_ARCH);

    PlazoFijo* vecPF=malloc(TAM_VEC_MAX*sizeof(PlazoFijo));
    if(!vecPF)
        exit(ERROR_MEMORIA);

    PlazoFijo* vecPF_ptr=vecPF;

    PlazoFijo auxPF;
    char linea[TAM_LINEA];
    fgets(linea,TAM_LINEA,archPF);

    while(!feof(archPF))
    {
        txtVarABin(linea,&auxPF);
        *vecPF_ptr=auxPF;
        vecPF_ptr++;
        (*ce)++;
        fgets(linea,TAM_LINEA,archPF);
    }


    fclose(archPF);
    return vecPF;
}


void txtVarABin(char* linea,PlazoFijo* plazo)
{
    char* act=strchr(linea,'\n');
    if(!act)
        exit(ERROR_LINEA);

    *act='\0';
    act=strrchr(linea,'|');
    sscanf(act+1,"%d",&plazo->dias);

    *act='\0';
    act=strrchr(linea,'|');
    sscanf(act+1,"%d/%d/%d",&plazo->fechaConstitucion.dia,&plazo->fechaConstitucion.mes,&plazo->fechaConstitucion.anio);

    *act='\0';
    act=strrchr(linea,'|');
    sscanf(act+1,"%f",&plazo->interesAnual);

    *act='\0';
    act=strrchr(linea,'|');
    sscanf(act+1,"%f",&plazo->monto);

    *act='\0';
    act=strrchr(linea,'|');
    sscanf(act+1,"%d",&plazo->cuenta);

    *act='\0';
    sscanf(linea,"%d",&plazo->nroPF);

}


void procesarPF_ALU(FILE* cuentasDat, PlazoFijo* plazosFijosVec, int ce, Fecha* fechaProceso)
{
    Cuenta cueAct;
    PlazoFijo* vecPF_ptr=plazosFijosVec;

    fread(&cueAct,sizeof(Cuenta),1,cuentasDat);
    while(!feof(cuentasDat) && vecPF_ptr<plazosFijosVec+ce)
    {
        int comp=cueAct.cuenta-vecPF_ptr->cuenta;
        if(comp==0)
        {
            Fecha fechaVen=sumarDiasAFecha_ALU(&vecPF_ptr->fechaConstitucion,vecPF_ptr->dias);
            while(cmpFechas_ALU(fechaProceso,&fechaVen)>0)
            {
                sumarDiasAFechaInSitu_ALU(&fechaVen,365);
                vecPF_ptr->monto+=((float)vecPF_ptr->dias/365*vecPF_ptr->interesAnual/100*vecPF_ptr->monto);
            }
            cueAct.saldo+=vecPF_ptr->monto;
            vecPF_ptr++;
        }
        if(comp<0)
        {
            fseek(cuentasDat,-(long)sizeof(Cuenta),SEEK_CUR);
            fwrite(&cueAct,sizeof(Cuenta),1,cuentasDat);
            fseek(cuentasDat,0,SEEK_CUR);
            fread(&cueAct,sizeof(Cuenta),1,cuentasDat);
        }
    }
    fclose(cuentasDat);
}


bool insertarPFEnVectorOrd_ALU(PlazoFijo* plazosFijosVec, int* ce, PlazoFijo* plazoFijo)
{
    PlazoFijo*pos=buscarPosIns_ALU(plazosFijosVec,*ce,plazoFijo);

    for(PlazoFijo* i=plazosFijosVec+*ce-1;i>=pos;i--)
    {
        *(i+1)=*i;
    }
    *pos=*plazoFijo;
    (*ce)++;
    return 1;
}


PlazoFijo* buscarPosIns_ALU(PlazoFijo* plazosFijosVec, int ce, PlazoFijo* plazoFijo)
{
    for(PlazoFijo*i=plazosFijosVec;i<plazosFijosVec+ce-1;i++)
    {
        if(i->cuenta<plazoFijo->cuenta && (i+1)->cuenta<=plazoFijo->cuenta)
            return i+1;
    }
    if((plazosFijosVec+ce-1)->cuenta<plazoFijo->cuenta)
        return plazosFijosVec+ce;
    else
    {
        return plazosFijosVec;
    }
}






bool esFechaValida_ALU(const Fecha* f)
{
    if(f->anio>=ANIO_BASE)
        if(f->mes>=1 && f->mes<=12)
            if(f->dia>=1 && f->dia<cantDiasMes_ALU(f->mes,f->anio))
                return true;
    return false;
}

int cantDiasMes_ALU(int mes,int anio)
{
    int cdm[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    if(mes==2 && (anio%400==0 || (anio%4==0 && anio%100!=0)))
        return 29;
    else
    {
        return cdm[mes];
    }
}

Fecha sumarDiasAFecha_ALU(const Fecha* f, unsigned dias)
{
    Fecha aux={f->dia+dias,f->mes,f->anio};
    while(aux.dia>cantDiasMes_ALU(aux.mes,aux.anio))
    {
        aux.dia-=cantDiasMes_ALU(aux.mes,aux.anio);
        aux.mes++;
        if(aux.mes>12)
        {
            aux.mes=1;
            aux.anio++;
        }
    }
    return aux;
}


void sumarDiasAFechaInSitu_ALU(Fecha* f, unsigned dias)
{
    f->dia+=dias;
    while(f->dia>cantDiasMes_ALU(f->mes,f->anio))
    {
        f->dia-=cantDiasMes_ALU(f->mes,f->anio);
        f->mes++;
        if(f->mes>12)
        {
            f->mes=1;
            f->anio++;
        }
    }
}


int cmpFechas_ALU(const Fecha* f1, const Fecha* f2)
{
    if(f1->anio>f2->anio)
        return 1;
    else
    {
        if(f1->anio<f2->anio)
            return -1;
        else
        {
            if(f1->mes>f2->mes)
                return 1;
            else
            {
                if(f1->mes<f2->mes)
                    return -1;
                else
                {
                    if(f1->dia>f2->dia)
                        return 1;
                    else
                    {
                        if(f1->dia<f2->dia)
                            return -1;
                    }
                }
            }
        }
    }
    return 0;
}



int txtAFecha_ALU(const char* txt, Fecha* fecha)
{
    char*act=strchr(txt,'\0');
    if(!act)
        return(0);

    act=strrchr(txt,'/');
    if(!act)
    {
        act=strrchr(txt,'-');
        if(!act)
            return(0);
    }
    sscanf(act+1,"%d",&fecha->anio);

    *act='\0';
    act=strrchr(txt,'/');
    if(!act)
    {
        act=strrchr(txt,'-');
        if(!act)
            return(0);
    }
    sscanf(act+1,"%d",&fecha->mes);

    *act='\0';
    sscanf(txt,"%d",&fecha->dia);

    return 1;
}
