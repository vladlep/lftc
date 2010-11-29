#ifndef included
 #define included
  
 struct atomi
 {
   int lineNO;
   //int atomNo;
   int codLexical;
   /*valori pentru codul lexical
   0 - operatori
   1 - cuvCheie
   2 - identificator
   3 - const integer
   4 - const real
   5 - const char
   6 - const sir
   7 - delimitator / semn punctuatoe
   8 - coment delimiters { | }
   */
   int atributId ; // indicile din tabela care ii corespunde atributului    
   char atom[20];
   struct atomi *urm;
   struct atomi *pre;   
 }	;
 
 
  struct tabelaSimboluri
 {

   char nume[25];
   char clasa[15];
   int tip ; 
   int index_val;
	 int adrel;
	 int deplrec;
	 int nivel;
	 int nr_par;
	 int dim_var;
	 int adr_start;
	 int lista_param[2][10];
	 int min;
	 int max;
	 int lista_rec[10];
	 int nr_lista_rec;
	 int incdom;
 }	;
 
 #define MAX_TS 100
 struct tabelaSimboluri ts[MAX_TS];
 int dimts;
 int dim_parent;
 int nivel;
 int adrel; 
 int tip;
 struct atomi *first;
 
 int constIntreg[100];
 double constReal[100];
 char constChar[100];
 char *constString[100];
 char *identif[100];
 int lenConstInt, lenConstReal, lenConstChar, lenConstString, lenIdentif; // lungimile tablourilor
 
 //tabela cu toate cuvintele limbajului
 #define TAB_CHEI_SIZE 29
 char *tabelaChei[] = {"and","begin","case","char","const","div","do","downto","else",
      "end","for","function","if","integer","mod","not","of","or","procedure","program","real",
      "repeat","then","until","var","while","record","array","to"};
 #define TAB_OPERATORI_SIZE 15
 char *tableOperatori[]={"+","-","*","/","(", ")","[","]","<","<=",">",">=","<>","=",":="} ;    
 #define TAB_DELIMITER_SIZE 5
 char *tableDelimiter[]={";",".",",",":",".."};
      

 void sapareteAtoms();
 
 //descide fisierul
 void openFile(const char* path); 
 // inchide fisierul deschis
 void closeFile();
 
 
 //afisez pe ecran toti atomii memorati
 void printfAtoms();    
 
  void printTS();
  void removeLevel(int nivel);
 
  struct atomi* checkBlock(struct atomi *curent);
  struct atomi* expresie(struct atomi* curent);
  struct atomi* instrComp(struct atomi* curent);
  struct atomi* instr(struct atomi* curent);
  struct atomi* conditie(struct atomi* curent);
  
#endif

