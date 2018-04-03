/*
 * Project 4: Virtual Memory Manager
 * Felix Adrian Lucaciu	27397941
 * Michael Magnabosco 	27189737
 */

#include <stdio.h>
#include <stdlib.h>


#define MAXCHAR 1000

int main() {

//-------------------------Reading Input File----------------------------//
  int InputAddress[MAXCHAR];
  int AddressCounter = 0;
  const char* filename = "/Users/Felix/school/University/Winter_2018/COEN346/COEN346Assignments/Ass4_2018/addresses.txt";
  FILE* file = fopen (filename, "r");
  int i = 0;

  //fscanf (file, "%d", &i);    
  while (!feof (file))
    {  
      fscanf (file, "%d", &i);
      InputAddress[AddressCounter] = i; 
      printf ("%d\n", i);
      AddressCounter++;      
    }
  fclose (file); 


//---------------------------------------------------------------------//

  





  return 0;
}

//The TLB is like a cache it's input is a logical  address and its outuput is a physical  address

