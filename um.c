#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "seq.h"
#include "assert.h"
#include "readDisk.h"
#include "run.h"

int main(int argc, const char* argv[]){

	(void) argc;
	(void) argv;

	if(argc < 2){
		fprintf(stdout, "INVALID INPUT! Use format: ./um file_path\n");
		exit(1);
	}

	//open file ensure it exists
	FILE* file = fopen(argv[1], "r");
	assert(file != NULL);

	//Read the file from the disk
	Seq_T prog = initRead(file, argv[1]);
	fclose(file);
	runUM(prog);

	return 0;
}
