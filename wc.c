#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

extern int errno;

unsigned long* analyzeStream(FILE *inStream) {
  bool wordFlag = false;
  char c=0;
  char prevC = 0;
  int newlinesCount = 0;
  int wordsCount = 0;
  int bytesCount = 0;

  static unsigned long returnBuf[3] = {0, 0, 0}; //newlinesCount, wordsCount, bytesCount

  while ((c = fgetc(inStream)) != EOF){
      if ((c == ' ') || (c == EOF) || (c == '\t') || (c == '\n') || (c == '\v') || (c == '\f') || (c == '\r')){ // When whitespace char
        if(wordFlag){ // When whitespace char after non-zero-length sequence of characters
          wordFlag = false;
          wordsCount++;
        }

        if (c == '\n') newlinesCount++;
      } else { // When non-whitespace char
        wordFlag = true;
      }
      bytesCount++;
  } 
  if(wordFlag) wordsCount++; 
  printf("  %d %d %d", newlinesCount, wordsCount, bytesCount);

  returnBuf[0] = newlinesCount;
  returnBuf[1] = wordsCount;
  returnBuf[2] = bytesCount;
  return returnBuf;
}


int main(int argc, char* argv[]){
  FILE *fp;
  int errnum;

  static unsigned long sumBuf[3] = {0, 0, 0}; //newlinesCount, wordsCount, bytesCount
  unsigned long* sumBufTmp; //newlinesCount, wordsCount, bytesCount

  if(argc>1){ // If arguments presented
    for(int i=1;i<argc;i++){// Iterating arguments
      fp = fopen(argv[i], "r");

      if (fp == NULL){ // If opening file unsuccessful
        errnum = errno;
        fprintf(stderr, "wc: %s: %s\n", argv[i], strerror(errnum));

      } else {
        sumBufTmp = analyzeStream(fp);
        sumBuf[0] += *sumBufTmp;
        sumBuf[1] += *(sumBufTmp + 1);
        sumBuf[2] += *(sumBufTmp + 2);
        printf(" %s\n", argv[i]);
      }
      fclose(fp);
    }
    printf("  %ld %ld %ld total\n", sumBuf[0], sumBuf[1], sumBuf[2]);
  } else { // If no arguments analyze stdin
    analyzeStream(stdin);
    putchar('\n');
  }
}




