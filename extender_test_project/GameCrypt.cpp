#include "stdafx.h"

CryptKeys gs_crypt;
CryptKeys client_crypt;

VOID SetKey(CCHAR *key, struct CryptKeys *ck) {
	ck->in_key[0] = key[0];
	ck->in_key[1] = key[1];
	ck->in_key[2] = key[2];
	ck->in_key[3] = key[3];
	ck->in_key[4] = key[4];
	ck->in_key[5] = key[5];
	ck->in_key[6] = key[6];
	ck->in_key[7] = key[7];
	ck->in_key[8] = key[8];
	ck->in_key[9] = key[9];
	ck->in_key[10] = key[10];
	ck->in_key[11] = key[11];
	ck->in_key[12] = key[12];
	ck->in_key[13] = key[13];
	ck->in_key[14] = key[14];
	ck->in_key[15] = key[15];

	ck->out_key[0] = key[0];
	ck->out_key[1] = key[1];
	ck->out_key[2] = key[2];
	ck->out_key[3] = key[3];
	ck->out_key[4] = key[4];
	ck->out_key[5] = key[5];
	ck->out_key[6] = key[6];
	ck->out_key[7] = key[7];
	ck->out_key[8] = key[8];
	ck->out_key[9] = key[9];
	ck->out_key[10] = key[10];
	ck->out_key[11] = key[11];
	ck->out_key[12] = key[12];
	ck->out_key[13] = key[13];
	ck->out_key[14] = key[14];
	ck->out_key[15] = key[15];

	//memcpy(ck->in_key,key,16);
	//memcpy(ck->out_key,key,16);
	ck->enabled = TRUE;
}

VOID Crypt(struct CryptKeys *ck, CCHAR *data, INT len) {
	if(!ck->enabled)
		return;

	int temp = 0;
	for (int i = 0; i < len; i++) {
		int temp2 = data[i] & 0xFF;
		temp = temp2 ^ ck->out_key[i & 15] ^ temp;
		data[i] = (CCHAR) temp;
	}
	
	int old = ck->out_key[8] & 0xff;
	old |= ck->out_key[9] << 8 & 0xff00;
	old |= ck->out_key[10] << 0x10 & 0xff0000;
	old |= ck->out_key[11] << 0x18 & 0xff000000;
	
	old += len;
	
	ck->out_key[8] = (CCHAR) (old & 0xff);
	ck->out_key[9] = (CCHAR) (old >> 0x08 & 0xff);
	ck->out_key[10] = (CCHAR) (old >> 0x10 & 0xff);
	ck->out_key[11] = (CCHAR) (old >> 0x18 & 0xff);
}

VOID Decrypt(struct CryptKeys *ck, CCHAR *data, INT len) {
	if(!ck->enabled)
		return;

	int temp = 0;
	for (int i = 0; i < len; i++) {
		int temp2 = data[i] & 0xFF;
		data[i] = (CCHAR) (temp2 ^ ck->in_key[i & 15] ^ temp);
		temp = temp2;
	}

	int old = ck->in_key[8] & 0xff;
	old |= ck->in_key[9] << 8 & 0xff00;
	old |= ck->in_key[10] << 0x10 & 0xff0000;
	old |= ck->in_key[11] << 0x18 & 0xff000000;
	
	old += len;
	
	ck->in_key[8] = (CCHAR) (old & 0xff);
	ck->in_key[9] = (CCHAR) (old >> 0x08 & 0xff);
	ck->in_key[10] = (CCHAR) (old >> 0x10 & 0xff);
	ck->in_key[11] = (CCHAR) (old >> 0x18 & 0xff);
}