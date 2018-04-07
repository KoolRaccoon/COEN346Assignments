/*
 * Project 4: Virtual Memory Manager part 1
 * Felix Adrian Lucaciu 27397941
 * Michael Magnabosco   27189737
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

int PhysicalMemory[PAGE_NUMBER][PAGE_SIZE];
int TLB [TLB_SIZE][2];
int PageTable [PAGE_NUMBER][2];

const int bit_mask_offset = 255;
const int bit_mask_addrress = 65535;

int FrameCounter = 0;
int PageFaultCounter = 0;
int TLBHitCounter = 0;
int TLBEntries = 0;

signed char value;
signed char buffer[BUFFER_SIZE];

FILE *backing_store;

void readStore (int);
void insertTLB (int, int);


int main(int argc, char **argv) {

//-------------------------Reading Input File----------------------------//
  int InputAddress[TOTAL_ADDRESS_ENTRIES];
  int AddressCounter = 0;

  
  if (argc < 3){
    printf("Expected 2 Arguments to be passed, terminating program\n");
    return 1;
  }

  FILE *Addressfile = fopen(argv[1], "r");

  int i = 0;

  //fscanf (file, "%d", &i);
  while (!feof (Addressfile)){
      fscanf (Addressfile, "%d", &i);
      InputAddress[AddressCounter] = i;
      //printf ("%d\n", i);
      AddressCounter++;
    }
  fclose (Addressfile);

// open the file containing the backing store
backing_store = fopen(argv[2], "rb");

if (backing_store == NULL) {
  printf("Could not find BACKING_STORE.bin, terminating program\n");
  return 1;
}

//----------------------------Getting Page Number and Offset-----------------------------------------//
for (int i = 0; i < TOTAL_ADDRESS_ENTRIES; i++){
  int frameNumber = -1;
  int NextAddress = InputAddress[i];
  Page PageEntry;
  PageEntry.PageNum = (NextAddress & bit_mask_addrress)>>8;
  PageEntry.Offset = NextAddress & bit_mask_offset;
  
  for (int j=0; j<TLB_SIZE; j++){
    if (TLB[j][0] == PageEntry.PageNum){
      frameNumber = TLB[j][1];
    TLBHitCounter++;
      break;
    }
  }

  if (frameNumber == -1) {
    //Display the value at the correct frame
    for (int k=0; k<FrameCounter; k++){
    if (PageTable[k][0] == PageEntry.PageNum){
        frameNumber = PageTable[k][1];
      }
    }
    if (frameNumber == -1){
      //Read Store
    readStore(PageEntry.PageNum);
    frameNumber = FrameCounter-1;
      PageFaultCounter++;
    }
  }
  
  //insert to TLB
  insertTLB(PageEntry.PageNum, frameNumber);
  
  //Store value
  value = PhysicalMemory[frameNumber][PageEntry.Offset]; 
  
  //Output virtual and physical address as well as the output
    printf("Virtual address: %d Physical address: %d Value: %d\n", NextAddress, ((frameNumber << 8) | PageEntry.Offset), value);



}// End main for loop
  printf("Number of translated addresses = %d\n", AddressCounter);
  double faultRate = PageFaultCounter / (double)AddressCounter;
  double hitRate = TLBHitCounter / (double)AddressCounter;
    
  printf("Page Faults = %d\n", PageFaultCounter);
  printf("Page Fault Rate = %.3f\n",faultRate);
  printf("TLB Hits = %d\n", TLBHitCounter);
  printf("TLB Hit Rate = %.3f\n", hitRate);
  
  return 0;
}

//----------------------------Inserting Into TLB TABLE-----------------------------------------//
void insertTLB(int pageNumber, int frameNumber){
  int i;  
  //Gets position of page number in the table if it is there
    for(i = 0; i < TLBEntries; i++){
        if(TLB[i][0] == pageNumber){
            break;
        }
    }
    
    //Checks if entry and index are the same
    if(i == TLBEntries){
    //Insert if there is room
        if(TLBEntries < TLB_SIZE) {  
            TLB[i][0] = pageNumber; 
            TLB[i][1] = frameNumber;
        }
        else {  
      //Shift everything by one
            for(i = 0; i < TLB_SIZE - 1; i++) {
        TLB[i][0] = TLB[i+1][0];
        TLB[i][1] = TLB[i+1][1];
            }
      //Insert page and frame number at the end
            TLB[TLBEntries-1][0] = pageNumber;  
            TLB[TLBEntries-1][1] = frameNumber;
        }        
    }
 
    else {
    //shift array
        for(i = i; i < TLBEntries - 1; i++) {
            TLB[i][0] = TLB[i + 1][0]; 
            TLB[i][1] = TLB[i + 1][1];
        }
    //Insert if there is room
        if(TLBEntries < TLB_SIZE){             
            TLB[TLBEntries][0] = pageNumber;
            TLB[TLBEntries][1] = frameNumber;
        }
    //Replace last entry if no room
        else{                                       
            TLB[TLBEntries-1][0] = pageNumber;
            TLB[TLBEntries-1][1] = frameNumber;
        }
    }
  //Increment if table isn't full
    if(TLBEntries < TLB_SIZE){      
        TLBEntries++;
    }
}

//----------------------------Reading Backing Store-----------------------------------------//
void readStore(int pageNumber){

  //look through backing store
  if (fseek(backing_store, pageNumber * BUFFER_SIZE, SEEK_SET) != 0) {
      printf("Could not seek from in backing store, terminating program\n");
      exit(0);
  }

  //Read the backing store
  if (fread(buffer, sizeof(signed char), BUFFER_SIZE, backing_store) == 0) {
      printf("Could not read from backing store, terminating program\n");
      exit(0);
  }

  // Adds page to Page Table and physical memory
     
  for(int i = 0; i < BUFFER_SIZE; i++){
    PhysicalMemory[FrameCounter][i] = buffer[i];
  }
  
  PageTable[FrameCounter][0] = pageNumber;
  PageTable[FrameCounter][1] = FrameCounter;
    
  //Increment Frame Counter
  FrameCounter++;

}