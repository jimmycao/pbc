#include "pbc.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define COUNT 1000000

static void read_file(const char *filename, struct pbc_slice *slice) 
{
	FILE *fp = NULL;
	if (NULL == (fp = fopen(filename, "rb"))) {
		perror("fopen");
		exit (1);
	}

	fseek(fp, 0, SEEK_END);
	slice->len = ftell(fp);

	fseek(fp, 0, SEEK_SET);
	slice->buffer = malloc(slice->len);
	fread(slice->buffer, 1, slice->len, fp);

	fclose(fp);
}

/*
struct pbc_wmessage * pbc_wmessage_new(struct pbc_env * env, const char *typename);
void pbc_wmessage_delete(struct pbc_wmessage *);

// for negative integer, pass -1 to hi
int pbc_wmessage_integer(struct pbc_wmessage *, const char *key, uint32_t low, uint32_t hi);
int pbc_wmessage_real(struct pbc_wmessage *, const char *key, double v);
int pbc_wmessage_string(struct pbc_wmessage *, const char *key, const char * v, int len);
struct pbc_wmessage * pbc_wmessage_message(struct pbc_wmessage *, const char *key);
void * pbc_wmessage_buffer(struct pbc_wmessage *, struct pbc_slice * slice);
*/

/*
struct pbc_rmessage * pbc_rmessage_new(struct pbc_env * env, const char * typename , struct pbc_slice * slice);
void pbc_rmessage_delete(struct pbc_rmessage *);

uint32_t pbc_rmessage_integer(struct pbc_rmessage * , const char *key , int index, uint32_t *hi);
double pbc_rmessage_real(struct pbc_rmessage * , const char *key , int index);
const char * pbc_rmessage_string(struct pbc_rmessage * , const char *key , int index, int *sz);
struct pbc_rmessage * pbc_rmessage_message(struct pbc_rmessage *, const char *key, int index);
int pbc_rmessage_size(struct pbc_rmessage *, const char *key);
int pbc_rmessage_next(struct pbc_rmessage *, const char **key);
*/

static void write_read_test(struct pbc_env *env, struct pbc_slice *slice)
{
	struct pbc_wmessage *w_msg = pbc_wmessage_new(env, "at");

	pbc_wmessage_integer(w_msg, "aa", 123, 0);
	pbc_wmessage_integer(w_msg, "bb", 456, 0);
	pbc_wmessage_string(w_msg, "cc", "haha!", 0);

	pbc_wmessage_buffer(w_msg, slice);

	struct pbc_rmessage *r_msg = pbc_rmessage_new(env, "at", slice);

	printf("aa = %d\n", pbc_rmessage_integer(r_msg, "aa", 0, NULL));
	printf("bb = %d\n", pbc_rmessage_integer(r_msg, "bb", 0, NULL));
	printf("cc = %s\n", pbc_rmessage_string(r_msg, "cc", 0, NULL));
	
	pbc_wmessage_delete(w_msg);
	pbc_rmessage_delete(r_msg);
}

int
main() {
	_pbcM_memory();
	struct pbc_env * env = pbc_new();
	struct pbc_slice slice;
	read_file("test.pb", &slice);
	int ret = pbc_register(env, &slice);
	assert(ret == 0);
	
	struct pbc_slice tmp_slice;
	char buffer[1024];
	tmp_slice.buffer = buffer;
	tmp_slice.len = 1024;

	write_read_test(env, &tmp_slice);

	pbc_delete(env);
	_pbcM_memory();

	return 0;
}
