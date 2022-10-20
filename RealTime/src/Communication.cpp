#include "Communication.h"


void open_serial(char* path)
{
    sprintf(serialPath,"%s",path);
    serialFile=fopen(serialPath,"w");
}