#include <stdio.h>
#include <stdlib.h>

#include "run.h"
#include "memory.h"
#include "assert.h"
#include "bitpack.h"

#include <time.h>

//Used to figure out how long to run
//clock_t s, f;
//int64_t numInst;

//Functions only need to be accessible to run.
//Moves value of reg[rB] to reg[rA] if condition is met
void condMove(uint32_t* reg, uint32_t rA, uint32_t rB, uint32_t rC);

//store the sum of valOne and valTwo into reg[sum]
void add(uint32_t* reg, uint32_t sum, uint32_t valOne, uint32_t valTwo);

//Store the product of valOne and valTwo in reg[product] 
void mult(uint32_t* reg, uint32_t product, uint32_t valOne , uint32_t valTwo);

//Store the quotient of dividend / divisor in reg[quotient]
void divid(uint32_t* reg, uint32_t quotient, uint32_t dividend, uint32_t divisor);

//Store the result of valOne NAND valTwo in reg[result]
void nand(uint32_t* reg, uint32_t result, uint32_t valOne, uint32_t valTwo);

// Read user input one charater at a time and store if in range 0<=n<=255
void input(uint32_t* reg, uint32_t value);

//Output value stored in register as long as its within range 0<=n<=255
void output(uint32_t* reg, uint32_t value);

//Stop free all used memory and exit
void halt(mem* memory);

// Load value stored in memory[segNum][offset] into reg[regLoc]
void segLoad(uint32_t* reg, mem* memory, uint32_t regLoc, uint32_t segNum, uint32_t offset);

//Store the value from reg[val] into memory[segNum][offset]
void segStore(mem* memory, uint32_t segNum, uint32_t offset, uint32_t val);

//Duplicate program found at memory[segLoc] into memory[0], free old prog and set counter
void loadProg(uint32_t* reg, mem* memory,  uint32_t segLoc, uint32_t ctr);

//Load val into reg[valLoc]
void loadVal(uint32_t* reg, uint32_t varLoc, uint32_t val);


void condMove(uint32_t* reg, unsigned rA, unsigned rB, unsigned rC){
	if(reg[rC] != 0)
		reg[rA] = reg[rB];
}

void segLoad(uint32_t* reg, mem* memory, unsigned regLoc, unsigned segNum, unsigned offset){
        Seq_T seg = Seq_get(memory->segments, segNum);
        reg[regLoc] = (uint32_t)(uintptr_t)Seq_get(seg, offset);
}

void segStore(mem* memory, unsigned segNum, unsigned offset, unsigned val){
        Seq_T seg = Seq_get(memory->segments, segNum);
        Seq_put(seg, offset, (void*)(uintptr_t)val);
}

void add(uint32_t* reg, unsigned sum, unsigned valOne, unsigned valTwo){
	reg[sum] = (reg[valOne] + reg[valTwo]);
}

void mult(uint32_t* reg, unsigned product, unsigned valOne , unsigned valTwo){
	reg[product] = (reg[valOne] * reg[valTwo]);
}

void divid(uint32_t* reg, unsigned quotient, unsigned dividend, unsigned divisor){
	reg[quotient] = reg[dividend] / reg[divisor];
}

void nand(uint32_t* reg, unsigned result, unsigned valOne, unsigned valTwo){
	reg[result] = ~(reg[valOne] & reg[valTwo]);
}

void halt(mem* memory){
        freeMemory(memory)
		;
	//Take time to end
	//printf("Num Instructions: %ld. Time to run: %f", numInst, (double)(f-s)/CLOCKS_PER_SEC);
        exit(0);
}

void input(uint32_t* reg, unsigned value){
	uint32_t input = getc(stdin);
	
	if(input == (uint32_t)EOF)
		input = ~(0x0);
	//Only save input if its within range 0-255 inclusive.
	if(input <= 255)
		reg[value] = input;
	
}

void output(uint32_t* reg, unsigned value){
	//ensures that value can only be 0-255 inclusive
	assert(reg[value] <=255);
	putchar(reg[value]);
}

void loadProg(uint32_t* reg, mem* memory,  uint32_t segLoc, uint32_t ctr){
	//If segment stays same then jump to program counter.
	//counter decremented by 1 to account for incrementor after call
	if(reg[segLoc] != 0){
		
		Seq_T progToLoad = Seq_get(memory->segments, reg[segLoc]);
		uint32_t size = Seq_length(progToLoad);
	
		//Create prog of equal length
		Seq_T newProg = Seq_new(size);

		// duplicate progToLoad into newProg
		for(uint32_t i = 0; i < size; i++)
			Seq_addhi(newProg, Seq_get(progToLoad, i));

		//Put newprog in 0 and free the segment that was previously there
		Seq_T oldProg = Seq_put(memory->segments, 0, newProg);
		freeSegment(&oldProg);
	}

	//Set the memory counter to the value -1 to account for incrementor.
	memory->progCount = reg[ctr] -1;
}


void loadVal(uint32_t* reg, unsigned varLoc, uint32_t val){
	reg[varLoc] = val;
}


void runUM(Seq_T prog){
	//Take time at start
	//s = clock();
	//Used to calculate number of instructions
	//numInst = 0;

	mem *memory = initMemory(prog);

	//Storing array on the stack
	uint32_t reg[8] = {0};

	uint32_t word;
	OPCODES opcode;
	uint32_t rA;
	uint32_t rB;
	uint32_t rC;

	while(1){
		
		//if(numInst == 50000000)
		//	f = clock();

		word = (uint32_t)(uintptr_t)Seq_get(Seq_get(memory->segments,0),memory->progCount);
		opcode = Bitpack_getu(word, 4, 28);
	
		if(opcode == vload){
			rA = Bitpack_getu(word, 3, 25);
			uint32_t val = Bitpack_getu(word, 25, 0);
			loadVal(reg, rA, val);
		}
		else{
			rA = Bitpack_getu(word, 3, 6);
                	rB = Bitpack_getu(word, 3, 3);
                	rC = Bitpack_getu(word, 3, 0);

			switch(opcode){
				case condMv:	
                                	condMove(reg, rA, rB, rC);
                                	break;
				case sload:
					segLoad(reg, memory, rA, reg[rB], reg[rC]);
                                	break;
				case sstore:
                                	segStore(memory, reg[rA], reg[rB], reg[rC]);
                                	break;
				case adtn:
             	                   	add(reg, rA, rB, rC);
                	                break;
				case multi:
                                	mult(reg, rA, rB, rC);
                                	break;
				case divi:
                                	divid(reg, rA, rB, rC);
                                	break;
				case bnand:
                                	nand(reg, rA, rB, rC);
                                	break;
				case hlt:
                                	halt(memory);
                                	break;
				case mseg:
                                	reg[rB] = mapSeg(memory, reg[rC]); 
                                	break;
				case umseg:
                                	umapSeg(memory, reg[rC]);
                                	break;
				case out:
                                	output(reg, rC);;
                                	break;
				case in:
                               		input(reg, rC);
                                	break;
				case pload:
                                	loadProg(reg, memory, rB, rC);
                                	break;
				default:
					fprintf(stderr, "Invalid constuction\n");
					exit(1);
				}
		}
		//numInst++;
		memory->progCount++;
	}
	return;
}
