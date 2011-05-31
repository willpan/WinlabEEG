#include <openssl/aes.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uchar;

// Fill in actual key here
//RESEARCH KEY
static const uchar ckey[] = "\x31\x00\x39\x54\x38\x10\x37\x42\x31\x00\x39\x48\x38\x00\x37\x50";
//CONUSMER KEY
//static const uchar ckey[] = "\x31\x00\x35\x54\x38\x10\x37\x42\x31\x00\x35\x48\x38\x00\x37\x50"

void print(const char *str, uchar *in, uchar *out)
{
    // Print in hex because encrypted data is not always ASCII-friendly ;-)

    int i;
	
    printf("%s\n", str);
    //printf("Input:\t");
    for (i = 0; i < AES_BLOCK_SIZE*2; i++)
        printf("%x ", in[i]);
    printf("\n");
    //printf("Output:\t");
    for (i = 0; i < AES_BLOCK_SIZE*2; i++)
        printf("%x ", out[i]);    printf("\n\n");

}

void Encrypt16(uchar *in, uchar *out, int debug)
{
    static int firstRun = 1;
    static AES_KEY encryptKey;

    if (firstRun == 1)
    {
        AES_set_encrypt_key(ckey, 128, &encryptKey);
        firstRun = 0;
    }

    AES_ecb_encrypt(in, out, &encryptKey, AES_ENCRYPT);

    if (debug)
        print("Encryption:", in, out);
}

void Decrypt16(uchar *in, uchar *out, int debug)
{
    static int firstRun = 1;
    static AES_KEY decryptKey;

    if (firstRun == 1)
    {
        AES_set_decrypt_key(ckey, 256, &decryptKey);
        firstRun = 0;
    }

    AES_ecb_encrypt(in, out, &decryptKey, AES_DECRYPT);

    if (debug)
        print("", in, out);
}

void Decrypt32(uchar *in, uchar *out, int debug)
{
    static int firstRun = 1;
    static AES_KEY decryptKey;
	uchar tempin[16];
	uchar tempout[16];

    if (firstRun == 1)
    {
        AES_set_decrypt_key(ckey, 256, &decryptKey);
        firstRun = 0;
	}	
	//FIRST 16 BITS
	int i = 0;
	for(;i < AES_BLOCK_SIZE ; ++i)
	{
		tempin[i] = in[i];
	}	
	AES_ecb_encrypt(tempin, tempout, &decryptKey, AES_DECRYPT);
	for(i = 0;i < AES_BLOCK_SIZE ; ++i)
	{
		out[i] = tempout[i];
	}	
	//SECOND 16 BITS
	for(i = 0 ; i < AES_BLOCK_SIZE ; ++i)
	{
		tempin[i] = in[i+16];
	}	
	AES_ecb_encrypt(tempin, tempout, &decryptKey, AES_DECRYPT);
	for(i = 0 ; i < AES_BLOCK_SIZE ; ++i)
	{
		out[i+16] = tempout[i];
	}	
    if (debug)
        print("", in, out);
}