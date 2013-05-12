#include "pbc.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define COUNT 1000000

static void
read_file (const char *filename , struct pbc_slice *slice) {
	FILE *f = fopen(filename, "rb");
	if (f == NULL) {
		slice->buffer = NULL;
		slice->len = 0;
		return;
	}
	fseek(f,0,SEEK_END);
	slice->len = ftell(f);
	fseek(f,0,SEEK_SET);
	slice->buffer = malloc(slice->len);
	fread(slice->buffer, 1 , slice->len , f);
	fclose(f);
}

static void
test(struct pbc_env *env) {
	// int i;
	// for(i=0; i<COUNT; i++)
	// {
	// 		struct pbc_wmessage* w_msg = pbc_wmessage_new(env, "at");
	// 		struct pbc_rmessage* r_msg = NULL;
	// 		struct pbc_slice sl;
	// 		char buffer[1024];
	// 		sl.buffer = buffer, sl.len = 1024;
	// 		pbc_wmessage_integer(w_msg, "aa", 123, 0);
	// 		pbc_wmessage_integer(w_msg, "bb", 456, 0);
	// 		pbc_wmessage_string(w_msg, "cc", "test string!", 0);
	// 		pbc_wmessage_buffer(w_msg, &sl);

	// 		r_msg = pbc_rmessage_new(env, "at", &sl);

	// 		//===============
	// 		printf("aa = %d\n", pbc_rmessage_integer(r_msg, "aa", 0, NULL));
	// 		printf("bb = %d\n", pbc_rmessage_integer(r_msg, "bb", 0, NULL));
	// 		printf("cc = %s\n", pbc_rmessage_string(r_msg, "cc", 0, NULL));
	// 		//===============

	// 		pbc_rmessage_delete(r_msg);
	// 		pbc_wmessage_delete(w_msg);
	// }

	struct pbc_wmessage *w_msg = pbc_wmessage_new(env, "at");
	pbc_wmessage_integer(w_msg, "aa", 123, 0);
	pbc_wmessage_integer(w_msg, "bb", 456, 0); 
	pbc_wmessage_string(w_msg, "cc", "test string!", 0);

	struct pbc_slice slice;
	char buffer[1024];
	slice.len = 1024;
	slice.buffer = buffer;
	pbc_wmessage_buffer(w_msg, &slice);
	// pbc_wmessage_delete(w_msg);  //!!!!!!!! should not delete here

	struct pbc_rmessage *r_msg = pbc_rmessage_new(env, "at", &slice);
	printf("aa = %d\n", pbc_rmessage_integer(r_msg, "aa", 0, NULL));
	printf("bb = %d\n", pbc_rmessage_integer(r_msg, "bb", 0, NULL));
	printf("cc = %s\n", pbc_rmessage_string(r_msg, "cc", 0, NULL));

	pbc_rmessage_delete(r_msg);
	pbc_wmessage_delete(w_msg);
}

int
main() {
	_pbcM_memory();
	struct pbc_env * env = pbc_new();
	struct pbc_slice slice;
	read_file("test.pb", &slice);
	int ret = pbc_register(env, &slice);
	assert(ret == 0);
	test(env);
	pbc_delete(env);
	_pbcM_memory();

	return 0;
}