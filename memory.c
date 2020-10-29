#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "memory.h"

mem* initMemory(Seq_T prog){

	mem *newMem = malloc(sizeof(mem));
	newMem->segments = Seq_new(0);
	newMem->unmapped = Seq_new(0);
	newMem->totSegs = 0;
	newMem->progCount = 0;
	
	//Push the program into newMem
	Seq_addhi(newMem->segments, prog);
	newMem->totSegs++;

	return newMem;
}


void freeMemory(mem *memory){
	unsigned totalSegs = memory->totSegs;
	Seq_T seg; 
	
	// Free each individual segment.
	for(unsigned i =0; i < totalSegs; i++){
		seg = Seq_get(memory->segments, i);
		freeSegment(&seg);
	}
	
	// Free 2 seqs in memory and memory struct
	Seq_free(&memory->segments);
	Seq_free(&memory->unmapped);

	free(memory);
}


void freeSegment(Seq_T* seg){
	Seq_free(seg);
}


uint32_t mapSeg(mem *memory, uint32_t size){

	//Create new seg of size and initialize all values to 0.
	Seq_T newSeg = Seq_new(size);
	for(uint32_t i = 0; i < size; i++)
		Seq_addhi(newSeg, (void*)(uintptr_t)0);
		
	// If no unmapped values create a new one
	if(Seq_length(memory->unmapped) == 0){
		Seq_addhi(memory->segments, newSeg);
		memory->totSegs++;

		return (uint32_t)(memory->totSegs -1);
	}
	else{
		//Remove id from top of stack
		uint32_t id = (uint32_t)(uintptr_t)Seq_remhi(memory->unmapped);
	
		Seq_T oldSeg;
		//Replace old segment with new and free old seg.
		oldSeg = Seq_put(memory->segments, id, newSeg);
		freeSegment(&oldSeg);
		return id;
	}
}


void umapSeg(mem *memory, uint32_t segLoc){
	Seq_addhi(memory->unmapped, (void*)(uintptr_t)segLoc);
}

