/*!																www.mycal.net			
 *---------------------------------------------------------------------------
 *! \file ybase64.c
 *  \brief base64 
 *																			
 *---------------------------------------------------------------------------
 * Version                                                                  -
 *		0.1 Original Version Jan 6, 2006								    -        
 *                                                                          -
 *---------------------------------------------------------------------------    
 *                                                             				-
 * Copyright (C) 2003, mycal.net						                    -
 *                                                                         	-
 *
 *---------------------------------------------------------------------------
 *
 * Notes:
 *
 *
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ybase64.h"



/*

	tests:


	Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.


    TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlz
    IHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2Yg
    dGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGlu
    dWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRo
    ZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4= 




	*/


//
// This used to be a table lookup, this is smaller for 8bit processors, encode b into base64
//
static char encode_byte(unsigned char b) 
{
  // Convert byte
  if(b < 26)
	  return 'A'+b;
  if(b < 52)
	  return 'a'+(b-26);
  if(b < 62)
	  return '0'+(b-52);
  if(b == 62)
	  return '+';
  return '/';
}

static unsigned char decode_byte(char b) 
{
  if(b >= 'A' && b <= 'Z')
	  return(b - 'A');
  if(b >= 'a' && b <= 'z')
	  return(b - 'a' + 26);
  if(b >= '0' && b <= '9')
	  return(b - '0' + 52);
  if(b == '+')
	  return 62;
  return 63;
}

int Mis_base64(unsigned char c) 
{
  return (isalnum(c) || (c == '+') || (c == '/') || (c== '='));
}



/**
 * Base64_encode encodes the passed byte array of passed length long.  Will malloc a destination buffer for return.
 * Caller must free the returned base64 string.
 * @param bytes_to_encode - a pointer to the data to be encoded.
 * @param len - then number of bytes to encode
 * @return encoded string otherwise NULL
 */

char *base64_encode(unsigned char const* bytes_to_encode, unsigned int len) 
{
  char *ret,*ptr;
  int i = 0;
  int in_len;
  unsigned char c1, c2, c3;

  // Check for Null input pointer
  if(NULL==bytes_to_encode)
    return NULL;

  // If no length is specified assume bytes to encode is a string, and encode assuming a zero terminated string
  if(!len)
    in_len= strlen((char *)bytes_to_encode);
  else
	  in_len=len;

  //
  // Malloc a chunk of memory for the output, size of output can be len*(256/64)+max_padding+"zero_terminator", or len*4/3+4+1 (or one third larger + padding)
  // Add extra word for zero terminator for ARM7 align bug.
  //
  ret=ptr= malloc(in_len*4/3+8);

  // Convert in 3 byte chunks to 4 output bytes until done
  i=0;
  while(i<in_len)
  {
	  c1=c2=c3=0;

	  c1=bytes_to_encode[i];
	  
	  if(i+1<in_len)
	  {
		  c2=bytes_to_encode[i+1];
	  }
	  if(i+2<in_len)
	  {
		  c3=bytes_to_encode[i+2];
	  }

	  // Encode 1st 6 bits of 1st byte
	  *ptr++=encode_byte(c1>>2);
	  // encode next 6 bits remainder of 1st bytes and part of 2nd byte
	  *ptr++=encode_byte( ((c1&0x3)<<4)|(c2>>4) );

	 // if we have more data, encode next bits
	  if(i+1<in_len)
	  {
		 // Encode next 6 bits
		 *ptr++= encode_byte( ((c2&0xf)<<2)|(c3>>6) );
	  } 
	  else 
	  {
		  // Padd
		  *ptr++= '=';
	  }
	 // if we have more data, encode next bits
	  if(i+2<in_len)
	  {
		 // Encode last 6 bit chunk
		 *ptr++= encode_byte( c3&0x3f );
	  } 
	  else 
	  {
		  // Padd
		  *ptr++= '=';
	  }
	  // Jump to next chunk
	  i=i+3;
  }
  // Zero terminate string
  *ptr='\0';

  // return pointer to beginning of string
  return ret;
}



// Input is zero terminated string of base64 encode

int base64_decode(unsigned char *output, unsigned char *bytes_to_decode) 
{
	int i=0,j=0;
	int out_len=0;
	unsigned char *ptr=output;
	unsigned char tarray[5];

	if((bytes_to_decode) && (*bytes_to_decode))
	{
		while (( bytes_to_decode[i] != '\0') && ( bytes_to_decode[i] != '=')) 
		{
			if(!Mis_base64(bytes_to_decode[i]))
				continue;

			tarray[j++] = decode_byte(bytes_to_decode[i++]);
			if(4==j)
			{
				*ptr++=((tarray[0]<<2)|(tarray[1]>>4) );
				*ptr++=(((tarray[1]&0xf)<<4)|(tarray[2]>>2) );
				*ptr++=(((tarray[2]&0x3)<<6)|tarray[3] );
				j=0;
			}
		}
		if(j)
		{
			tarray[3]=0;
			if(j<3) 
				tarray[2]=0;
			if(j<2) 
				tarray[1]=0;

			*ptr++=((tarray[0]<<2)|(tarray[1]>>4) );
			if(1<j)
				*ptr++=(((tarray[1]&0xf)<<4)|(tarray[2]>>2) );
		}
		*ptr++='\0';
		return(ptr-output);
	}
	return(0);
}




#if defined(TEST_PGM)
main()
{
	int a1,a2;
	char *out,*out1;
	char torig[512];
	char torig2[512];

	out1 =base64_encode("Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.",0);
	a1=base64_decode(torig, out1);

	printf("output %s\norig - %s",out,torig);


	free(out);

	free(out1);
}
#endif
