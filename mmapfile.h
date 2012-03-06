
struct mmap_f {
	int fd;
	void * p;
	size_t len;
};

extern int mmap_f_open(const char * file, struct mmap_f * mem);
extern int mmap_f_close(struct mmap_f * mem);