#ifdef __cplusplus
extern "C"
{
#endif

#ifndef file_system_android_helper_h
#define file_system_android_helper_h

#include <stdbool.h>

void fs_set_apk_path(const char* apk_path);
void fs_set_mem_path(const char* mem_path);
const char* fs_get_mem_path();

bool fs_is_file_exist(const char* file);

unsigned char* fs_get_file_data(const char* filename, 
								const char* mode, 
								unsigned long* size);

#endif // file_system_android_helper_h

#ifdef __cplusplus
}
#endif