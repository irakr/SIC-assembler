/*
 *	sic.h
 *
 *	Defines the standards for the SIC(Simplified Instructional Computer) version.
 */

#ifndef SIC_H_
#define SIC_H_

#include <stdint.h>

//Define SIC standard
#ifndef SIC_STD_
#define SIC_STD_

//SIC standard data types

//SIC memory address (15 bits)
typedef struct{
	uint16_t val : 15;	//111 1111 1111 1111(MAX)
} SIC_Addr;

//SIC word(1 word = 3 bytes = 24 bits)
typedef struct{
	int32_t val : 24;	//1111 1111 1111 1111 1111 1111(MAX)
} SIC_Word;

//SIC integer type(1 word)
typedef SIC_Word SIC_Integer;

//SIC registers(8-bit)
typedef char SIC_Register;

//Logical address range of a source program. Actual address will be determined by the loader.
#define MAX_ADDR 0x7fff		//Largest 15-bits unsigned integer.(For address values)

#define WORDSIZE 3	//Default word size of SIC architecture
#define MAX_WORD 0x7fffff	//Largest 24-bits signed number

#endif	//SIC_STD_

#endif
