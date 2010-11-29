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
 
 
 //verifica daca tipul trimis e acceptat cu tipul memorat pana acum in variabila tip;
int typeMatch(int left)
{
 		if(left >= tip) // corect
		 		return 1;

    return 0;
}

/**
modifica tipul daca e nevoie. Exemplu float si era int ---> tip devine float
se merge pe tipurile din structura TS :
	 0 = char  	 1 = integer 		2 = real		
	 
in structura atom avem : 3 = int 4 = real 5 = char
cele din atom se transforma  cu formula : (tip - 2) %3 

*/
void typeCalc(int urm)
{
 		 if(urm > 2)
 		  			urm = (urm -2) %3;
 		if( tip < urm)
 				tip = urm;
    

}
 //find an atom in the TS;
int checkAprear(char nume[])
{
 		int i;
 		for(i = 0;i<dimts; i++)
 				if(strcmp(ts[i].nume, nume)==0 )
				    return i;

return -1;
}

  //verific daca am dubluri in TS si printez eroare 
int  checkDuplicaTS()
{
  int i;
  for(int i = 0; i< dimts; i++)
  for (int j = i+1; j< dimts; j++)
   if(strcmp(ts[i].nume, ts[j].nume)==0 )
   {
      printf("\nDouble declaration of variable %s ! \n\n",ts[i].nume );
      return 0;
   }
   return 1;
}

 
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

/**
return 1 : if it is true, it's a simple atom
return 0 : if not
*/
int tipSimplu(struct atomi* curent)
{
  if((curent->codLexical == 1 && curent->atributId ==13)||(curent->codLexical == 1 && curent->atributId ==3) || (curent->codLexical == 1 && curent->atributId ==20) )// integer || real || char
      return 1;  //true                     
 return 0;   
}//final & finished

/**
return 1 : if it is true, it's a simple atom
return 0 : if not
*/
int opRel(struct atomi* curent)
{
  if(curent->codLexical == 0 && (curent->atributId >=8 && curent->atributId <=13) )// <||> || <= || => || <> || =
      return 1;  //true                     
 return 0;   
}//final & finished

/**
return 1 : if it is true, it's a simple atom
return 0 : if not
*/
int sens(struct atomi* curent)
{
  if(curent->codLexical == 1 && (curent->atributId ==28 && curent->atributId ==7) )// to | downto
      return 1;  //true                     
 return 0;   
}//final & finished


/**return 1 : if it is true, it's a simple atom
return 0 : if not
*/
int opLog(struct atomi* curent)
{
  if((curent->codLexical == 1 && curent->atributId ==17)||(curent->codLexical == 1 && curent->atributId ==0) )// and || or
      return 1;  //true                     
 return 0;   
}//final & finished


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

struct atomi* tipTablou(struct atomi* curent)
{
 			 int min,max,tip;
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
                        min = constIntreg[ curent->pre->atributId -1];     
                        curent = expresieStatica(curent->urm);                   
 
                        if(curent== NULL)
                        {
                          printf("Not a normal second static expresion\n");
                          return NULL;
                         }
                        else
                          if(curent->codLexical == 0 && curent->atributId ==7) //]
                          {
													     max = constIntreg[ curent->pre->atributId -1];
                              curent = curent->urm;
                              if(curent->codLexical == 1 && curent->atributId ==16) // of
                              {
                                 curent = curent->urm;
                                 if(tipSimplu(curent)  == 1)
                                  {
																	
                                  printf("good array\n %d %d  %d %d",min,max,dimts,dim_parent);
                                  tip = curent->atributId/10;
                                  
																  for(int i = dim_parent; i<dimts; i++)
                                  {
																	 	ts[i].adrel = adrel;
																		 adrel = adrel + max-min+1;
																	 	ts[i].min = min;
																	 	ts[i].max  = max;

																	}
																//	dimts++;
																	
                                  
                                  printTS();
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
/**
first atom : should be an identificator
return : first atom after the simple type 
*/

struct atomi* declSimpla(struct atomi* curent)
{
     //  printf("\n\n%s\n",curent->atom);
 if(curent->codLexical == 2) // identificator     
  {
	 strcpy(ts[dimts].nume, curent->atom);
	 if(dim_parent)
	 {ts[dimts].nivel = nivel;
	 ts[dimts].adrel = adrel++;  //adrel++;                             
	 }
	 curent = curent->urm;
   int start;
   start = dimts;
   dimts++;
  
       if(curent->codLexical == 7 && curent->atributId ==1) //,
       {
			 		
					  curent = curent->urm; 
            
						while(curent->codLexical == 2) //identificator
            {
					
			 			strcpy(ts[dimts].clasa, ts[dimts-1].clasa); // e ca anteriorul :parametru / camp
			 		  if(dim_parent)
					 	{
					   ts[dimts].nivel = nivel;
						 ts[dimts].adrel = adrel++; 
            }
						strcpy(ts[dimts].nume, curent->atom);
						for(int k =0; k< ts[dimts-1].nr_lista_rec; k++)
           	ts[dimts].lista_rec[k] = ts[dimts-1].lista_rec[k];
           	ts[dimts].nr_lista_rec = ts[dimts-1].nr_lista_rec;
						 dimts++;

            curent = curent->urm; 
             if(!(curent->codLexical == 7 && curent->atributId ==1) )//,
             			break;									 
						curent = curent->urm;  
           }
       }
       if(curent->codLexical == 7 && curent->atributId ==3) //:
       {
              curent = curent->urm;             
               if( tipSimplu(curent) == 1)
               {
							 	//printf("\n\n\nsunt %d parametri de tip : %s\n",(dimts-start+1),curent->atom);
								//printTS();
								 for(int i = start; i<dimts;i++)
								 {
								 				 //le pun tipul in tabela si le adaug si in procedura daca dim_parent_ nu 0

												 ts[i].tip = curent->atributId/10 ;			
												 if(dim_parent )
												  {	
														
														ts[dim_parent].lista_param[0][ts[dim_parent].nr_par] = ts[i].tip;
														ts[dim_parent].lista_param[1][ts[dim_parent].nr_par] = 0;	
														ts[dim_parent].nr_par = ts[dim_parent].nr_par + 1;
														
													//	printf("\n\n\nsunt  parametri de tip : %d %d\n",	ts[dim_parent].lista_param[0][ts[dim_parent].index_lista] = ts[i].tip,ts[dim_parent].index_lista++);
													}		 				 
								 }	 
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
   int i=0,j;
    struct atomi* temp;
   do
   {
       temp = curent;
       i++;
       strcpy(ts[dimts].clasa,"camp");
 			 if(!(strcmp(ts[dimts-1].clasa,"camp") ==0))
				 for( j = dim_parent; j< dimts; j++)
				 {
				 		ts[dimts].lista_rec[ts[dimts].nr_lista_rec]=j; 
						ts[dimts].nr_lista_rec = ts[dimts].nr_lista_rec + 1;
					}	
				else
				{

				 	 for( j = 0; j< ts[dimts-1].nr_lista_rec; j++)
				 {
				 		ts[dimts].lista_rec[j]=ts[dimts-1].lista_rec[j]; 
					
					}
					 ts[dimts].nr_lista_rec =  ts[dimts-1].nr_lista_rec;	
	 			}
				dim_parent = 0;
			 
			 curent = declSimpla(curent);

			// ts[dimts-1].adrel =-2;
			 //adrel --;
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
  {
	 	for(int i = dim_parent; i<dimts;i++)
	 	 {
		 			ts[i].tip = curent->atributId/10;
		 			ts[i].adrel = adrel++;
		 }		
		return curent->urm;     
 }
 else
     if((curent->codLexical == 1 && curent->atributId ==26)) //record
       {
 				  int temp = dim_parent;
				
          printf("record type found\n");
      	 
				  ts[dimts].nr_lista_rec = 0;
				  int aux = dimts;
					curent = listaCamp(curent->urm);
          
						for(int i = temp; i<aux;i++)
					 	 {
 						 	strcpy(ts[i].clasa,"struct");
 						 	ts[i].adrel = adrel+1;
 						 	adrel+=dimts-aux;
							}	
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
				 	 	for(int i = dim_parent; i<dimts;i++)
							 	 {
		 						 	strcpy(ts[i].clasa,"tablou");// = curent->atributId/10;
		 							ts[i].adrel = adrel;
		 							
		 							}		
											 
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
	 dim_parent = dimts;											
   strcpy(ts[dimts].clasa,"variabila");
   strcpy(ts[dimts].nume, curent->atom);
   ts[dimts].nivel = nivel;
   	dimts++;
	 curent = curent->urm;
   if(strcmp(curent->atom,"j")==0)
   {
                                                                    
                                  
					 printf("\n\n\naaaa\n\n\n\n"); printTS();
	 }
       if(curent->codLexical == 7 && curent->atributId ==1) //,
       {
            curent = curent->urm; 
   				
	          while(curent->codLexical == 2) //identificator
            {
                strcpy(ts[dimts].clasa,"variabila");
							  strcpy(ts[dimts].nume, curent->atom);
  				      ts[dimts].nivel = nivel;
					      dimts++;
					      
								curent = curent->urm; 
                  if(!(curent->codLexical == 7 && curent->atributId ==1)) //,
        									break;
				          curent = curent->urm;  
           }
       }
       if(curent->codLexical == 7 && curent->atributId ==3) //:
       {
              curent = curent->urm;             
               if( (curent->codLexical == 1 && curent->atributId ==26) || (curent->codLexical == 1 && curent->atributId ==13)||(curent->codLexical == 1 && curent->atributId ==27) || (curent->codLexical == 1 && curent->atributId ==3) || (curent->codLexical == 1 && curent->atributId ==20) )// integer || real || char || array || record
               {
                curent = tipVar(curent);
               
                dim_parent = 0;
                
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

/**
first atom after var key word
*/

struct atomi* sectiuneVar(struct atomi* curent)
{
  printf("SectiuneVar \n first atom : %s\n", curent->atom); 
  struct atomi* temp =curent;
  curent =  declarVar(curent); 
  if(curent ==NULL )
  {
     printf("first atom isn't an variable decalration\n");
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

struct atomi* declarPar(struct atomi* curent)
{
 			 	 printf("declarPar\n"); 	
 			 if(curent->codLexical ==1 && curent->atributId==24  )// var
			    curent = curent->urm;
    	  	
					strcpy(ts[dimts].clasa,"parametru");
				 curent = declSimpla(curent);
			 
				if(curent ==NULL)
						  return NULL;	
				return curent;
}
/**
first atom : should be var | an identifier for the declar simplu 
return : ) atom if corect or NULL if errors
*/
struct atomi* listaParamForm(struct atomi* curent)
{	
 			
			 printf("listaParamForm\n");
			 	curent = declarPar(curent);
					if(curent == NULL)
						  return NULL;	
				
				struct atomi* temp;
				temp = curent;

			 while(curent->codLexical ==7 && curent->atributId==0  )// ;
       { // ve have more than 1 var
			 				   curent = curent->urm;
					 temp = curent;
					curent = declarPar(curent);
					if(curent == NULL)
					{			
			     return temp;		
			    }
			    temp = curent;
			 }		
			 	
			 return curent;
}

/** first atom is ( or can be epsilon 
returned : first atom afte ) or curent if it doesn't start with (
return: NULL it exists and has errors;
*/   
struct atomi* paramForm(struct atomi* curent)
{
        if(!(curent->codLexical ==0 && curent->atributId==4 ) )// (
        return curent; // it's epsilon case --- ok
        curent = curent->urm;      
								 												
				curent = listaParamForm(curent);
				if(curent ==NULL)
						  return NULL;		        
							  
        if(!(curent->codLexical ==0 && curent->atributId==5 ) )// )
       {
							printf("No ) at end of parameters declaration. Line : %d\n",curent->lineNO);
							return NULL;
       }
       			
       return curent->urm;
}

/** first atom is the frist one after function key word 
returned : ':' atom if corect
*/   
struct atomi* antetSubprog(struct atomi* curent)
{
       printf("antet subprogram\n");
       if(!(curent->codLexical == 2 )) // identificator     
       {
         printf("No identificator in antet subprogram. Line : %d\n",curent->lineNO);
         return NULL;
       }
       
       strcpy(ts[dim_parent].nume, curent->atom); /// copiez numele procedurii 
       
			 curent = curent->urm;
			 
       curent = paramForm(curent); 
     
			 return curent; //NULL or : | ; should be
}

/**first atom is the first one after procedure key word
returned : first atom after ; 
*/
struct atomi* declarProcedure(struct atomi* curent)
{
       printf(" declarProcedure\n");
      
			 strcpy(ts[dimts].clasa,"procedure");
       ts[dimts].nivel  = nivel-1;
       ts[dimts].adr_start = -1;
       ts[dimts].incdom = -1;
       ts[dimts].nr_par = 0;
			 dim_parent = dimts;      
 			
			  dimts++;
			 int adrel_temp  = adrel;
       adrel = 0;
			 curent = antetSubprog(curent);
    	 
			 ts[dim_parent].dim_var = adrel; 
			 dim_parent=0;
    	 adrel = adrel_temp;
       
			 if (curent == NULL)
         return NULL;
         
       curent =checkBlock(curent);
       if (curent == NULL)
         return NULL;       
       
       if(!(curent->codLexical ==7 && curent->atributId==0 ) )// ;
       {
        printf("No ; after block. Line %d . We have %s\n",curent->lineNO, curent->atom);
        return NULL;
       }
       
       
       return curent->urm;//firs one after ; 
}

/** first atom is the frist one after function key word 
returned : first atom after ; 
*/      
struct atomi* declarFunction(struct atomi* curent)
{ 
       printf(" declarFunction\n");

       strcpy(ts[dimts].clasa,"function");
       ts[dimts].nivel  = nivel-1;
       ts[dimts].adr_start = -1;
       ts[dimts].incdom = -1;
       dim_parent = dimts;  
       ts[dimts].nr_par = 0;
		
			 dimts++;
       int adrel_temp  = adrel;
       adrel = 0;
			 curent = antetSubprog(curent);
		
			 ts[dim_parent].dim_var = adrel; 
			 dim_parent=0;
			 adrel = adrel_temp;
     
		   if (curent == NULL)
         return NULL;
         
       if(!(curent->codLexical ==7 && curent->atributId==3 ) )// :
       {
        printf("No : after antet. Line %d \n",curent->lineNO);
        return NULL;
       }
       curent = curent->urm;

       if( tipSimplu(curent) == 0) //false
       {
        printf("No simpe type after : Line %d. We have %s \n",curent->lineNO, curent->atom);
        return NULL;
       }
       curent = curent ->urm;
        
      
			 if(!(curent->codLexical ==7 && curent->atributId==0 ) )// ;
       {
        printf("No ; after declared type in function. Line %d We have %s  \n",curent->lineNO, curent->atom);
        return NULL;
       }
       curent = curent->urm;
       
            
			 curent =checkBlock(curent);
       if (curent == NULL)
         return NULL;       
       
       if(!(curent->codLexical ==7 && curent->atributId==0 ) )// ;
       {
        printf("No ; after block. Line %d We have %s  \n",curent->lineNO, curent->atom);
        return NULL;
       }
       
       
       return curent->urm;//firs one after ;
}     

/** curent : is the function  | procedure key word 
returned : first atom after ; 
*/  
struct atomi* declarSubprogram(struct atomi* curent)
{
 printf("declar subprogram\n");
 
      if(curent->codLexical ==1 && curent->atributId==18 ) //procedure
     {
		  nivel ++;
		  curent =declarProcedure(curent->urm);
			printTS();
			removeLevel(nivel);
			nivel --;
			}
			
			if (curent == NULL)
         return NULL;
      if(curent->codLexical ==1 && curent->atributId==11 ) //function
      {
			nivel++;
			curent = declarFunction(curent->urm);
			printTS();
			removeLevel(nivel);
			printTS();
			nivel --;
			}
			if (curent == NULL)
         return NULL;
      return curent;
 return NULL;
}
      
struct atomi* sectiuneDeclSubprog(struct atomi* curent)
{

  printf("SectiuneDeclSubprog %d\n",curent->urm->lineNO); 
   struct atomi* temp =curent;
   if(curent->codLexical ==1 && (curent->atributId==18 || curent->atributId==11)) // procedure | function
		 curent =  declarSubprogram(curent); 
  if(curent ==NULL )
  {
     printf("first atom isn't a valid function declaration\n");
     return NULL;
  }else
       {
   
         printf("returned  atom : %s\n",curent->atom);    
        temp = curent;     
      while( curent->codLexical ==1 && (curent->atributId==18 || curent->atributId==11) && ( curent = declarSubprogram(curent)) !=NULL) 
			   temp = curent;
        return temp;
       }
 
  
}


struct atomi* listaExpresii(struct atomi* curent)
{	
			
 			 do
 			 {
			 	 curent = expresie(curent);
			 	 if(curent == NULL)
			 	    return NULL;
 	   			if(! (curent->codLexical == 7 && curent->atributId ==1))	// ,
			 				 return curent;
 				 curent = curent->urm;
			 }while(1);

 return NULL;
}


struct atomi* factor(struct atomi* curent)
{
 	         if(!(curent->codLexical == 0 && curent->atributId ==4 || (curent->codLexical == 2 || constanta(curent) ==1 ))) //( || indent || constanta
           {                    
             printf("Not a corect factor start. Line %d We have %s\n",curent->lineNO, curent->atom);
						 return NULL;           
						 }

            if(curent->codLexical == 2 )
						{
						 	curent = curent->urm;
						 	if( !((curent->codLexical == 0 && curent->atributId ==4)) || (curent->codLexical ==7 && curent->atributId==2 ) || (curent->codLexical ==0 && curent->atributId==6)) // ( || . || [
						 	{	
							
								int index = checkAprear(curent->pre->atom);
								if(index == -1)
												 {
												 	 printf("Undeclared variable %s at line %d",curent->pre->atom,curent->pre->lineNO);
												 	 return NULL;
												 }
								typeCalc( ts[index].tip);
								printf("\n\nindex %d tipul nou %d\n\n", index,tip);
								getchar();
								return curent;// just a simple identificator;										
						  }	
							if(curent->codLexical == 0 && curent->atributId ==4)	// (
							{
							     
									 curent= listaExpresii(curent->urm);					
									 if(curent ==NULL)
							 					 return NULL;
								   if(curent->codLexical == 0 && curent->atributId ==5) //)
													 return curent->urm;
									 printf(") missing. Line %d we have %s\n",curent->lineNO, curent->atom);
									 return NULL;
							 }
							 
							 	if(curent->codLexical == 7 && curent->atributId ==2)	// .
							{
							 	curent = curent->urm;
							 	   if(curent->codLexical == 2 )
							 	   {
									 				
												  int index = checkAprear(curent->atom);
													if(index == -1)
																	 {
																	 	 printf("Undeclared variable %s at line %d",curent->atom,curent->lineNO);
																	 	 return NULL;
																	 }
													typeCalc( ts[index].tip);
													printf("\n\nindex %d tipul nou %d\n\n", index,tip);
													getchar();								 
							 				 return curent->urm;
					 	       
									 }
									 printf("Missing indetifier after. at line %d\n",curent->lineNO);
									 return NULL;
							}
							
								if(curent->codLexical == 0 && curent->atributId ==6)	// [
							{
							     curent = expresie(curent->urm);
							 		 if(curent ==NULL)
							 					 return NULL;
									  if(curent->codLexical == 0 && curent->atributId ==7) //]
													 return curent->urm;
									 printf("] Missing. Line %d\n",curent->lineNO);	
									 return NULL;		 
							}
							 
						}  
  				
						if( constanta(curent) ==1  ) // constanta
							{
						//	 printf("const %s\n",curent->atom);
								  typeCalc( curent->codLexical);// schimb type-ul
								  printf("line ~ 950 type %d dupa constanta %s\n", tip, curent->atom);
								  getchar();
							 		return curent->urm;
							}
							
						if(curent->codLexical == 0 && curent->atributId ==4) //(
						{
						 curent = curent->urm;
						 
					   curent = expresieStatica(curent);
					   if(curent == NULL)
					    return NULL;
				    
				    if(curent->codLexical == 0 && curent->atributId ==5) //)
							 return curent->urm;
						printf(") Missing. Line %d we have %s\n",curent->lineNO, curent->atom);
						return NULL;
						}

 return NULL; //daca se ajunge aici e eroare de programare
}

/**
firs atom after := if in variable declaration or other stuf == firs atom in the expresion
return : firs atom after expresie
*/
struct atomi* expresie(struct atomi* curent)
{
 	           // printf("expresie \n");
							struct atomi* temp;
							do
							{
							temp = curent;
  						
  						curent = factor(curent);
  						if(curent == NULL)
  											return NULL;
  											
  					  if( !( opAdd(curent) || opMul(curent) )) 
  						{
							
							 		return curent;
					 		}
							curent = curent->urm;
							
							}while(1); //do until no return is found
  						
    return NULL; // it will never get here;
}

/**
first atom : identificator
return := or NULL
*/
struct atomi* variabila(struct atomi* curent)
{
 		 if(!(curent->codLexical == 2 )) // identificator  
			return NULL;
		 
		 	
			  int index = checkAprear(curent->atom);
				if(index == -1)
								 {
								 	 printf("Undeclared variable %s at line %d\n",curent->atom,curent->lineNO);
								 	 return NULL;
								 }
				typeCalc( ts[index].tip);
				printf("\n\nindex in TS  %d pt var %s. Tipul nou %d\n\n", index,curent->atom,tip);
				getchar();								 

		 curent = curent->urm;
		 
		 if(! ((curent->codLexical ==0 && curent->atributId==6 ) || (curent->codLexical ==7 && curent->atributId==2 )))// [ sau .
		 return curent;
		 
		 if(curent->codLexical ==0 && curent->atributId==6 ) //[
		 {
		 			curent = curent->urm;								 
		     curent = expresie(curent);
		     if((curent->codLexical ==0 && curent->atributId==7 )) //]
		     		return curent->urm;
				 printf("No ] at line %d\n",curent->lineNO);
				 return NULL; 
		 }

 		 if( curent->codLexical ==7 && curent->atributId==2 )//.
 		 {
				 if((curent->codLexical == 2 )) // identificator  
				 				 return curent->urm;
				 printf("No identificator.identificator at line %d\n",curent->lineNO);
				 return NULL;
		 }	 
		 
 return NULL;
}

/**
first atom : identificator 
return : curent if NOT an instAtrib, NULL if it is and it's incorect or ; if it's ok
*/
struct atomi* instrAtrib(struct atomi* curent)
{
 			 int left, right;
 			 printf("instr->instr atribuire\n");
  	 if(!(curent->codLexical == 2 )) // identificator     
	     return curent;

	     // daca ajunge aici inseaman ca avem o instr atribuire
			 		tip = -1; // initializam tipul;
			 	 curent = variabila(curent);
			 	 if(curent == NULL)
			 	  return NULL;
			 	  left = tip;

			 if(!(curent->codLexical ==0 && curent->atributId==14 ) )// :=
       {
        printf("No := after variabila . Line %d \n",curent->lineNO);
        return NULL;
       }
       curent = curent->urm;
			 
			 tip = -1; // initializez tipul din nou
       curent= expresie(curent);
       if ( typeMatch(left) ==0 ) // nu e bun 
       {
			 		printf("\nIncorect types. left  %d right %d. Line %d \n",left,tip,curent->lineNO);
					getchar();
					return NULL;
		 	 }else
		 	 {printf("corect type  left  %d right %d. \n",left,tip);
			 getchar();
			 }
       return curent; // poate fi NULL sau altul
}

struct atomi* exprRel(struct atomi* curent)
{
 			 	if(curent->codLexical == 0 && curent->atributId ==4) //(
						{
						  curent = curent->urm;
						  curent= conditie(curent);
						   if(curent == NULL)
 			 					 return NULL;
 			 						if(curent->codLexical == 0 && curent->atributId ==5) //) 
										  return curent; //
									printf(") missing at line nr : %d. we have %s\n",curent->lineNO,curent->atom);
									return NULL;		
						}
				curent = expresie(curent);
			 if(curent == NULL)
 			 					 return NULL;
 			
			  if( opRel(curent) == 0 )//false
 			 {
			 		 printf("missing realtional op line %d. We have %s",curent->lineNO,curent->atom);
			 		 return NULL;
			 }				 
 			 curent = curent->urm;
				
				curent = expresie(curent);
				return curent; // can be NULL						 
}

/**
first atom : fist atom of exprRel type
*/
struct atomi* exprLogica(struct atomi* curent)
{
 			 curent = exprRel(curent);
			 if(curent == NULL)
 			 {
			 					 printf("Not a valid first logical expresion.");
			  				 return NULL;
			 }
			 
			 if(opLog(curent) == 0) //false
			  return curent; // only one 
			 curent = curent ->urm;
			 
			 do
			 {
		 	   curent = exprRel(curent);
				 if(curent == NULL)
	 			      return NULL;
     					
					if(opLog(curent) == 0) //false
						return curent; //it's ok and finished
					
					curent = curent->urm;			 		
			 }while(1);

}

/**
first : not | expresie
*/
struct atomi* conditie(struct atomi* curent)
{
   if(curent->codLexical ==1 && curent->atributId==15 )//not
		 curent= curent->urm;
	 
	 curent =	exprLogica(curent); 
	 return curent;// can be null
}

/**first atom after if
*/
struct atomi* instrIf(struct atomi* curent)
{
 			 curent = conditie(curent);
 			 if(curent == NULL)
 			 					 return NULL;
 			
			  if(!(curent->codLexical ==1 && curent->atributId==22))//then
			  {
					printf("then key word missing. Line %d we have %s",curent->lineNO,curent->atom);
					return NULL;
				}
				
				curent = instr(curent);
				if(curent == NULL)
 			 		 return NULL;
 			 		 
	 		 if(curent->codLexical ==1 && curent->atributId==8)//else
	     {
					 struct atomi* temp;
					 temp = curent->urm;
			 	   curent =  instr(curent->urm);
			 	   if(curent== temp) 
					 {
					 	   printf("Invalid instruction in else. Line nr %d. We have %s\n",curent->lineNO, curent->atom);
		 			 	   return NULL;
					 }
			 }
			  if(curent == NULL)
 			 					 return NULL;
   
	 			 return curent;
}
/**
first after while
*/
struct atomi* instrWhile(struct atomi* curent)
{
 			 curent = conditie(curent);
 			 if(curent == NULL)
							 return NULL;
		  
			if(!(curent->codLexical ==1 && curent->atributId==6))//do
			{
			 			printf("do missing. Line %d . we have %s \n",curent->lineNO,curent->atom);										
			 			return NULL;
			}					 
			curent = curent->urm;
			
			 struct atomi* temp;
			 temp = curent;
	 	   curent =  instr(curent);
	 	   if(curent== temp) 
			 {
			 	   printf("Invalid instruction in while. Line nr %d. We have %s\n",curent->lineNO, curent->atom);
 			 	   return NULL;
			 }
			 
 			 return curent;
 			 
}
/**
first atom : first after begin
return :  ; if corect or NULL - esec
REmembrer : Toate instr returneaza null doar daca au identificat primul atom ca ar apartine lor si dupa e gresit
 daca primul atom nu e a lor returneaza curent (ce o primit)
*/
struct atomi* instr(struct atomi* curent)
{
 			 //instrAtrib
 			 curent = instrAtrib(curent); 
 			 if(curent == NULL)
			 					 return NULL;
								 
				//instrComp				 
		    if(curent->codLexical ==1 && curent->atributId==1)//begin
	      {
				printf("instr->instr compusa\n");
				 curent = instrComp(curent->urm); // tre sa primeasca primul atom dupa begin 
				 if(curent == NULL)
				  return NULL;
			   return curent;
	      }
	      
	      //instrif
		    if(curent->codLexical ==1 && curent->atributId==12)//if
	      {
  				printf("instr->instr if\n");
					curent = instrIf(curent->urm);  
				  if(curent == NULL)
				   return NULL;
					return curent;    
				} 
				
				//while instr
				 if(curent->codLexical ==1 && curent->atributId==25)//while
	      {
						 printf("instr->instr while\n");
				 	curent = instrWhile(curent->urm);  
				  return curent; //can be null
				}
	 			 //TODO next instructuini
 			 return curent;
}

/**
first atom : first after begin
return : end if corect or NULL - esec
*/
struct atomi* listaInstr(struct atomi* curent)
{
printf("SectiuneDeclSubprog %d\n",curent->urm->lineNO); 
   struct atomi* temp =curent;
   //if(curent->codLexical ==1 && (curent->atributId==18 || curent->atributId==11)) // procedure | function
		 curent =  instr(curent); 
  if(curent ==NULL || curent == temp) // nu s-a progresat
  {
     printf("first atom isn't a valid instruction declaration\n");
     return NULL;
 
	}
	 else
       {
	   	 if(!(curent->codLexical ==7 && curent->atributId==0 ) )// ;
	       {
	        printf("No ; after instruction. Line %d We have %s  \n",curent->lineNO, curent->atom);
	        return NULL;
	       }
	       curent = curent->urm;
        // printf("returned  atom : %s\n",curent->atom);    
        temp = curent;     
      while( ( curent = instr(curent)) !=NULL && curent != temp)  // s-a progresat
			  {
							 if(!(curent->codLexical ==7 && curent->atributId==0 ) )// ;
				       {
				        printf("No ; after instruction. Line %d We have %s  \n",curent->lineNO, curent->atom);
				        return NULL;
				       }
				       curent = curent->urm;
						   temp = curent;
		    }
				return temp;
       }
 
}

/**
first atom after begin
return : . atom if corect
*/
struct atomi* instrComp(struct atomi* curent)
{
       printf("InstrComp. First atom : %s %d\n",curent->atom, curent->lineNO); 
      
			curent = listaInstr(curent);
			if(curent == NULL)
								return NULL;
		
		   if( curent!=NULL && curent->codLexical ==1 && curent->atributId==9)//end
			 return curent->urm;
			return NULL;
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
          
					ts[0].dim_var = ts[dimts-1].adrel ; // pun la program dimensiunea variabilelor;
					printTS();
					
				  printf("first atom after sectiune var : %s\n ", curent->atom);
      }
       if(curent!=NULL && curent->codLexical ==1 && (curent->atributId==18 || curent->atributId==11)) // procedure | function
      {//SectiuneDeclSubprog  
          curent = sectiuneDeclSubprog(curent); // nu trec la urm atom
         if(curent == NULL) // eroare in functia sectiuneConst
            return NULL;
        //printf("subprogram\n");
      }
      if(curent!=NULL && curent->codLexical ==1 && curent->atributId==1)//begin
      {//InstrComp - begin
        printf("first atom in sectiune begin : %s %d\n ", curent->atom, curent->lineNO);
         curent =  instrComp(curent->urm);        
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
 return curent; // TODO : delete after instrComp is implemented
 }           
 void checkProgram(struct atomi *curent)
 {
      if( curent->codLexical ==1 && curent->atributId==19) // program 
      {
			 		strcpy(ts[dimts].clasa,"program");
			 		ts[dimts].nivel = nivel;
			 		ts[dimts].adr_start = -1;
			 		ts[dimts].incdom = -1;
			 		ts[dimts].dim_var = 0;
          
          
					curent = curent->urm;
          
          if(curent->codLexical == 2) // identificator
          {        
						 strcpy(ts[dimts].nume,curent->atom);
						 dimts++;	
						 	 
             curent = curent->urm;
             if(curent->codLexical ==7 && curent->atributId ==0) //;
               {
                   
                 curent = checkBlock(curent->urm); 
                   if(curent == NULL) // eroare in functia sectiuneConst
                       return;
                 printf("first atom after main block %s\n",curent->atom);
                  if(!(curent->codLexical ==7 && curent->atributId ==2) && curent== first->pre) //. & last atom
                  {
                      perror("no . at end of project");
                  }
                  if(curent->urm != first)
                   printf("more parametres after ending of program\n");
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
 
 void initTS();

 int main(int argc, char *argv[])
 {
 		 dimts = 0;
		 dim_parent =0;
 		 nivel = 1;
 		 adrel = 0; 

     openFile("in.txt");
     sapareteAtoms();
     //printf("%c",getCharUrm());
     closeFile();
     printfAtoms();
     initTS(); 
     printTS();
		 checkProject(first);
     getchar();
    return 0;
 }
 void removeLevel(int niv)
 {
 			//return ;
 	//		printf("\n\n\n %d", niv);
 			int i;
 	for (i = dimts-1; i >=1, 	ts[i].nivel == niv; i--)
 			{
			ts[i].adr_start = -2;
		  adrel = ts[i].adrel;
			ts[i].adrel= -2;		
	    ts[i].deplrec = -2;
	    ts[i].dim_var = -2;
 			
		  ts[i].tip  = -2; 
			ts[i].index_val =-2;
	 		ts[i].adrel =-2;
			ts[i].deplrec =-2;
			ts[i].nivel = -2;
			ts[i].nr_par =-2;
			ts[i].dim_var = -2;
			ts[i].adr_start = -2;
		
			ts[i].min =-2;
		 	ts[i].max = -2; 
			
		 	ts[i].nr_lista_rec = -2;
		 	ts[i].incdom = -2;
	 		dimts --;
			  }		
}
 void initTS(){
 			int i;
 			for (i = 0; i <MAX_TS; i++)
 			{
			ts[i].adr_start = -2;
			ts[i].adrel= -2;		
	    ts[i].deplrec = -2;
	    ts[i].dim_var = -2;
 			
		  ts[i].tip  = -2; 
			ts[i].index_val =-2;
	 		ts[i].adrel =-2;
			ts[i].deplrec =-2;
			ts[i].nivel = -2;
			ts[i].nr_par = -2;
			ts[i].dim_var = 2;
			ts[i].adr_start = -2;

			ts[i].min =-2;
		 	ts[i].max = -2; 
			
		 	ts[i].nr_lista_rec = -2;
		 	ts[i].incdom = -2;
	 		 }
 }

void printTS()
{
 int i,j;
 //dimts =3;
 //printf("i nume	cls	tip	i_val	adrl deplrec nivel	nr_par d_var adr_st l_par min	max	l_rec	nr_l_r incdom");
 if( checkDuplicaTS() ==0)
   return;
 printf("Tabela simbolica:\n\n");
 for (i =0; i<dimts; i++)
 {
 		 printf("%d %s %s %d %d %d ",i,ts[i].nume,ts[i].clasa,ts[i].tip, ts[i].index_val, ts[i].adrel);
 		 printf("%d %d %d %d %d [", ts[i].deplrec, ts[i].nivel, ts[i].nr_par, ts[i].dim_var, ts[i].adr_start);
     
		 for( j=0; j<ts[i].nr_par; j++)
     {
		 printf("(%d,%d) ",ts[i].lista_param[0][j],ts[i].lista_param[1][j]);			
		}
		printf("] %d %d [",ts[i].min, ts[i].max);
	for( j=0; j<ts[i].nr_lista_rec; j++)
     {
		 printf("%d ",ts[i].lista_rec[j]);			
		}
		printf("] %d %d\n\n",ts[i].nr_lista_rec,ts[i].incdom);
		 
 }
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
 							   if( createAndSaveAtom(10,0,lineNo,">") ==0)
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
 
