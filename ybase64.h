/*!																www.mycal.net			
 *---------------------------------------------------------------------------
 *! \file ybase64.h
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

#ifndef BASE64_H
#define BASE64_H

//#include <config.h>


int base64_decode(unsigned char *output, unsigned char *bytes_to_decode);
char *base64_encode(unsigned char const* bytes_to_encode, unsigned int len) ;

#endif



