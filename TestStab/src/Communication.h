#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__
#include <stdio.h>

char serialPath [100];
FILE* serialFile; 

void open_serial(char* path);

#endif // __COMMUNICATION_H__