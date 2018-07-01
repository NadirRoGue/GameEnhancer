#ifndef GAMECRYPT_H_
#define GAMECRYPT_H_

#include <Windows.h>

struct CryptKeys {
	CCHAR in_key[16];
	CCHAR out_key[16];
	BOOL enabled;
};

extern CryptKeys gs_crypt, client_crypt;

VOID SetKey(CCHAR *key, struct CryptKeys *ck);
VOID Crypt(struct CryptKeys *ck, CCHAR *data, INT len);
VOID Decrypt(struct CryptKeys *ck, CCHAR *data, INT len); 

#endif