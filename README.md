# Universal Machine

This was project was designed as an assignment for CSC411 Spring 2020 semester. It was build around utilizing Hanson's Datastructures.

The goal of this project was to design a "Universal Machine" that is able to read 32-bit word instructions from a textfile
passed via the command-line. Each word contains a op code stored in the four most significant bits and depending on the op
1 or 3 registers to store information.


## Abilities/ Functions

In the following descriptions Reg refers to a simulated register and Mem represents simulated memory. 

The UM can perform the following functions:

* Conditional Move: Moves value of Reg[A] to Reg[B] when Reg[C] == 0
* Segmented Load: Reg[A] stores the instructions located at Mem[Reg[B]][Reg[C]] Where Reg[B] is a seg-ID and Reg[C] is an offset.
* Segmented Store: Store the value of Reg[C] into Mem[Reg[A]][Reg[B]] Where Reg[A] is a seg-ID and Reg[B] is an offset.
* Addition: Stores Reg[B] + Reg[C] into Reg[A].
* Multiplication: Stores Reg[B] * Reg[C] into Reg[A].
* Division: Stores Reg[B] ^ Reg[C] into Reg[A].
* Bitwise NAND: Stores -(Reg[B]^Reg[C]) into Reg[A].
* Halt: Stops the UM application.
* Map Segment: A segment the size of the value stored in Reg[C] is created and initialized to 0. The ID of this segment (whether new or used) is returned and store in Reg[B]
* Unmap Segment: Reg[C] stores the ID of a segment that is to be unmapped for reuse.
* Output: Display the value stored in Reg[C] provided its in the range 0-255.
* Input: UM waits for I/O containing a value from 0-255, value is stored into Reg[C] until EOF when ~(0x0) is stored.
* Load Program
* Load Value: Stores the 3 bits after the opcode into R[A]

## Components
### UM.c:
Expects location of instruction file to be passed via cmd then attempts to open to validate existance.
This program then passes the file to readDisk.c and passes the results of that call to run.c.

### readDisk.c:
Contains the secrets of reading the program from the disk. Calculates the size of the file in bytes then
uses it to calculate the number of words. Finally reads the file in big-endian order packing the bits into words.

### run.c:
Contains the intial loop for reading in instructions, extracts the words using bitpack then uses the opcode to call

### memory.c:
Simulates the memory of a machine. 

Made up of a struct that contains:
* A sequence of mapped (in-use) segments. Segments are represented as a sequence of words.
* A sequence of uint32 ID's that indicates which memory segments arent in use. This is utilized
  like a stack  so the most recently freed memory segments are used first.
* A counter that keeps track of the total number of segments. Used for ID's and freeing memory.
* A program counter that keeps track of the current word of the segment.

A new segment is made only when there are no unmapped segments available and the total number of segments
is then incremented. If there are IDs stored in the unmapped sequence they are then popped out  and the 
old memory segment is then freed and replaced with a new segment.
