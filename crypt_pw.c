//1.generate encrypted password through crypt(3)
//as /etc/shadow does, note that shell escape '$'
//character, salt should be 8 characters 
//formatted '$id$salt[$]',where id could be 1, 
//  5, 6...corresponding to md5, sha256, sha512...,
//the ending $ could be omitted
//example:progname password $6$
//2.usage: progname string_to_encrypt salt

#include <unistd.h>
#include <crypt.h>
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
	if (argc != 3){
		fprintf(stderr, "usage: progname string_to_encrypt salt.\n");
		exit(1);
	}
	char *key, *salt;
	key = argv[1];
	salt = argv[2];
	printf("key:%s\n", key);
	printf("salt:%s\n", salt);
	char *s = crypt(key, salt);
	if (s != NULL) printf("encrypted data:%s\n", s);
	else fprintf(stderr, "crypt failed.\n");

	return 0;
}
