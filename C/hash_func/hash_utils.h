#ifndef HASH_UTILS_H
#define HASH_UTILS_H

unsigned int RSHash(char* str, unsigned int len);
unsigned int JSHash(char* str, unsigned int len);
unsigned int PJWHash(char* str, unsigned int len);
unsigned int ELFHash(char* str, unsigned int len);
unsigned int BKDRHash(char* str, unsigned int len);
unsigned int SDBMHash(char* str, unsigned int len);
unsigned int DJBHash(char* str, unsigned int len);
unsigned int DEKHash(char* str, unsigned int len);
unsigned int BPHash(char* str, unsigned int len);
unsigned int FNVHash(char* str, unsigned int len);
unsigned int APHash(char* str, unsigned int len);

#endif // HASH_UTILS_H
