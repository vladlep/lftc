#include "vm.h"
//vbaza porneste de la 1 pentru primul nivel din baza
//incarca valoarea
void LOD(virtualM *vm, int nivel, int adrel)
{
	int i = vm->vbaza;
	stivaApSub *baza = getStivaApSubI(vm->baza, i);
	//ajung la primul element de pe nivelul din stiva de baza
	while(nivel != baza->level)
	{
		i--;
		baza = baza->prec;
	}

	int adr = baza->bloc + adrel;
	stivaLucru *stiva = getStivaLucruI(vm->stiva, adr);
	while(strcmp(stiva->tip_nod, "adresa") == 0)
	{
		adr = *((int *)stiva->info);
		stiva = getStivaLucruI(vm->stiva, adr);
	}
	adaugaNodStiva(vm, stiva->tip_nod, stiva->info);
	vm->ni = vm->ni + 3;
}

void LODI(virtualM *vm, int iconst)
{
	adaugaNodStiva(vm, vm->tab_const[iconst].tip_const, vm->tab_const[iconst].con);
	vm->ni = vm->ni + 2;
}
//faci o adresa catre urm
void LODA(virtualM *vm, int nivel, int adrel)
{
	int i = vm->vbaza;
	stivaApSub *baza = getStivaApSubI(vm->baza, i);
	while(nivel != baza->level)
	{
		i--;
		baza = baza->prec;
	}

	int adr = baza->bloc + adrel;
	adaugaNodStiva(vm, "adresa", &adr);
	vm->ni = vm->ni + 3;
}

void LODX(virtualM *vm, int nivel, int adrel)
{
	int i = vm->vbaza;
	stivaApSub *baza = getStivaApSubI(vm->baza, i);
	while(nivel != baza->level)
	{
		i--;
		baza = baza->prec;
	}

	int adr = baza->bloc + adrel + vm->rx;
	stivaLucru *stiva = getStivaLucruI(vm->stiva, adr);
	while(strcmp(stiva->tip_nod, "adresa") == 0)
	{
		adr = *((int *)stiva->info);
		stiva = getStivaLucruI(vm->stiva, adr);
	}
	adaugaNodStiva(vm, stiva->tip_nod, stiva->info);
	vm->ni = vm->ni + 3;
}

void COPI(virtualM *vm)
{
	adaugaNodStiva(vm, vm->stivaVarf->tip_nod, vm->stivaVarf->info);
	vm->ni++;
}

void STO(virtualM *vm)
{
	int adr = *((int *)(vm->stivaVarf->prec->info));
	stivaLucru *stiva = getStivaLucruI(vm->stiva, adr);
	while(strcmp(stiva->tip_nod, "adresa") == 0)
	{
		adr = *((int *)(stiva->info));
		stiva = getStivaLucruI(vm->stiva, adr);
	}
	stiva->tip_nod = vm->stivaVarf->tip_nod;
	if(strcmp(stiva->tip_nod, "intreg") == 0)
		*((int *)(stiva->info)) = *((int *)(vm->stivaVarf->info));
	if(strcmp(stiva->tip_nod, "real") == 0)
		*((double *)(stiva->info)) = *((double *)(vm->stivaVarf->info));
	if(strcmp(stiva->tip_nod, "caracter") == 0)
		*((char *)(stiva->info)) = *((char *)(vm->stivaVarf->info));
	stergeVarfStivaLucru(vm);
	stergeVarfStivaLucru(vm);
	vm->ni++;
}

void MVRX(virtualM *vm)
{
	vm->rx = *((int *)(vm->stivaVarf->info));
	stergeVarfStivaLucru(vm);
	vm->ni++;
}

void RED(virtualM *vm)
{
	stergeVarfStivaLucru(vm);
	vm->ni--;
}

void UJP(virtualM *vm, int adr)
{
	vm->ni = adr;
}

void FJP(virtualM *vm, int adr)
{
	////!!!!! valoare din varful stivei e 0 adica NULL? ca stiva[SP} e o structura
	//daca se sterge valoare din varful stivei inteleg ca de fapt stiva[sp].info = 0
	if(*((int *)(vm->stivaVarf->info)) == 0)
	{
		vm->ni = adr;
	}
	else
	{
		vm->ni = vm->ni + 2;
	}
	stergeVarfStivaLucru(vm);
}

void LES(virtualM *vm)
{
	double d1,d2;
	d1 = getInfoVarfAsDouble(vm);
	stergeVarfStivaLucru(vm);

	d2 = getInfoVarfAsDouble(vm);
	stergeVarfStivaLucru(vm);

	int b;
	if(d2 < d1)
	{
		b = 1;
	}
	else
	{
		b = 0;
	}
	adaugaNodStiva(vm, "intreg", &b);
	vm->ni++;
}

void LEQ(virtualM *vm)
{
	double d1,d2;
	d1 = getInfoVarfAsDouble(vm);
	stergeVarfStivaLucru(vm);

	d2 = getInfoVarfAsDouble(vm);
	stergeVarfStivaLucru(vm);

	int b;
	if(d2 <= d1)
	{
		b = 1;
	}
	else
	{
		b = 0;
	}
	adaugaNodStiva(vm, "intreg", &b);
	vm->ni++;
}

void GRT(virtualM *vm)
{
	double d1,d2;
	d1 = getInfoVarfAsDouble(vm);
	stergeVarfStivaLucru(vm);

	d2 = getInfoVarfAsDouble(vm);
	stergeVarfStivaLucru(vm);

	int b;
	if(d2 > d1)
	{
		b = 1;
	}
	else
	{
		b = 0;
	}
	adaugaNodStiva(vm, "intreg", &b);
	vm->ni++;
}

void GEQ(virtualM *vm)
{
	double d1,d2;
	d1 = getInfoVarfAsDouble(vm);
	stergeVarfStivaLucru(vm);

	d2 = getInfoVarfAsDouble(vm);
	stergeVarfStivaLucru(vm);

	int b;
	if(d2 >= d1)
	{
		b = 1;
	}
	else
	{
		b = 0;
	}
	adaugaNodStiva(vm, "intreg", &b);
	vm->ni++;
}

void EQU(virtualM *vm)
{
	double d1,d2;
	d1 = getInfoVarfAsDouble(vm);
	stergeVarfStivaLucru(vm);

	d2 = getInfoVarfAsDouble(vm);
	stergeVarfStivaLucru(vm);

	int b;
	if(d2 == d1)
	{
		b = 1;
	}
	else
	{
		b = 0;
	}
	adaugaNodStiva(vm, "intreg", &b);
	vm->ni++;
}

void NEQ(virtualM *vm)
{
	double d1,d2;
	d1 = getInfoVarfAsDouble(vm);
	stergeVarfStivaLucru(vm);

	d2 = getInfoVarfAsDouble(vm);
	stergeVarfStivaLucru(vm);

	int b;
	if(d2 != d1)
	{
		b = 1;
	}
	else
	{
		b = 0;
	}
	adaugaNodStiva(vm, "intreg", &b);
	vm->ni++;
}

void ADD(virtualM *vm)
{
	char *tip1, *tip2;
	double d1,d2,d3;
	d1 = getInfoVarfAsDouble(vm);
	tip1 = (char *)malloc(sizeof(char)*(strlen(vm->stivaVarf->tip_nod) + 1));
	strcpy(tip1, vm->stivaVarf->tip_nod);
	stergeVarfStivaLucru(vm);

	d2 = getInfoVarfAsDouble(vm);
	tip2 = (char *)malloc(sizeof(char)*(strlen(vm->stivaVarf->tip_nod) + 1));
	strcpy(tip2, vm->stivaVarf->tip_nod);
	stergeVarfStivaLucru(vm);

	d3 = d1 + d2;
	if(strcmp(tip1, "intreg") == 0 && strcmp(tip2, "intreg") == 0)
	{
		int rI;
		rI = (int)d3;
		adaugaNodStiva(vm, "intreg", &rI);
	}
	if(strcmp(tip1, "real") == 0 && strcmp(tip2, "real") == 0)
	{
		adaugaNodStiva(vm, "real", &d3);
	}
	if(strcmp(tip1, "caracter") == 0 && strcmp(tip2, "caracter") == 0)
	{
		char rC;
		rC = (char)d3;
		adaugaNodStiva(vm, "caracter", &rC);
	}
	if((strcmp(tip1, "intreg") == 0 && strcmp(tip2, "adresa") == 0) 
		|| (strcmp(tip2, "intreg") == 0 && strcmp(tip1, "adresa") == 0))
	{
		int rI;
		rI = (int)d3;
		adaugaNodStiva(vm, "adresa", &rI);
	}
	vm->ni++;
	free(tip1);
	free(tip2);
}

void SUB(virtualM *vm)
{
	char *tip1, *tip2;
	double d1,d2,d3;
	d1 = getInfoVarfAsDouble(vm);
	tip1 = (char *)malloc(sizeof(char)*(strlen(vm->stivaVarf->tip_nod) + 1));
	strcpy(tip1, vm->stivaVarf->tip_nod);
	stergeVarfStivaLucru(vm);

	d2 = getInfoVarfAsDouble(vm);
	tip2 = (char *)malloc(sizeof(char)*(strlen(vm->stivaVarf->tip_nod) + 1));
	strcpy(tip2, vm->stivaVarf->tip_nod);
	stergeVarfStivaLucru(vm);

	d3 = d1 - d2;
	if(strcmp(tip1, "intreg") == 0 && strcmp(tip2, "intreg") == 0)
	{
		int rI;
		rI = (int)d3;
		adaugaNodStiva(vm, "intreg", &rI);
	}
	if(strcmp(tip1, "real") == 0 && strcmp(tip2, "real") == 0)
	{
		adaugaNodStiva(vm, "real", &d3);
	}
	if(strcmp(tip1, "caracter") == 0 && strcmp(tip2, "caracter") == 0)
	{
		char rC;
		rC = (char)d3;
		adaugaNodStiva(vm, "caracter", &rC);
	}
	if((strcmp(tip1, "intreg") == 0 && strcmp(tip2, "adresa") == 0) 
		|| (strcmp(tip2, "intreg") == 0 && strcmp(tip1, "adresa") == 0))
	{
		int rI;
		rI = (int)d3;
		adaugaNodStiva(vm, "adresa", &rI);
	}
	vm->ni++;
	free(tip1);
	free(tip2);
}

void MUL(virtualM *vm)
{
	char *tip1, *tip2;
	double d1,d2,d3;
	d1 = getInfoVarfAsDouble(vm);
	tip1 = (char *)malloc(sizeof(char)*(strlen(vm->stivaVarf->tip_nod) + 1));
	strcpy(tip1, vm->stivaVarf->tip_nod);
	stergeVarfStivaLucru(vm);

	d2 = getInfoVarfAsDouble(vm);
	tip2 = (char *)malloc(sizeof(char)*(strlen(vm->stivaVarf->tip_nod) + 1));
	strcpy(tip2, vm->stivaVarf->tip_nod);
	stergeVarfStivaLucru(vm);

	d3 = d1 * d2;
	if(strcmp(tip1, "intreg") == 0 && strcmp(tip2, "intreg") == 0)
	{
		int rI;
		rI = (int)d3;
		adaugaNodStiva(vm, "intreg", &rI);
	}
	if(strcmp(tip1, "real") == 0 && strcmp(tip2, "real") == 0)
	{
		adaugaNodStiva(vm, "real", &d3);
	}
	if(strcmp(tip1, "caracter") == 0 && strcmp(tip2, "caracter") == 0)
	{
		char rC;
		rC = (char)d3;
		adaugaNodStiva(vm, "caracter", &rC);
	}
	if((strcmp(tip1, "intreg") == 0 && strcmp(tip2, "adresa") == 0) 
		|| (strcmp(tip2, "intreg") == 0 && strcmp(tip1, "adresa") == 0))
	{
		int rI;
		rI = (int)d3;
		adaugaNodStiva(vm, "adresa", &rI);
	}
	vm->ni++;
	free(tip1);
	free(tip2);
}

void DIV(virtualM *vm)
{
	char *tip1, *tip2;
	double d1,d2,d3;
	d1 = getInfoVarfAsDouble(vm);
	tip1 = (char *)malloc(sizeof(char)*(strlen(vm->stivaVarf->tip_nod) + 1));
	strcpy(tip1, vm->stivaVarf->tip_nod);
	stergeVarfStivaLucru(vm);

	d2 = getInfoVarfAsDouble(vm);
	tip2 = (char *)malloc(sizeof(char)*(strlen(vm->stivaVarf->tip_nod) + 1));
	strcpy(tip2, vm->stivaVarf->tip_nod);
	stergeVarfStivaLucru(vm);

	if(d2 == 0)
	{
		perror("impartire cu 0");
		exit(0);
	}

	d3 = d1 / d2;
	adaugaNodStiva(vm, "real", &d3);
	
	vm->ni++;
	free(tip1);
	free(tip2);
}

void MOD(virtualM *vm)
{
	char *tip1, *tip2;
	double d1,d2,d3;
	d1 = getInfoVarfAsDouble(vm);
	tip1 = (char *)malloc(sizeof(char)*(strlen(vm->stivaVarf->tip_nod) + 1));
	strcpy(tip1, vm->stivaVarf->tip_nod);
	stergeVarfStivaLucru(vm);

	d2 = getInfoVarfAsDouble(vm);
	tip2 = (char *)malloc(sizeof(char)*(strlen(vm->stivaVarf->tip_nod) + 1));
	strcpy(tip2, vm->stivaVarf->tip_nod);
	stergeVarfStivaLucru(vm);

	if(d2 == 0)
	{
		perror("impartire cu 0");
		exit(0);
	}
	if(strcmp(tip1, "intreg") == 0 && strcmp(tip2, "intreg") == 0)
	{
		d3 = d1 - ((int)(d1 / d2)) * d2;	
		int rI;
		rI = (int)d3;
		adaugaNodStiva(vm, "intreg", &rI);
	}
	if(strcmp(tip1, "caracter") == 0 && strcmp(tip2, "caracter") == 0)
	{
		d3 = d1 - ((int)(d1 / d2)) * d2;	
		char rC;
		rC = (char)d3;
		adaugaNodStiva(vm, "caracter", &rC);
	}
	if((strcmp(tip1, "intreg") == 0 && strcmp(tip2, "adresa") == 0) 
		|| (strcmp(tip2, "intreg") == 0 && strcmp(tip1, "adresa") == 0))
	{
		d3 = d1 - ((int)(d1 / d2)) * d2;	
		int rI;
		rI = (int)d3;
		adaugaNodStiva(vm, "adresa", &rI);
	}
	vm->ni++;
	free(tip1);
	free(tip2);
}

//!!!! ce face exact aici?
void RBM(virtualM *vm, int dim_var, int nr_loc)
{
	vm->sp = vm->sp + nr_loc - 1;
	adaugaNodStiva(vm, "", &dim_var);
	vm->ni = vm->ni + 3;
}

//!!!! de intrebat cum functioneaza aici exact
void CALL(virtualM *vm, int adr_salt, int nr_par, int nivel)
{
	adaugaNodBaza(vm, vm->sp - nr_par + 1, nivel);
	stivaLucru *stiva = getStivaLucruI(vm->stiva, vm->bazaVarf->bloc - 1);
	int n = *((int *)(stiva->info));
	vm->sp = vm->sp + n;
	*((int *)(stiva->info)) = vm->ni + 4;
	vm->ni = adr_salt;
}

//!!!! nu trebuie sterse si aici ceva noduri?
//NI = Stiva[sp+1] ???? cumva NI = Stiva[sp+1].info?
void RET(virtualM *vm)
{
	vm->sp = vm->bazaVarf->bloc - 2;
	stivaLucru *stiva = getStivaLucruI(vm->stiva, vm->sp + 1);
	vm->ni = *((int *)(stiva->info));
	stergeVarfBaza(vm);
}

//!!!! ce fac aici???
void FCALL(virtualM *vm, int adr_salt, int nr_par, int nivel)
{
}

void STOP(virtualM *vm)
{
	vm->st = "stop";
}

void ERR(virtualM *vm, int cod)
{
	vm->st = "err";
	vm->ie = cod;
}

void INP(virtualM *vm, char *tip)
{
	int i;
	char c;
	double d;
	printf("introduceti o variabila de tip %s:", tip);
	if(strcmp(tip, "intreg") == 0 || strcmp(tip, "adresa") == 0)
	{
		scanf("%d", &i);
		adaugaNodStiva(vm, tip, &i);
	}
	if(strcmp(tip, "real") == 0)
	{
		scanf("%d", &d);
		adaugaNodStiva(vm, tip, &d);
	}
	if(strcmp(tip, "caracter") == 0)
	{
		scanf("%c", &c);
		adaugaNodStiva(vm, tip, &c);
	}	
	vm->ni = vm->ni + 2;
}

void OUT(virtualM *vm)
{
	if(strcmp(vm->stivaVarf->tip_nod, "intreg") == 0 || strcmp(vm->stivaVarf->tip_nod, "adresa") == 0)
	{
		printf("%d\n", *((int *)(vm->stivaVarf->info)));
		stergeVarfStivaLucru(vm);
	}
	if(strcmp(vm->stivaVarf->tip_nod, "real") == 0)
	{
		printf("%f\n", *((double *)(vm->stivaVarf->info)));
		stergeVarfStivaLucru(vm);
	}
	if(strcmp(vm->stivaVarf->tip_nod, "caracter") == 0)
	{
		printf("%c\n", *((char *)(vm->stivaVarf->info)));
		stergeVarfStivaLucru(vm);
	}	
	vm->ni++;
}

void CONV(virtualM *vm, char *tip, int nr_loc)
{
	int t = -1;
	double d;
	if(nr_loc == 0)
	{
		if((strcmp(vm->stivaVarf->tip_nod, "intreg") == 0 || strcmp(vm->stivaVarf->tip_nod, "adresa") == 0) && strcmp(tip, "intreg") != 0)
		{
			d = *((int *)(vm->stivaVarf->info));
			t = 0;
		}
		if(strcmp(vm->stivaVarf->tip_nod, "real") == 0 && strcmp(tip, "real") != 0)
		{
			d = *((double *)(vm->stivaVarf->info));
			t = 1;
		}
		if(strcmp(vm->stivaVarf->tip_nod, "caracter") == 0 && strcmp(tip, "caracter") != 0)
		{
			d = *((char *)(vm->stivaVarf->info));
			t = 2;
		}	
		if(t != -1)
		{
			free(vm->stivaVarf->info);
			if(strcmp(tip, "intreg") == 0)
			{
				vm->stivaVarf->info = (int *)malloc(sizeof(int));
				*(int *)vm->stivaVarf->info = (int)d;
				vm->stivaVarf->tip_nod = "intreg";
			}
			if(strcmp(tip, "adresa") == 0)
			{
				vm->stivaVarf->info = (int *)malloc(sizeof(int));
				*(int *)vm->stivaVarf->info = (int)d;
				vm->stivaVarf->tip_nod = "adresa";
			}
			if(strcmp(tip, "real") == 0)
			{
				vm->stivaVarf->info = (double *)malloc(sizeof(double));
				*(double *)vm->stivaVarf->info = d;
				vm->stivaVarf->tip_nod = "double";
			}
			if(strcmp(tip, "caracter") == 0)
			{
				vm->stivaVarf->info = (char *)malloc(sizeof(char));
				*(char *)vm->stivaVarf->info = (char)d;
				vm->stivaVarf->tip_nod = "caracter";
			}
			vm->ni = vm->ni + 3;
		}
	}
	if(nr_loc == 1)
	{
		if((strcmp(vm->stivaVarf->prec->tip_nod, "intreg") == 0 || strcmp(vm->stivaVarf->prec->tip_nod, "adresa") == 0) && strcmp(tip, "intreg") != 0)
		{
			d = *((int *)(vm->stivaVarf->prec->info));
			t = 0;
		}
		if(strcmp(vm->stivaVarf->prec->tip_nod, "real") == 0 && strcmp(tip, "real") != 0)
		{
			d = *((double *)(vm->stivaVarf->prec->info));
			t = 1;
		}
		if(strcmp(vm->stivaVarf->prec->tip_nod, "caracter") == 0 && strcmp(tip, "caracter") != 0)
		{
			d = *((char *)(vm->stivaVarf->prec->info));
			t = 2;
		}	
		if(t != -1)
		{
			free(vm->stivaVarf->prec->info);
			if(strcmp(tip, "intreg") == 0)
			{
				vm->stivaVarf->prec->info = (int *)malloc(sizeof(int));
				*(int *)vm->stivaVarf->prec->info = (int)d;
				vm->stivaVarf->prec->tip_nod = "intreg";
			}
			if(strcmp(tip, "adresa") == 0)
			{
				vm->stivaVarf->prec->info = (int *)malloc(sizeof(int));
				*(int *)vm->stivaVarf->prec->info = (int)d;
				vm->stivaVarf->prec->tip_nod = "adresa";
			}
			if(strcmp(tip, "real") == 0)
			{
				vm->stivaVarf->prec->info = (double *)malloc(sizeof(double));
				*(double *)vm->stivaVarf->prec->info = d;
				vm->stivaVarf->prec->tip_nod = "double";
			}
			if(strcmp(tip, "caracter") == 0)
			{
				vm->stivaVarf->prec->info = (char *)malloc(sizeof(char));
				*(char *)vm->stivaVarf->prec->info = (char)d;
				vm->stivaVarf->prec->tip_nod = "caracter";
			}
			vm->ni = vm->ni + 3;
		}
	}
}

void AND(virtualM *vm)
{
	double d1,d2;
	d1 = getInfoVarfAsDouble(vm);
	stergeVarfStivaLucru(vm);

	d2 = getInfoVarfAsDouble(vm);
	stergeVarfStivaLucru(vm);

	int b;
	if(d2 && d1)
	{
		b = 1;
	}
	else
	{
		b = 0;
	}
	adaugaNodStiva(vm, "intreg", &b);
	vm->ni++;
}

void OR(virtualM *vm)
{
	double d1,d2;
	d1 = getInfoVarfAsDouble(vm);
	stergeVarfStivaLucru(vm);

	d2 = getInfoVarfAsDouble(vm);
	stergeVarfStivaLucru(vm);

	int b;
	if(d2 || d1)
	{
		b = 1;
	}
	else
	{
		b = 0;
	}
	adaugaNodStiva(vm, "intreg", &b);
	vm->ni++;
}

void NOT(virtualM *vm)
{
	double d1;
	d1 = getInfoVarfAsDouble(vm);
	stergeVarfStivaLucru(vm);

	int b;
	if(d1)
	{
		b = 0;
	}
	else
	{
		b = 1;
	}
	adaugaNodStiva(vm, "intreg", &b);
	vm->ni++;
}

stivaLucru *getStivaLucruI(stivaLucru *stiva, int index)
{
	stivaLucru *stivaAux = stiva;
	while(index > 1)
	{
		stivaAux = stivaAux->urm;
		index--;
	}
	return stivaAux;
}
/** returneaza varful stivei*/
stivaApSub *getStivaApSubI(stivaApSub *stiva, int index)
{
	stivaApSub *stivaAux = stiva;
	while(index > 1)
	{
		stivaAux = stivaAux->urm;
		index--;
	}
	return stivaAux;
}

void vmInit(virtualM *vm)
{
	vm->sp = 0;
	vm->vbaza = 0;
	vm->ni = 0;
	vm->st = "";
	vm->rx = 0;
	vm->ie = 0;
	vm->stiva = NULL;
	vm->stivaVarf = NULL;
	vm->baza = NULL;
	vm->bazaVarf = NULL;
}

void afisareStivaLucru(stivaLucru *stiva)
{
	stivaLucru *s = stiva;
	printf("\nStiva de lucru:\n");
	while(s != NULL)
	{
		if(strcmp(s->tip_nod, "intreg") == 0)
			printf("tip nod:%s valoare nod:%d\n", s->tip_nod, *((int *)s->info));
		if(strcmp(s->tip_nod, "real") == 0)
			printf("tip nod:%s valoare nod:%f\n", s->tip_nod, *((double *)s->info));
		if(strcmp(s->tip_nod, "caracter") == 0)
			printf("tip nod:%s valoare nod:%c\n", s->tip_nod, *((char *)s->info));
		if(strcmp(s->tip_nod, "adresa") == 0)
			printf("tip nod:%s valoare nod:%d\n", s->tip_nod, *((int *)s->info));
		s = s->urm;
	}
	printf("\n");
}

void afisareStivaApSub(stivaApSub *stiva)
{
	stivaApSub *s = stiva;
	printf("\nStiva pentru apeluri de subprograme:\n");
	while(s != NULL)
	{
		printf("indice bloc stiva de lucru:%d level:%d\n", s->bloc, s->level);
		s = s->urm;
	}
}

void adaugaNodBaza(virtualM *vm, int bloc, int level)
{
	stivaApSub *bazaNew = (stivaApSub *)malloc(sizeof(stivaApSub));
	bazaNew->level = level;
	bazaNew->bloc = bloc;
	bazaNew->prec = NULL;
	bazaNew->urm = NULL;

	if(vm->baza == NULL)
	{
		vm->baza = bazaNew;
		vm->bazaVarf = bazaNew;
		vm->vbaza = 1;
	}
	else
	{
		bazaNew->prec = vm->bazaVarf;
		vm->bazaVarf->urm = bazaNew;
		vm->bazaVarf = bazaNew;
		vm->vbaza++;
	}
}

void adaugaNodStiva(virtualM *vm, char *tipNod, void *info)
{
	stivaLucru *stivaNew = (stivaLucru *)malloc(sizeof(stivaLucru));
	stivaNew->tip_nod = (char *)malloc(sizeof(char)*(strlen(tipNod)+1));
	strcpy(stivaNew->tip_nod, tipNod);	
	stivaNew->prec = NULL;
	stivaNew->urm = NULL;

	if(strcmp(tipNod,"intreg") == 0)
	{
		stivaNew->info = (int *)malloc(sizeof(int));
		*((int *)stivaNew->info) = *((int *)info);
	}
	if(strcmp(tipNod,"real") == 0)
	{
		stivaNew->info = (double *)malloc(sizeof(double));
		*((double *)stivaNew->info) = *((double *)info);
	}
	if(strcmp(tipNod,"caracter") == 0)
	{
		stivaNew->info = (char *)malloc(sizeof(char));
		*((char *)stivaNew->info) = *((char *)info);
	}
	if(strcmp(tipNod,"adresa") == 0)
	{
		stivaNew->info = (int *)malloc(sizeof(int));
		*((int *)stivaNew->info) = *((int *)info);
	}

	if(vm->stiva == NULL)
	{
		vm->stiva = stivaNew;
		vm->stivaVarf = stivaNew;
		vm->sp = 1;
	}
	else
	{
		stivaNew->prec = vm->stivaVarf;
		vm->stivaVarf->urm = stivaNew;
		vm->stivaVarf = stivaNew;
		vm->sp++;
	}
}

void stergeVarfStivaLucru(virtualM *vm)
{
	stivaLucru *stiva = vm->stivaVarf;
	vm->stivaVarf = vm->stivaVarf->prec;
	vm->stivaVarf->urm = NULL;
	free(stiva->info);
	free(stiva);
	vm->sp--;
}

void stergeVarfBaza(virtualM *vm)
{
	stivaApSub *stiva = vm->baza;
	vm->bazaVarf = vm->bazaVarf->prec;
	vm->bazaVarf->urm = NULL;
	free(stiva);
	vm->vbaza--;
}

double getInfoVarfAsDouble(virtualM *vm)
{
	double d;
	if(strcmp(vm->stivaVarf->tip_nod, "caracter") == 0)
	{
		d = *((char *)vm->stivaVarf->info);
	}
	if(strcmp(vm->stivaVarf->tip_nod, "intreg") == 0)
	{
		d = *((int *)vm->stivaVarf->info);
	}
	if(strcmp(vm->stivaVarf->tip_nod, "real") == 0)
	{
		d = *((double *)vm->stivaVarf->info);
	}
	if(strcmp(vm->stivaVarf->tip_nod, "adresa") == 0)
	{
		d = *((int *)vm->stivaVarf->info);
	}
	return d;
}