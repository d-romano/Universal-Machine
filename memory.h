#include "seq.h"
#include <stdint.h>
/** Represents memory in a computer.
 * Will contain mapped segs, unmapped IDS and prog counter
 **/
typedef struct mem{
	// Holds all mapped segments
        Seq_T segments;
	//Holds ID's of unmapped segments
        Seq_T unmapped;
        //Holds the total number of segs.
        unsigned totSegs;
	// Holds program counter
        int progCount;
}mem;

//Creates new memory, sets all intial values and returns to UM
mem* initMemory(Seq_T prog);

//Frees memory 
void freeMemory(mem *memory);

// Frees an indiviual segment of memory
void freeSegment(Seq_T* seg);

//Maps a segment of size
uint32_t mapSeg(mem *memory, uint32_t size);

//Unmap segment in memory located at segLoc
void umapSeg(mem *memory, uint32_t segLoc);
