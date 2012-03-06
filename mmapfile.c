#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "mmapfile.h"

#define DEBUG

#ifdef DEBUG
#define handle_error(msg) \
	do {perror(msg); exit(EXIT_FAILURE); } while (0)
#endif

int mmap_f_open(const char * file, struct mmap_f * mem)
{
	int fd;
	size_t len;
	char *addr;
	struct stat sb;
	
	if ((fd = open(file, O_RDWR)) == -1) {
#ifdef DEBUG
		handle_error("open file");
#endif
		return -1;
	}
	if (fstat(fd, &sb) == -1) {
#ifdef DEBUG
		handle_error("fstat");
#endif
		return -1;
	}
	len = sb.st_size;
	addr = mmap(NULL, len, PROT_READ|PROT_WRITE,
		MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED) {
#ifdef DEBUG
		handle_error("mmap");
#endif
		return -1;
	}
	
	mem->fd = fd;
	mem->p = addr;
	mem->len = len;
	
	return 0;
}

int mmap_f_close(struct mmap_f * mem)
{
	int fd = mem->fd;
	char *p = mem->p;
	size_t len = mem->len;
	
	if (msync(p, len, MS_ASYNC) == -1) {
#ifdef DEBUG
		handle_error("msync");
#endif
		return -1;
	}
	if (munmap(p, len) == -1) {
#ifdef DEBUG
		handle_error("munmap");
#endif
		return -1;
	}
	if (close(fd) == -1) {
#ifdef DEBUG
		handle_error("close file");
#endif
	return -1;
	}
	
	return 0;
}