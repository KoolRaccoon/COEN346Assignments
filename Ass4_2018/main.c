/*
 * Project 4: Virtual Memory Manager
 * Felix Adrian Lucaciu	27397941
 * Michael Magnabosco 	27189737
 */

#include <stdio.h>
#include <stdlib.h>

int main()
{
   int num;
   FILE *fptr;

   if ((fptr = fopen("/Users/Felix/school/University/Winter_2018/COEN346/COEN346Assignments/Ass4_2018/addresses.txt","r")) == NULL){
       printf("Error! opening file");

       // Program exits if the file pointer returns NULL.
       exit(1);
   }

   fscanf(fptr,"%d", &num);

   printf("Value of n=%d \n", num);
   fclose(fptr); 
  
   return 0;
}

//The TLB is like a cache it's input is a logical  address and its outuput is a physical  address

