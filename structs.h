
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
struct atomi *first;

int constIntreg[100];
double constReal[100];
char constChar[100];
char *constString[100];
char *identif[100];
int lenConstInt, lenConstReal, lenConstChar, lenConstString, lenIdentif; // lungimile tablourilor

//tabela cu toate cuvintele limbajului
#define TAB_CHEI_SIZE 26
char *tabelaChei[] = {"and","begin","case","char","const","div","do","downto","else",
     "end","for","function","if","integer","mod","not","of","or","procedure","program","real",
     "repeat","then","until","var","while"};
#define TAB_OPERATORI_SIZE 15
char *tableOperatori[]={"+","-","*","/","(", ")","[","]","<","<=",">",">=","<>","=",":="} ;    
#define TAB_DELIMITER_SIZE 5
char *tableDelimiter[]={";",".",",",":",".."};
     

void sapareteAtoms();

//descide fisierul
void openFile(const char* path); 
// inchide fisierul deschis
void closeFile();

