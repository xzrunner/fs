#ifdef __cplusplus
extern "C"
{
#endif

#ifndef file_system_file_h
#define file_system_file_h

#include <stddef.h>

struct fs_file;

struct fs_file* fs_open(const char* path, const char* format);
void fs_close(struct fs_file* f);

size_t fs_size(struct fs_file* f);

int fs_read(struct fs_file* f, void* buffer, size_t size);
int fs_write(struct fs_file* f, void* buffer, size_t size);

void fs_seek_from_cur(struct fs_file* f, int offset);
void fs_seek_from_head(struct fs_file* f, int offset);

int fs_feof(struct fs_file* f);

#endif // file_system_file_h

#ifdef __cplusplus
}
#endif
