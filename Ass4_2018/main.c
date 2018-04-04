/*
 * Project 4: Virtual Memory Manager
 * Felix Adrian Lucaciu	27397941
 * Michael Magnabosco 	27189737
 */

#include <stdio.h>
#include <stdlib.h>


#define TotalAddressEntries 1000

typedef struct {

  int PageNum;
  int Offset;

} Page;

int main() {

//-------------------------Reading Input File----------------------------//
  int InputAddress[TotalAddressEntries];
  int AddressCounter = 0;
  const char* filename = "/Users/Felix/school/University/Winter_2018/COEN346/COEN346Assignments/Ass4_2018/addresses.txt";
  FILE* file = fopen (filename, "r");
  int i = 0;

  //fscanf (file, "%d", &i);    
  while (!feof (file))
    {  
      fscanf (file, "%d", &i);
      InputAddress[AddressCounter] = i; 
      //printf ("%d\n", i);
      AddressCounter++;      
    }
  fclose (file); 


//---------------------------------------------------------------------//
int bit_mask = 255;

for (int i = 0; i<2; i++){
  int NextAddress = InputAddress[i];
  Page PageEntry;
  printf("Displaying entry %d\n", i+1);
  PageEntry.PageNum = NextAddress>>8;
  printf("Page number: %d\n", PageEntry.PageNum);
  PageEntry.Offset = NextAddress & bit_mask;
  printf(" Offset: %d\n\n", PageEntry.Offset);



}





  return 0;
}

//The TLB is like a cache it's input is a logical  address and its outuput is a physical  address

