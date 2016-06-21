#ifdef __ANDROID__

#include "fs_android_helper.h"

#include <logger.h>

#include <stddef.h>
#include <string.h>
#include <assert.h>

struct fs_file {
	unsigned long size;
	unsigned char* buffer;
	size_t offset;
};

struct fs_file* 
fs_open(const char* path, const char* format) {
	unsigned long sz;
	unsigned char* buffer;
	buffer = fs_get_file_data(path, format, &sz);
	if(buffer){
		struct fs_file* f = malloc(sizeof(*f));
		f->buffer = buffer;
		f->offset = 0;
		f->size = sz;
		return f;
	} else {
		return NULL;
	}
}

void 
fs_close(struct fs_file* f) {
	if(f == NULL) return;

	if(f->buffer) {
		free(f->buffer);
	}
	free(f);
}

size_t 
fs_size(struct fs_file* f) {
	return f->size;
}

int 
fs_read(struct fs_file* f, void* buffer, size_t size) {
	assert(f->size >= f->offset && f->size > 0 && f->buffer != NULL);
	size_t sz;
	int ret;
	if(f->offset + size <= f->size) {
		sz = size;
		ret = 1;
	} else {
		sz = f->size - f->offset;
		ret = 0;
	}
	if (sz > 0) {
		memcpy(buffer, f->buffer + f->offset, sz);
		f->offset += sz;
	} 
	return ret;
}

int 
fs_write(struct fs_file* f, void* buffer, size_t size) {
	// todo
}

void 
fs_seek_from_cur(struct fs_file* f, int offset) {
	f->offset += offset;
}

void 
fs_seek_from_head(struct fs_file* f, int offset) {
	f->offset = offset;
}

#endif // __ANDROID__