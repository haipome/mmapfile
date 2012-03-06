#include <stdio.h>
#include "mmapfile.h"

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "usage: mmap filename\n");
		return -1;
	}
	
	struct mmap_f f;
	mmap_f_open(argv[1], &f);
	
	*((char *)f.p) += 1;
	
	mmap_f_close(&f);
	
	return 0;
}