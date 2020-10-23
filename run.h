#include "seq.h"

//Holds the opcodes 
typedef enum{
	condMv =0,
	sload,
	sstore,
	adtn,
	multi,
	divi,
	bnand,
	hlt,
	mseg,
	umseg,
	out,
	in,
	pload,
	vload
}OPCODES;

/**
 * Runs the initial program and loads it into memory ($m[0])
 * then extract the opcodes and register locations to run instructions.
 **/
void runUM(Seq_T prog);
