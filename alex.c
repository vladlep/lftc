 #include <stdlib.h>
 #include <ctype.h>
 #include <stdio.h>
 #include <math.h>
 #include <string.h>
 #include <malloc.h>
 #include "alex.h"
 
 FILE *f;  // fisierul ce il voi compila;
 
 
 /*
 
 
 //aloca memorie si salveaza atomul
 //int createAndSaveAtom(int atribut, int codLexical, int linNo, char atom[20]);
 
 //identific tipul atomului si il trimite sper memorare
 int identifyStringAndSave(char* atom, int len, int lineNo);
 //calculez si salvez int cu baza @ nr
 int calcAndSaveBaseInteger(char* atom,int len,int lineNo);
 //calculez si salvez int normal, fara baza;
 int calcAndSaveNormalInteger(char* atom,int len,int lineNo);
 //interpreteaza primii 2 atomi ai arrayului
 int interpretArray(char* atom,int len,int lineNo);
 //calculez realul normal;
 int identifyNormalReal(char* atom,int len,int lineNo);
 
 void writeToConstInt(int nr);
 void writeToConstReal(double nr);
 void writeToConstChar(char c);
 void writeToConstString(char* str);
 */
 
  //fucntions to check the semantics 
int constanta(struct atomi* curent)
{
    if( curent->codLexical ==4 || curent->codLexical ==3 || curent->codLexical ==5 ) // real || intreg || char
        return 1;
    return 0;
}//final&& finihed

int opAdd(struct atomi* curent)
{
    if( curent->codLexical ==0  && (curent->atributId ==0  || curent->atributId ==1)) // + | -
        return 1;
    return 0;
}//final&& finihed

int opMul(struct atomi* curent)
{
       if( curent->codLexical ==0  && (curent->atributId ==2  || curent->atributId ==3) ) // + | -
        return 1;
    return 0;
}//final&& finihed

struct atomi* expresieStatica(struct atomi* curent)
{
         if(curent->codLexical == 0 && curent->atributId ==4 ) //(
           {                    
               // printf(" if1aaa %s  \n",curent->atom); 
           
            curent = expresieStatica(curent->urm);
              

  
             if (!(curent->codLexical == 0 && curent->atributId ==5 )) //   )  
             {
              if(curent!=NULL)
                 printf( " Not enough ). Line %d\n",curent->lineNO);   
                return NULL;
             } 
              if( opAdd(curent->urm) || opMul(curent->urm) ) 
              {
                  curent= curent->urm->urm;
                  curent = expresieStatica(curent);
          //          printf(" dupa )  %s   \n",curent->atom); 
              }else
              curent= curent->urm;
                         }
          
       if(curent->codLexical == 2 || constanta(curent) ==1 )// indent || constanta
       {
      //   printf(" if2 %s %d   \n",curent->atom, constanta(curent)); 
          curent = curent->urm;
          if( opAdd(curent) || opMul(curent) ) 
           {
           curent = curent ->urm;
           curent = expresieStatica(curent);
           return curent;
           }
           else 
            {
            if(curent->codLexical ==7 && curent->atributId ==0 || (curent->codLexical == 0 && curent->atributId ==5 )) //;
              return curent;
          //  if(curent!=NULL)
            //  printf("An operator was expected at line : %d\n",curent->lineNO);
            return curent;       
            }
       }
       
    
      // printf("Eror in const declaratio. Line %d\n",curent->lineNO);
       return curent;
}

struct atomi* declarConst(struct atomi* curent)
{
      // printf("declar const ");
    
  if(curent->codLexical == 2) // identificator     
  {
   curent = curent->urm;
   if(curent->codLexical == 0 && curent->atributId ==13) //=
   {
          curent = curent->urm;             
           if(curent->codLexical == 2 || (curent->codLexical == 0 && curent->atributId ==4) || constanta(curent) ==1)// indent || ( || constanta
           {
          curent = expresieStatica(curent);
            if( curent == NULL)
            {
                return NULL;
            }
            if( !(curent->codLexical == 7 && curent->atributId ==0)) // ;
                printf("No ; at end of const declaration. %d Instead we have %s\n",curent->lineNO, curent->atom);
            
          return curent->urm; 
          }
          else
          printf("No staticalExpresion. Line %d\n",curent->lineNO);
   }   
   else
       printf("No = sign. Line %d\n",curent->lineNO);  
  }
  return NULL;
}
 
struct atomi* sectiuneConst(struct atomi* curent)
{
       struct atomi* temp =curent;
       curent = declarConst(curent);
      
       if( curent == NULL)
       {           
          printf("eroare.No corect const declared line %d\n",temp->lineNO); 
          return NULL;          
  
       }else
       {
            
        temp = curent;     
        while( ( curent = declarConst(curent)) !=NULL) temp = curent;      
        return temp;
       }
            
  return curent;
}
int tipSimplu(struct atomi* curent)
{
  if((curent->codLexical == 1 && curent->atributId ==13)||(curent->codLexical == 1 && curent->atributId ==3) || (curent->codLexical == 1 && curent->atributId ==20) )// integer || real || char
      return 1;  //true                     
 return 0;   
}//final & finished

struct atomi* tipTablou(struct atomi* curent)
{
       printf("array type\n");
           if(curent->codLexical == 0 && curent->atributId ==6) ;//[
           {
             curent = expresieStatica(curent->urm);      
             if(curent== NULL)
               {
                  printf("Not a normal static expresion\n");
                  return NULL;
               }
               else
                if( curent->codLexical == 7 && curent->atributId ==4) // ..
                    {
                                      
                        curent = expresieStatica(curent->urm);                   
                        if(curent== NULL)
                        {
                          printf("Not a normal second static expresion\n");
                          return NULL;
                         }
                        else
                          if(curent->codLexical == 0 && curent->atributId ==7) //]
                          {
                              curent = curent->urm;
                              if(curent->codLexical == 1 && curent->atributId ==16) // of
                              {
                                 curent = curent->urm;
                                 if(tipSimplu(curent)  == 1)
                                  {
                                  printf("good array\n");
                                  return curent->urm;
                                  }else
                                  {
                                  printf("no type of array specified\n");
                                  return NULL;
                                  }
                              }
                              else
                              {   
                                  printf("specify what is the type of the array . Line %d\n",curent->lineNO);
                                  return NULL;
                              }
                          }else 
                          {
                               printf("No ] to close the array declaration\n");
                               return NULL;
                          }
                    }
                    else
                    {
                        printf("missing .. to specify the interval \n");
                        return NULL;
                    }            
           }    
}

struct atomi* declSimpla(struct atomi* curent)
{
     //  printf("\n\n,,,,,,,%s\n",curent->atom);
 if(curent->codLexical == 2) // identificator     
  {
   curent = curent->urm;
  
       if(curent->codLexical == 7 && curent->atributId ==1) //,
       {
            curent = curent->urm; 
            while(curent->codLexical == 2) //identificator
            {
                curent = curent->urm; 
                  if(curent->codLexical == 7 && curent->atributId ==1) //,
                      curent = curent->urm;  
           }
       }
       if(curent->codLexical == 7 && curent->atributId ==3) //:
       {
              curent = curent->urm;             
               if( tipSimplu(curent) == 1)
               {
                return curent-> urm; // s-a terminat de declarat un atom de tip decalr Simplu
               }
            else
             { 
                  printf("No simple type declared for the variable. Line %d\n",curent->lineNO);
                  return NULL;
              }
       }   
       else
       {
        printf("No : sign. Line %d\n",curent->lineNO);  
        return NULL;
       }
  }
  return NULL;
       
}
struct atomi* listaCamp(struct atomi* curent)
{ 
   int i=0;
    struct atomi* temp;
   do
   {
       temp = curent;
       i++;
       curent = declSimpla(curent);
      // curent = curent->urm;
    
       if(curent != NULL) 
       {
       // printf("lista camp %s\n", curent->atom);
         curent = curent->urm;
         }
   }while(curent !=NULL && (curent->pre->codLexical == 7 && curent->pre->atributId ==0)); // ;
     if(i== 1 && curent == NULL)
         printf(" we don't have any declared var in the struct \n");
   return temp; // NULL || curent-ul returnat
}



struct atomi* tipVar(struct atomi* curent)
{
    
 if(tipSimplu(curent) ==1  ) // simplu 
   return curent->urm;     
 else
     if((curent->codLexical == 1 && curent->atributId ==26)) //record
       {
          printf("record type found\n");
          curent = listaCamp(curent->urm);
          if(curent == NULL)
                return NULL;
          if(curent->codLexical == 1 && curent->atributId ==9) //end
            return curent->urm;
          printf("Missing the end key word. Line %d\n", curent->lineNO);
          return NULL;
       }//end record
      else
        if((curent->codLexical == 1 && curent->atributId ==27) ) // array
        {
        curent = tipTablou(curent->urm) ;
        if( curent == NULL) 
            return NULL;
       //     printf( "  %s  \n\n",curent->atom);
        return curent;
        }//array type
    printf("Not a corect type set for the variable. Line %d\n",curent->lineNO);
 return NULL;
}

struct atomi* declarVar(struct atomi* curent)
{ 
  if(curent->codLexical == 2) // identificator     
  {
   curent = curent->urm;
   //TODO identificator cu, intre
  
       if(curent->codLexical == 7 && curent->atributId ==1) //,
       {
            curent = curent->urm; 
            while(curent->codLexical == 2) //identificator
            {
                curent = curent->urm; 
                  if(curent->codLexical == 7 && curent->atributId ==1) //,
                      curent = curent->urm;  
           }
       }
       if(curent->codLexical == 7 && curent->atributId ==3) //:
       {
              curent = curent->urm;             
               if( (curent->codLexical == 1 && curent->atributId ==26) || (curent->codLexical == 1 && curent->atributId ==13)||(curent->codLexical == 1 && curent->atributId ==27) || (curent->codLexical == 1 && curent->atributId ==3) || (curent->codLexical == 1 && curent->atributId ==20) )// integer || real || char || array || record
               {
                curent = tipVar(curent);
                if( curent == NULL)
                {
                    return NULL;
                }
                if( !(curent->codLexical == 7 && curent->atributId ==0)) // ;
                    printf("No ; at end of variable declaration. %d Instead we have %s\n",curent->lineNO, curent->atom);
                
              return curent->urm; 
              }
              else
              printf("No type for the variable declared. Line %d\n",curent->lineNO);
       }   
       else
       printf("No : sign. Line %d\n",curent->lineNO);  
  }
  else 
       printf("Warning (good except for the last var): No identificator in var declaration %d\n",curent->lineNO);
  return NULL;
}

struct atomi* sectiuneVar(struct atomi* curent)
{
  printf("SectiuneVar %d\n first atom : %s\n",curent->lineNO, curent->atom); 
  struct atomi* temp =curent;
  curent =  declarVar(curent); 
  if(curent ==NULL )
  {
     printf("first atom isn't an variable decalration");
     return NULL;
  }else
       {
   
         printf("returned %d\n first atom : %s\n",curent->lineNO, curent->atom);    
        temp = curent;     
      while( ( curent = declarVar(curent)) !=NULL) temp = curent;      
        return temp;
       }
 
  return curent;
}

struct atomi* instrComp(struct atomi* curent)
{
       printf("InstrComp %d\n",curent->lineNO); 
      if( !(curent->codLexical == 1 && curent->atributId ==1)) // begin
      {
          printf("No begin in the %s\n",curent->atom);
      }
      else
      {
         // curent = instructiuni(curent->urm);
          if(curent==NULL)
          {
            printf("Nu exista o instructiune corecta");
            return NULL;
          }
      }
     
      return NULL;
}

      

      
struct atomi* sectiuneDeclSubprog(struct atomi* curent)
{

  printf("SectiuneDeclSubprog %d\n",curent->urm->lineNO); 
  return curent->urm;
}
      

 struct atomi* checkBlock(struct atomi *curent)
 {
      if(curent!=NULL && curent->codLexical ==1 && curent->atributId==4)//const
      {//SectiuneConst - const
         printf("avem sectiune de const\n");
         curent = sectiuneConst(curent->urm);
         if(curent == NULL) // eroare in functia sectiuneConst
            return NULL;
      }
      if( curent!=NULL && curent->codLexical ==1 && curent->atributId==24)//var
      {//SectiuneVar  - var
        curent = sectiuneVar(curent->urm);

         if(curent == NULL)
            return NULL;      
          printf("first atom after sectiune var : %s\n ", curent->atom);
      }
       if(curent!=NULL && curent->codLexical ==1 && (curent->atributId==18 || curent->atributId==11)) // procedure | function
      {//SectiuneDeclSubprog  
          //curent = sectiuneDeclSubprog(curent); // nu trec la urm atom
         if(curent == NULL) // eroare in functia sectiuneConst
            return NULL;
        //printf("subprogram\n");
      }
      if(curent!=NULL && curent->codLexical ==1 && curent->atributId==1)//begin
      {//InstrComp - begin
      
//         curent =  instrComp(curent->urm);        
         if(curent == NULL) // eroare in functia sectiuneConst
            return NULL;    
          return curent;         
      }
      else
      {
         if(curent!=NULL)
           printf("block not closed. No mandatory begin. Or other eroes on the way Line%d\n",curent->lineNO);
         return NULL;
      }
 }           
 void checkProgram(struct atomi *curent)
 {
      if( curent->codLexical ==1 && curent->atributId==19) // program 
      {
          curent = curent->urm;
          if(curent->codLexical == 2) // identificator
          {        
             curent = curent->urm;
             if(curent->codLexical ==7 && curent->atributId ==0) //;
               {
                   
                 curent = checkBlock(curent->urm); 
                   if(curent == NULL) // eroare in functia sectiuneConst
                       return;
                  if(!(curent->codLexical ==7 && curent->atributId ==2) && curent== first->pre) //. & last atom
                  {
                      perror("no . at end of project");
                  }
               }
             else
             if(curent!=NULL)
                printf("no ; delimeter at end of program declaration.Line %d\n",curent->lineNO);
         }else
             if(curent!=NULL)
                printf("no identificator after program. Line %d\n",curent->lineNO);
      }else
        if(curent!=NULL)
          perror("doesn't start with program key word");
 }
 void checkProject(struct atomi *curent)
 {
 	checkProgram(curent);
 	
 	
 }
 
 int main(int argc, char *argv[])
 {
    
     openFile("in.txt");
     sapareteAtoms();
     //printf("%c",getCharUrm());
     closeFile();
     printfAtoms();
     checkProject(first);
     getchar();
    return 0;
 }
 
 void openFile(const char* path)
 {
 
     if( (f= fopen(path,"r"))==NULL)
     {
         perror("eroare deschidere");
         exit(1);
     }
 }
 
 void closeFile()
 {
     if( fclose(f) !=0)
     {
         perror("eroare inchidere");
     	exit(1);
     }
 }    
 
 int getCharUrm(char* nextChar)
 {
  return fscanf(f,"%c",nextChar);
 }  
 
 
 void writeToConstInt(int nr){
 	constIntreg[lenConstInt] = nr;
 	lenConstInt++;
 }
 void writeToConstReal(double nr){
 	constReal[lenConstReal] = nr;
 	lenConstReal++;
 	}
 	
 void writeToConstChar(char c){
 	constChar[lenConstChar++] = c;
 }
 void writeToConstString(char* str){
 	constString[lenConstString] = (char*) malloc(sizeof(str)+1);
 	strcpy(constString[lenConstString], str);
 }
 
 /**
 @return  0 case of erors 
  		1 case identification was succesful
 */
 int createAndSaveAtom(int atribut, int codLexical, int lineNo, char atom[])
 {
 	struct atomi *temp;
 	if(first == NULL)
 	{
 		if( (first = (struct atomi * ) malloc(sizeof(struct atomi)) ) == NULL)
 		{
 			perror("memory alloc error");	
 			return 0; // esec la alocare memorie
 		}	
 		first->pre = first;
 		first->urm = first;
 		first->lineNO = lineNo;
 		first->codLexical = codLexical;
 		first->atributId = atribut;
 		strcpy(first->atom,atom);
 	}
 	else
 	{
 		if( (temp = (struct atomi * ) malloc(sizeof(struct atomi)) ) == NULL)
 		{
 			perror("memory alloc error");	
 			return 0; // esec la alocare memorie
 		}		
 		first->pre -> urm = temp;
 		temp->pre = first->pre;
 		temp->urm = first;
 		first->pre = temp;
 		temp->lineNO = lineNo;
 		temp->codLexical = codLexical;
 		temp->atributId = atribut;
 		strcpy(temp->atom, atom);
 
 	}
 	 return 1; // succcesful
 }//end createAndSaceAtom
 
 
 
 /**
 @return  0 case of erors 
  		1 case identification was succesful
 */
 int identifyStringAndSave(char* atom, int len,int lineNo)
 {
 	atom[len] = '\0';
 	
 	int i;
 	for( i=0; i<TAB_CHEI_SIZE; i++)
 	{
 	//	printf("%s\n",tabelaChei[i]);
 		if(strcmp(tabelaChei[i], atom)==0 )
 		{
 			if( createAndSaveAtom(i,1,lineNo,tabelaChei[i])==0)
 				return 0;
 			//printf("\n\n%s",atom);
 		return 1;			
 		}
 	}
 	//daca ajunge aici => nu e cuvant cheie deci e identificator
 	if( (identif[lenIdentif] = (char *)malloc(sizeof(atom)+1)) ==NULL)
 		{
 		perror("eroare alocare in fct: identifyStringAndSave");
 		return 0; 
 		}
 	strcpy(identif[lenIdentif], atom);
 	if( createAndSaveAtom(lenIdentif,2,lineNo, identif[lenIdentif])==0)
 		return 0;
 	lenIdentif ++;
 	//printf("\nsunt in functia identifyString %s\n",atom);
 	return 1; // succesful
 }
 
 
 /**
 @return  0 case of erors 
  		1 case identification was succesful
 */
 int calcAndSaveBaseInteger(char *atom,int len,int lineNo)
 {
 	int base=0;
 	int nr = 0;
 	int i =2 ,j;
 	base = atom[0]-'0';
 
 	if(atom[2]=='@')
 		{
 		base = base * 10 + atom[1] -'0';
 		i=3;
 		}
 
 	for ( j = i; j< len; j++)
 	{
 		if( base <=10)
 		{
 			if( atom[j] - '0' < base )
 				nr = nr *base + atom[j] - '0';
 			else
 			{
 		
 				return 0;
 			}
 		}
 		else
 		{
 			nr = nr * base;
 			if( isdigit(atom[j]) )
 				nr = nr + atom[j] - '0';
 			else
 				if( atom[j] -'A' >0 && atom[j] - 'A' < base)
 					nr = nr + atom[j] - 'A'+10;				
 				else 
 					return 0;
 		}
 	}
 	
 	//after calculating add it to the constInteget
 
 //	constIntreg[lenConstInt] = nr;
 	writeToConstInt(nr);
 	char nr_ch[20];
 	sprintf(nr_ch,"%d",nr);
 	if( createAndSaveAtom(lenConstInt,3,lineNo, nr_ch)==0)
 		return 0;
 	//lenConstInt++;
 	return 1;
 }//end calcAndSaveBaseInteger(atom, len, lineNo);
 
 int calcAndSaveNormalInteger(char* atom,int len,int lineNo)
 {
 	int i;
 	int nr = 0;
 	 
 	for ( i = 0; i < len ; i++)
 			nr = nr*10 + atom[i]-'0';
 			
 	//constIntreg[lenConstInt] = nr;
 	writeToConstInt(nr);
 	char nr_ch[20];
 	sprintf(nr_ch,"%d",nr);
 	if( createAndSaveAtom(lenConstInt,3,lineNo,nr_ch )==0)
 		return 0;
 //	lenConstInt++;
 return 1;
 }
 
 int interpretArray(char *atom,int len,int lineNo)
 {
 	
 	calcAndSaveNormalInteger(atom, len-1, lineNo);
 	if( createAndSaveAtom(4,7,lineNo,"..") ==0 ) // 4 == indexul in tabelul de delimitatori ai lui ..
 		return 0;
 	return 1;
 	
 }
 
 int identifyNormalReal(char* atom,int len,int lineNo)
 {
 	int i =0;
 	float nr=0;
 	float afterdot=0;
 	int lenAfterDot=0;
 	int exp =1;
 //	printf("\n\n\n %s %d\n\n",atom, len);
 	while (i<len && atom[i]!='.')
 	{
 	nr= nr*10+atom[i]-'0';
 	i++;
 	}
 	i++;
 	while(i<len && atom[i]!='E' && atom[i]!='e')
 	{
 		lenAfterDot ++;
 		afterdot = afterdot*10 + atom[i]-'0';
 		i++;
 	}
 	nr=nr+ afterdot/(lenAfterDot*10);
 //	printf("\n\n %lf\n",nr);
 	i++;
 	if(atom[i] == '-')
 	{
 		i++;
 		exp = -1;
 	}
 	else
 	if (atom[i] == '+')
 		 i++;
 	while(i<len)
 	{
 	nr = nr* pow(2,exp);	
 	i++;
 	}
 	writeToConstReal(nr);
 	char nr_char[20];
 	sprintf(nr_char,"%f",nr);
 	if( createAndSaveAtom(lenConstReal,4,lineNo,nr_char)==0)
 		return 0;
 //	printf("\n\n %lf\n",nr);
 	return 1;
 }
 
 /*TODO 
 	1. metodele ar trebui sa le fac cu reallocare de memori
 	2. ar trebui sa verific daca constanta e deja in table inainte sa o introduc si sa ii returnez indicele ? --sau mai mult pierd prin cautare?
 */
 
 
 void printfAtoms()
 {
 	struct atomi *temp;
 	temp = first;	
 	do
 	{
 		printf("line No : %d codLexical : %d atribut Id: %d val memorata:  %s \n",temp->lineNO,temp->codLexical, temp->atributId, temp->atom);
 		temp = temp->urm;
 	}while(	temp	!=	first);
 }//end printfAtoms
 
 
 void sapareteAtoms()
 {
 	// variable line number;
 	int lineNo= 0;
 	int state =0;
 	char tempChar;
 	char atom[100];
 	int len = 0;
 	int flagCharNotChecked =0; // 1= true (la starile finale ); 0 = false 
  	while( fscanf(f,"%c",&tempChar) !=EOF)
 	{
 	//printf("%c",tempChar);
 	do{ // ar trebui sa se execute maxim de 2 ori ... in momentul in care sunt in faze terminale 
 		flagCharNotChecked =0;
 		switch(state)
 			{
 			case 0 : // neglijez spatiile si raman in aceeasi stare  == avem si fct. isspace(char);
 					if(isspace(tempChar))
 					{
 						state = 0; //ramane pe 0; practic nu face nimic
 						if(tempChar == '\n') // new line 
 							lineNo ++;
 					}	
 					else
 						if( isalpha(tempChar))
 						{
 						  state  = 1;
 						  atom[len++] = tempChar;
 						}
 						else 
 							if(isdigit(tempChar))
 							{
 							  state = 3;
 							  atom[len++] = tempChar;
 							}	
 							else
 							if(tempChar=='{')
 							{
 							 state =2; // nu memorez commentul
 							}									
 							
 							else 
 								switch(tempChar)
 								{
 								case ';' :if(createAndSaveAtom(0,7,lineNo,";") ==0)
 										 {
 										 	printf("out of memory.Nu mai aloca pt ;");
 										 	exit(1);
 										 };break;
 								case ',' : if(createAndSaveAtom(1,7,lineNo,",") ==0)
 										 {
 										 	printf("out of memory.Nu mai aloca pt ,");
 										 	exit(1);
 										 };break;
 								case '.' : if( createAndSaveAtom(2,7,lineNo,".") ==0)
 										 {
 										 	printf("out of memory.Nu mai aloca pt .");
 										 	exit(1);
 										 };break;
 								case ':' :  state = 16;break;	
 								case '+' :if( createAndSaveAtom(0,0,lineNo,"+") ==0)
 										 {
 										 	printf("out of memory.Nu mai aloca pt +");
 										 	exit(1);
 										 };break;
 								case '-' :if( createAndSaveAtom(1,0,lineNo,"-") ==0)
 										 {
 										 	printf("out of memory.Nu mai aloca pt -");
 										 	exit(1);
 										 };break;									
 								case '*' :if( createAndSaveAtom(2,0,lineNo,"*") ==0)
 										 {
 										 	printf("out of memory.Nu mai aloca pt *");
 										 	exit(1);
 										 };break;										
 								case '/' :if( createAndSaveAtom(3,0,lineNo,"/") ==0)
 										 {
 										 	printf("out of memory.Nu mai aloca pt /");
 										 	exit(1);
 										 };break;									
 								case '(' :if( createAndSaveAtom(4,0,lineNo,"(") ==0)
 										 {
 										 	printf("Out of memory.");
 										 	exit(1);
 										 };break;											  	 
 								case ')' :if( createAndSaveAtom(5,0,lineNo,")") ==0)
 										 {
 										 	printf("Out of memory.");
 										 	exit(1);
 										 };break;	
 								case '[' :if( createAndSaveAtom(6,0,lineNo,"[") ==0)
 										 {
 										 	printf("Out of memory.");
 										 	exit(1);
 										 };break;											 										 
 								case ']' :if( createAndSaveAtom(7,0,lineNo,"]") ==0)
 										 {
 										 	printf("Out of memory.");
 										 	exit(1);
 										 };break;	
 								case '<' :state = 17;break;	
 								case '>' :state = 18;break;	
 								case '\'' :state = 19;break;
 								case '=' : if( createAndSaveAtom(13,0,lineNo,"=") ==0)
 										 {
 										 	printf("Out of memory.");
 										 	exit(1);
 										 };break;	
 								case '\"' :state =20; break;
 								}//end this switch 2					
 					//			ableOperatori[]={"+","-","*","/","(", ")","[","]","<","<=",">",">=","<>","=",":="} ;    
 					;break;//end case 0 :
 			case 1 : if( isdigit(tempChar) || isalpha(tempChar)) 
 					{
 					  state  = 1;
 					  atom[len++] = tempChar;
 					}
 					else
 					{	
 						
 						if(	identifyStringAndSave(atom,len,lineNo) ==  0  ) // eror
 						{
 							printf("eroare la identificare unui sir de caractere; line %d",lineNo);
 						}
 						flagCharNotChecked = 1; // caracterul citit nu e interpretat deci mai trecem prin switch inainte sa citim altul
 						state = 0 ;
 						len = 0;
 					}
 					;
 					break;
 			case 2 : if( tempChar == '}') //comment skip;
 						state = 0;
 					else
 						if( ! (isprint(tempChar) || isspace(tempChar) ) ) 
 							printf("unprintable char in comment");
 					;break;
 			case 3 : if(isdigit(tempChar))
 					 {
 					   atom[len++] = tempChar;
 					   state = 4;
 					 }
 					 else
 					   if(tempChar == '.')
 					   {
 					   	atom[len++] = tempChar;
 					   	state = 10;
 					   }
 					   else
 					 	 if( tempChar == '@')
 					 	 {
 					 	    atom[len++] = tempChar;
 					   		state = 5;
 					 	 }
 					 	 else
 					 	 {
 					 	 	flagCharNotChecked = 1;
 					 	 	state = 9;
 					 	 }
 			
 					;break;	
 			case 4 : if( isdigit(tempChar)  )
 					{
 						atom[len++] = tempChar; 
 						state = 8;						
 					}
 					else
 						if( tempChar =='@')		
 						 {
 						
 						 	atom[len++] = tempChar;
 						 	state = 5;
 						 }
 						 else 
 						   if( tempChar == '.')
 						   {
 						    atom[len++] = tempChar;
 						    state = 10;
 						   }
 						   else 
 						   {
 						    state = 9;
 						   	flagCharNotChecked = 1;
 						   }
 					;break;
 			case 5 : if( isdigit(tempChar) || ('A'<=tempChar && tempChar<='F'))
 					{
 						atom[len++] = tempChar;
 						state = 6;
 					}
 					else
 					{
 						printf("Bad format of integer Number : [2-16]@[0-9,A-F]* ; line %d",lineNo);
 						exit(1);
 					}
 					;break;
 			case 6 : if( isdigit(tempChar) || ('A'<=tempChar && tempChar<='F'))
 					{
 							atom[len++] = tempChar;
 						
 					}
 					else
 					{
 					
 						if( calcAndSaveBaseInteger(atom, len, lineNo) == 0)
 						{
 							printf("could not transform to integer; line %d ", lineNo); 
 							exit(1);
 						}
 						flagCharNotChecked = 1;
 						state = 0 ;
 						len = 0;
 							
 					}
 					;break;
 				case 7 : if( interpretArray(atom,len,lineNo) == 0)
 					{
 						perror("eror interpreting and array");	
 						exit(1);
 					}			
 					state = 0;
 					len =0;
 					;break;
 			case 8 : if(tempChar =='.')
 					{
 						atom[len++]='.';
 						state = 10;
 					}
 					else
 						if( isdigit(tempChar) )
 						{
 							atom[len++] = tempChar;
 						}
 						else
 						{
 							state = 9;
 							flagCharNotChecked = 1;				
 						}
 						 
 					;break;
 			case 9 : {
 						if(calcAndSaveNormalInteger(atom,len,lineNo) ==0)
 						{
 							printf("Eror at saving integer");
 							exit(1);
 						}
 						flagCharNotChecked = 1;
 						state = 0;
 						len = 0;
 					 };break;
 			case 10 : if(tempChar == '.')
 					{
 						state = 7;
 						flagCharNotChecked = 1;				
 					}
 					else
 					 if(isdigit(tempChar))
 					 {
 					 	atom[len++] = tempChar;
 					 	state = 11;
 					 }
 					 else
 					  printf("wrong format input; line : %d", lineNo);
 					;break;
 			case 11 : if( isdigit(tempChar ))
 						atom[len++] = tempChar;
 					  else 
 					   if( tempChar == 'E' || tempChar == 'e')
 					   {
 					   	atom[len++] = tempChar;
 					   	state =12;
 					   }			
 					   else
 					    if(identifyNormalReal(atom,len,lineNo) == 0)
 					    	printf("wrong real format.Could not identify; line : %d", lineNo);
 					    else
 					    {
 					    	state =0;
 					    	len =0;
 					    	flagCharNotChecked = 1;				
 					    }	
 					;break;
 			case 12 :if( tempChar =='+' || tempChar =='-')
 					{
 						atom[len++] = tempChar;
 						state =13;
 					}
 					else
 					if(isdigit(tempChar))
 					{
 						atom[len++] = tempChar;
 						state = 14;
 					}
 					else
 					{
 						printf("Bad real reprezentation; line %d",lineNo);
 						exit(1);
 					}
 					;break;
 			case 13 :if(isdigit(tempChar))
 					{
 						atom[len++] = tempChar;
 						state = 14;
 					} 
 					else
 					{
 						printf("Bad real reprezentation; line %d",lineNo);
 						exit(1);										
 					}
 					;break;
 			case 14 :if(isdigit(tempChar))
 					{ 
 						atom[len++] = tempChar;
 						state = 15;
 					}
 					else
 					{
 						state = 15;
 						flagCharNotChecked = 1;				
 					}
 					;break;
 			case 15 :if(isdigit(tempChar))
 					{
 						printf("prea multe cifre la exponent; line %d\n",lineNo);
 						exit(1);
 					}
   					 if(identifyNormalReal(atom,len,lineNo) == 0)
 					    	printf("wrong real format.Could not identify; line : %d", lineNo);
 					    else
 					    {
 					    	state =0;
 					    	len =0;
 					    	flagCharNotChecked = 1;				
 					    }	
 					 ;break;
 			case 16 : if(tempChar =='=')
 						{
 						if(createAndSaveAtom(14,0,lineNo,":=") ==0)
 							{
 							 	printf("out of memory.Nu mai aloca pt :=");
 							 	exit(1);
 							}
 						state =0;
 						}
 						else
 						{
 							if(createAndSaveAtom(3,7,lineNo,":") ==0)
 							{
 							 	printf("out of memory.Nu mai aloca pt :");
 							 	exit(1);
 							 }
 						state =0;
 						flagCharNotChecked = 1;				
 						}
 					;break; //{"+", "-" ,"*"2 ,"/" ,"(" 4,  ")","["6,   "]","<"8   ,"<=",">" 10 ,">=","<>" 12  ,"=",":=" 14} ;    
 			case 17 : if(tempChar== '=' )
 						{
 						   if( createAndSaveAtom(9,0,lineNo,"<=") ==0)
 							 {
 							 	printf("Out of memory.");
 							 	exit(1);
 							 }	
 						state = 0;
 						}
 						else 
 							if(tempChar == '>')
 							{
 						
 							   if( createAndSaveAtom(12,0,lineNo,"<>") ==0)
 								 {
 								 	printf("Out of memory.");
 								 	exit(1);
 								 }	
 							state = 0;
 							}
 							else
 							{							
 							   if( createAndSaveAtom(8,0,lineNo,"<") ==0)
 								 {
 								 	printf("Out of memory.");
 								 	exit(1);
 								 }	
 								state = 0;
 								flagCharNotChecked = 1;		
 							}	
 					;break;
 			case 18 : if(tempChar== '=' )
 						{
 						   if( createAndSaveAtom(11,0,lineNo,">=") ==0)
 							 {
 							 	printf("Out of memory.");
 							 	exit(1);
 							 }	
 						state = 0;
 						}
 						else
 							{							
 							   if( createAndSaveAtom(9,0,lineNo,">") ==0)
 								 {
 								 	printf("Out of memory.");
 								 	exit(1);
 								 }	
 								state = 0;
 								flagCharNotChecked = 1;		
 							}
 					 ;break;
 			case 19 : if(isprint(tempChar) || tempChar ==' ')
 			        {
                       atom[len++] = tempChar;
                       atom[len] = '\0';
                       state  = 21;
                     }
                     ;break;
 			case 20 :if(tempChar == '\"')
                         {
                          atom[len] ='\0';
                          //mem la constString si adaug la atom           
                         writeToConstString(atom);
                         createAndSaveAtom(lenConstString-1,6,lineNo,atom);
                         state = 0;
                         len = 0;
                         }else
                             if(isprint(tempChar) || tempChar ==' ' )
     			            {
                             atom[len++] = tempChar;
                             }
                      ;break;
             case 21 :if( tempChar=='\'')
 			        {
                      //memorez char in const si mem atom     
                      writeToConstChar(atom[0]);
                      char ch[2];
                      sprintf(ch,"%c",atom[0]);
                      createAndSaveAtom(lenConstChar-1,5,lineNo,ch);
                      state = 0;
                      len = 0;
                     }
                     else
                     {
                         printf("That is not a char. To many caracters. line %d",lineNo);
                         exit(1);
                     }
                     ;break;
             default : printf("Not a treated case of input; ");
 			}		
 			printf("line No : %d state : %d\n",lineNo,state);  
 		}while(flagCharNotChecked == 1);
 	}//end while de citire;
 	if(state !=0) 
 	{       
              if(state== 2)
                  printf("comentariu nu a fost inchis");
              else
                  printf("eroare.Nu s-a terminat in starea 0.");
              exit(1);
     }
 }
 
