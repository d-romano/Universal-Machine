#include "bitpack.h"
#include "assert.h"
#include <stdio.h>
static inline uint64_t shl(uint64_t word, unsigned bit){
	if(bit == 64)
		return 0;
	else return word << bit;
}

//Perform logical right shift
static inline int64_t shr(uint64_t word, unsigned bit){
	if(bit == 64)
		return 0;
	else return word >> bit;
}

//Perform arithmetic right shift
static inline int64_t sha(uint64_t word, unsigned bit){
	if(bit == 64)
		bit =63;
	return (int64_t)word >> bit;
}


bool Bitpack_fitsu(uint64_t n, unsigned width){

	/** idea is to shift the word to the left by 64-width then right 64-width bits. 
	 * If the word fits in the passed width then it will be unchanged, else it will
	 * be a different number indicating it will not fit.
	 **/
	
	assert(width <= 64);

//	uint64_t testedNum = shr(shl(n, 64-width), 64-width);
//	return testedNum == n;
//
	return shr(n, width) == 0;
}


bool Bitpack_fitss(int64_t n, unsigned width){
	// idea remains the same as fitsu, shift left by 64-width and then back right to 64-width.
	 
	assert(width <= 64);

        int64_t testedNum = sha(shl(n, 64-width), 64-width);

	return testedNum == n;
}

uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb){
	assert(width <= 64);
	//Ensure with-in range 0<= width+lsb <= 64
	assert(width+lsb <= 64);

	/**
	 * shift left by 64-(width+lsb) to trip all values left of msb, shift right 64-width
	 * to put set values lsb to 0
	 **/	
	return shr(shl(word, 64-(width+lsb)), 64-width);
}

int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb){
	assert(width <= 64);
	assert(width+lsb <= 64);

	/**
	 * same idea as bitpack_getu but utilizes an arithmetic right shift
	 * if number is negative the ones to the left will cancel each other and
	 * only the proper negative value will remain.
	 **/
	return sha(shl(word, 64-(width+lsb)), 64-width);
}


uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value){

	// Ensure that the value fits else raise Bitpack overflow
	if(!Bitpack_fitsu(value, width))
		RAISE(Bitpack_Overflow);

	/**
	 * Separate the the values to the left and of the replaced word
	 * then use bitwise OR to add these values to create the updated word.
	 * */ 
	uint64_t leftVal = word;
	uint64_t newVal = value;
	uint64_t rightVal = word;


	leftVal = shl(shr(leftVal, width+lsb), width+lsb);
	rightVal = shr(shl(rightVal, 64-lsb), 64-lsb);
	newVal = shl(newVal, lsb);

	return leftVal | rightVal | newVal;
}

uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, int64_t value){

	 // Ensure that the value fits else raise Bitpack overflow
        if(!Bitpack_fitss(value, width))
                RAISE(Bitpack_Overflow);

        /**
         * Separate the the values to the left and of the replaced word
         * then use bitwise OR to add these values to create the updated word.
         **/
       	
        uint64_t leftVal = word;

	// Ensure there are no additional ones before the value
        uint64_t newVal = Bitpack_getu(value, width, 0);
        uint64_t rightVal = word;

        leftVal = shl(shr(leftVal, width+lsb), width+lsb);
        rightVal = shr(shl(rightVal, 64-lsb), 64-lsb);
        newVal = shl(newVal, lsb);

        return leftVal | rightVal | newVal;
}


Except_T Bitpack_Overflow = {"Overflow packing bits"};
