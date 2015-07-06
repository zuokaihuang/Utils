#include <crypto/md5helper.h>
#include <crypto/md5.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char* MD5Helper::md5 (const char *in){
    if (!in) return nullptr;

    unsigned int len = strlen (in);

    MD5_CTX mdContext;

    MD5Init (&mdContext);
    MD5Update (&mdContext, (unsigned char*)in, len);
    MD5Final (&mdContext);

    const size_t size = 32;
    char* buffer = (char*) malloc(size);
    for (int i = 0; i < 16; i++)
        snprintf (buffer+i*2, size, "%02x", mdContext.digest[i]);

    return buffer;
}
