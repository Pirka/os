/*
 * main.c
 *
 *  Created on: Feb 14, 2016
 *      Author: root
 */


#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>

int main( void )
{
  FILE *pFile;
  uint8_t data[512];
  int bytesRead = 0;
  int i,j;
  uint16_t address=0;


  pFile = fopen("block_2.bin", "rb");
  if (pFile)
  {
	  bytesRead = fread(data, 1, 512, pFile);

	  for (i=0,address=0; i < 512/16; i++)
	  {
		printf("%04X: ", address);
		address += 16;

	    for (j=0; j < 16; j++)
	    {
		  printf("%02X ", data[(i*16)+j]);
	    }
	    printf("\n");
	  }
  }

  printf("\n");
  fflush(stdout);

  return 0;
}







