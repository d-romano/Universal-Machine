#include <stdint.h>
#include "readDisk.h"
#include "bitpack.h"
#include "assert.h"


// uses sys/stat.h to get file size in bytes.
unsigned getFileSize(const char* fname);
/**
 *  Calculates the number of words in the program
 *  32-bits per word, 8 bits per byte = 4 bytes per word.
 **/
unsigned getNumWords(unsigned fsize);


Seq_T initRead(FILE* file, const char *fname){
	
	unsigned numWords;
	unsigned fsize = getFileSize(fname);

	// Ensures there are only full words in file
	assert(fsize %4 == 0);
	
	numWords = getNumWords(fsize);

	//Creates a new seq that represents the program stored in $m[0]
	Seq_T prog = Seq_new(fsize);
	

	/**
	 * Moves to the beginning of a word then reads the next 4 chars
	 * and packs them into a uint32_t word.
	 **/
	unsigned val;
	unsigned width = 8;
	for(unsigned i = 0; i < numWords; i++){
		uint32_t word = 0x0;
		int lsb = 32;
		for(unsigned j = 1; j <=4; j++){
			val = fgetc(file);
			word = Bitpack_newu(word, width, lsb-(width*j), val);
		}
		Seq_addhi(prog, (void*) (uintptr_t)word);
	}

	return prog;
}


unsigned getFileSize(const char* fname){
	struct stat st;

	stat(fname, &st);
	return st.st_size;
}

unsigned getNumWords(unsigned fsize){
	return fsize/4;
}
