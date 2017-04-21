#ifdef __ANDROID__

#include "fs_android_helper.h"

#include <logger.h>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <stddef.h>
#include <string.h>
#include <assert.h>

struct fs_file {
	AAsset* file;
	size_t offset;
};

struct fs_file* 
fs_open(const char* path, const char* format) {
	AAssetManager* mgr = fs_get_asset_mgr();
	if (!mgr) {
		LOGD("android asset manager is NULL!\n");
		return NULL;
	}

	AAsset* file = AAssetManager_open(mgr, path, AASSET_MODE_RANDOM);
	if (!file) {
		LOGD("load file fail: %s\n", path);
		return NULL;
	}

	struct fs_file* f = malloc(sizeof(*f));
	f->file = file;
	f->offset = 0;
	return f;
}

void 
fs_close(struct fs_file* f) {
	if (!f) {
		return;
	}
	AAsset_close(f->file);
	free(f);
}

size_t 
fs_size(struct fs_file* f) {
	return AAsset_getLength(f->file);
}

int 
fs_read(struct fs_file* f, void* buffer, size_t size) {
	int ret = AAsset_read(f->file, buffer, size);
	f->offset += ret;
	return ret;
}

int 
fs_write(struct fs_file* f, void* buffer, size_t size) {
	// todo
}

void 
fs_seek_from_cur(struct fs_file* f, int offset) {
	AAsset_seek(f->file, offset, SEEK_CUR);
	f->offset += offset;
}

void 
fs_seek_from_head(struct fs_file* f, int offset) {
	AAsset_seek(f->file, offset, SEEK_SET);
	f->offset = offset;
}

int
fs_feof(struct fs_file* f) {
    return (f->offset >= fs_size(f)) ? 1 : 0;
}

#endif // __ANDROID__
