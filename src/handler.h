#ifndef _HENDLER
#define _HENDLER

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

struct fileatr
{
	char fname[255];
	int fsize;
};

int filecreate(char* x);
int filewrite(char* x, char* y);
int fileread(char* x, char* y);
void curses(char* x, int maxlen);
void filemanager();

struct fileatr* odir(const char* x, int*y);
#endif
