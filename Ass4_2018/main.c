/*
 * Project 4: Virtual Memory Manager
 * Felix Adrian Lucaciu	27397941
 * Michael Magnabosco 	27189737
 */

#include <stdio.h>
#include <stdlib.h>


#define TOTAL_ADDRESS_ENTRIES 1000
#define BUFFER_SIZE 256
#define PAGE_SIZE 256
#define PAGE_NUMBER 256
#define TLB_SIZE 16
typedef struct {

  int PageNum;
  int Offset;

} Page;

int buffer[BUFFER_SIZE];
int PhysicalMemory[PAGE_NUMBER][PAGE_SIZE];
int TLB [TLB_SIZE][2];
int PageTable [PAGE_NUMBER][2];

int FrameCounter = 0; 
int PageFaultCounter = 0;
int TLBHitCounter = 0;


void readStore (int);


int main(int argc, char **argv) {

//-------------------------Reading Input File----------------------------//
  int InputAddress[TOTAL_ADDRESS_ENTRIES];
  int AddressCounter = 0;
  //const char* filename = "/Users/Felix/school/University/Winter_2018/COEN346/COEN346Assignments/Ass4_2018/addresses.txt";
  //FILE* file = fopen (filename, "r");
  if (argc < 3){
    printf("Expected more Arguments passed, terminating program\n");
    return 1;
  }

  FILE *Addressfile = fopen(argv[1], "r");

  int i = 0;

  //fscanf (file, "%d", &i);    
  while (!feof (Addressfile))
    {  
      fscanf (Addressfile, "%d", &i);
      InputAddress[AddressCounter] = i; 
      //printf ("%d\n", i);
      AddressCounter++;      
    }
  fclose (Addressfile); 

// open the file containing the backing store
backing_store = fopen("BACKING_STORE.bin", "rb");

if (backing_store == NULL) {      
  printf("Could not find BACKING_STORE.bin, terminating program\n",);
  return 1;
}

//----------------------------Getting Page Number and Offset-----------------------------------------//
int bit_mask = 255;

for (int i = 0; i<TOTAL_ADDRESS_ENTRIES; i++){
  int frameNumber = -1;
  int NextAddress = InputAddress[i];
  Page PageEntry;
  //printf("Displaying entry %d\n", i+1);
  PageEntry.PageNum = NextAddress>>8;
  //printf("Page number: %d\n", PageEntry.PageNum);
  PageEntry.Offset = NextAddress & bit_mask;
  //printf(" Offset: %d\n\n", PageEntry.Offset);


  for (int j=0; j<TLB_SIZE; j++){
    if (TLB[j][0] == PageEntry.PageNum){
      frameNumber = TLB[j][1];
      break;
    }
  }

  //if (TLB_hit){
  if (frameNumber == -1)
    //Display the value at the correct frame
    for (int k=0; k<PAGE_NUMBER; k++){
    if (PageTable[k][0] == PageEntry.PageNum){
      frameNumber == PageTable[k][1];
      break;
    }
  }
  //else {
  if (frameNumber == -1){
    //Read Store
    PageFaultCounter++;
  }
  //insert to TLB 

  //find value



  }// End main for loop






  return 0;
}



void readStore(int pageNumber){

  if (fseek(backing_store, pageNumber * PAGE_SIZE, SEEK_SET) != 0) {
      printf("Could not seek from in backing store, terminating program\n");
      return 1;
  }
  
  // now read CHUNK bytes from the backing store to the buffer
  if (fread(buffer, sizeof(signed char), PAGE_SIZE, backing_store) == 0) {
      printf("Could not read from backing store, terminating program\n");
      return 1;        
  }
  
  // Adds page to Page Table 
  // Store into the Physical memory

}

//The TLB is like a cache it's input is a logical  address and its outuput is a physical  address

