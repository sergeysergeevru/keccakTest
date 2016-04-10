#pragma once
#include"sqlite3.h"
#include <sstream>
#include<iostream>
#include<fstream>
#include<iomanip>
#include<Windows.h>
#include<io.h>
#include <stdio.h> 
#include <direct.h>


void FIPS202_SHAKE128(const unsigned char *input, unsigned int inputByteLen, unsigned char *output, int outputByteLen);
void FIPS202_SHA3_256(const unsigned char *input, unsigned int inputByteLen, unsigned char *output);
int callback(void *obj, int argc, char **argv, char **szColName);
void fileHash(char  *fileName, unsigned char * ch);
void writesum(char*, sqlite3 *);
bool fileExists(const char *fname);