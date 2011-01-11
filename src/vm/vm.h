#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

struct stivaLucru
{
	char *tip_nod;
	void *info;
	stivaLucru *urm;
	stivaLucru *prec;
};

struct stivaApSub
{
	int bloc;
	int level;
	stivaApSub *urm;
	stivaApSub *prec;
};

struct cod
{
	int n;
};

struct constante
{
	char *tip_const;
	void *con;
};

struct erori
{
	char *mesaj_err;
};

struct virtualM
{
	int sp;
	int vbaza;
	int ni;
	char *st;
	int rx;
	int ie;
	stivaLucru *stiva;
	stivaLucru *stivaVarf;
	stivaApSub *baza;
	stivaApSub *bazaVarf;
	cod tab_cod[100];
	constante tab_const[100];
	erori tab_err[10];
};

stivaLucru *getStivaLucruI(stivaLucru *stiva, int index);
stivaApSub *getStivaApSubI(stivaApSub *stiva, int index);
void vmInit(virtualM *vm);
void afisareStivaLucru(stivaLucru *stiva);
void afisareStivaApSub(stivaApSub *stiva);
void adaugaNodBaza(virtualM *vm, int bloc, int level);
void adaugaNodStiva(virtualM *vm, char *tipNod, void *info);
void stergeVarfStivaLucru(virtualM *vm);
double getInfoVarfAsDouble(virtualM *vm);
void stergeVarfBaza(virtualM *vm);

void LOD(virtualM *vm, int nivel, int adrel);
void LODI(virtualM *vm, int iconst);
void LODA(virtualM *vm, int nivel, int adrel);
void LODX(virtualM *vm, int nivel, int adrel);
void COPI(virtualM *vm);
void STO(virtualM *vm);
void MVRX(virtualM *vm);
void RED(virtualM *vm);
void UJP(virtualM *vm, int adr);
void FJP(virtualM *vm, int adr);
void LES(virtualM *vm);
void LEQ(virtualM *vm);
void GRT(virtualM *vm);
void GEQ(virtualM *vm);
void EQU(virtualM *vm);
void NEQ(virtualM *vm);
void ADD(virtualM *vm);
void SUB(virtualM *vm);
void MUL(virtualM *vm);
void DIV(virtualM *vm);
void MOD(virtualM *vm);
void RBM(virtualM *vm, int dim_var, int nr_loc);
void CALL(virtualM *vm, int adr_salt, int nr_par, int nivel);
void FCALL(virtualM *vm, int adr_salt, int nr_par, int nivel);
void STOP(virtualM *vm);
void ERR(virtualM *vm, int cod);
void INP(virtualM *vm, char *tip);
void OUT(virtualM *vm);
void CONV(virtualM *vm, char *tip, int nr_loc);
void AND(virtualM *vm);
void OR(virtualM *vm);
void NOT(virtualM *vm);
