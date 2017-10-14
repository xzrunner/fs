#ifdef __cplusplus
extern "C"
{
#endif

#ifndef file_system_android_helper_h
#define file_system_android_helper_h

#include <android/asset_manager.h>

#include <stdbool.h>

void fs_set_asset_mgr(AAssetManager* mgr);
AAssetManager* fs_get_asset_mgr();

bool fs_get_file_size(const char *path, unsigned long* size);

unsigned char* fs_get_file_data(const char* path, 
								const char* mode, 
								unsigned long* size);

bool fs_is_file_exist(const char* path);

void fs_set_apk_path(const char* apk_path);
void fs_set_mem_path(const char* mem_path);
const char* fs_get_mem_path();

#endif // file_system_android_helper_h

#ifdef __cplusplus
}
#endif