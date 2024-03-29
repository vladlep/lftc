#include <stdio.h>


#include "vm.h"

void testLOD1();

int main(int argc, char *argv[])
{
	testLOD1();
	getch();
}


void testLOD1()
{
	virtualM vm;
	vmInit(&vm);
	
	//LOD
	int n1 = 7;
	adaugaNodStiva(&vm, "intreg", &n1);
	double n2 = 7.5;
	adaugaNodStiva(&vm, "real", &n2);
	char n3 = 'c';
	adaugaNodStiva(&vm, "caracter", &n3);
	int n4 = 1;
	adaugaNodStiva(&vm, "adresa", &n4);
	afisareStivaLucru(vm.stiva);
	
	adaugaNodBaza(&vm, 1, 1);
	adaugaNodBaza(&vm, 2, 2);
	afisareStivaApSub(vm.baza);

	LOD(&vm, 2, 0);
	LOD(&vm, 2, 1);
	LOD(&vm, 2, 4);
	LOD(&vm, 1, 4);
	afisareStivaLucru(vm.stiva);
	printf("ni:%d\n",vm.ni);
	printf("sp:%d\n",vm.sp);

	//LODI
	int c1 = 11;
	char c2 = 'd';
	double c3 = 11.5;
	vm.tab_const[0].con = &c1;
	vm.tab_const[0].tip_const = "intreg";
	vm.tab_const[1].con = &c2;
	vm.tab_const[1].tip_const = "caracter";
	vm.tab_const[2].con = &c3;
	vm.tab_const[2].tip_const = "real";
	LODI(&vm, 2);
	afisareStivaLucru(vm.stiva);
	printf("ni:%d\n",vm.ni);
	printf("sp:%d\n",vm.sp);

	//LODA
	LODA(&vm, 1, 1);
	LODA(&vm, 1, 3);
	LOD(&vm, 1, 10);
	afisareStivaLucru(vm.stiva);
	printf("ni:%d\n",vm.ni);
	printf("sp:%d\n",vm.sp);

	//COPI
	COPI(&vm);
	afisareStivaLucru(vm.stiva);
	printf("ni:%d\n",vm.ni);
	printf("sp:%d\n",vm.sp);

	//STO
	LODA(&vm, 1, 10);
	LODI(&vm, 0);
	afisareStivaLucru(vm.stiva);
	STO(&vm);
	afisareStivaLucru(vm.stiva);
	printf("ni:%d\n",vm.ni);
	printf("sp:%d\n",vm.sp);

	//LES
	int nod1 = 1;
	int nod2 = 2;
	adaugaNodStiva(&vm, "intreg", &nod1);
	adaugaNodStiva(&vm, "intreg", &nod2);
	LES(&vm);
	afisareStivaLucru(vm.stiva);
	printf("ni:%d\n",vm.ni);
	printf("sp:%d\n",vm.sp);

	//ADD
	nod1 = 5;
	nod2 = 12;
	adaugaNodStiva(&vm, "intreg", &nod1);
	adaugaNodStiva(&vm, "adresa", &nod2);
	ADD(&vm);
	afisareStivaLucru(vm.stiva);
	printf("ni:%d\n",vm.ni);
	printf("sp:%d\n",vm.sp);

	//CALL
	CALL(&vm, 1, 2, 2);
	afisareStivaLucru(vm.stiva);
	afisareStivaApSub(vm.baza);
	printf("ni:%d\n",vm.ni);
	printf("sp:%d\n",vm.sp);
}
