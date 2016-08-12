#ifndef __ANDROID__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct fs_file {
	FILE* fp;
};

struct fs_file* 
fs_open(const char* path, const char* format) {
	FILE* fp = fopen(path, format);
	if (fp) {
		struct fs_file* ret = malloc(sizeof(*ret));
		ret->fp = fp;
		return ret;
	} else {
		return NULL;
	}
}

void 
fs_close(struct fs_file* f) {
	if (f == NULL) return;

	if (f->fp) {
		fclose(f->fp);
	}
	free(f);
}

size_t 
fs_size(struct fs_file* f) {
	size_t save_pos = ftell(f->fp);
	fseek(f->fp, 0, SEEK_END);
	size_t sz = ftell(f->fp);
	fseek(f->fp, save_pos, SEEK_SET);
	return sz;
}

int 
fs_read(struct fs_file* f, void* buffer, size_t size) {
	return fread(buffer, 1, size, f->fp);
}

int 
fs_write(struct fs_file* f, void* buffer, size_t size) {
	return fwrite(buffer, size, 1, f->fp);
}

void 
fs_seek_from_cur(struct fs_file* f, int offset) {
	fseek(f->fp, offset, SEEK_CUR);
}

void
fs_seek_from_head(struct fs_file* f, int offset) {
	fseek(f->fp, offset, SEEK_SET);
}

fs_feof(struct fs_file* f) {
    return feof(f->fp);
}

#endif // __ANDROID__
