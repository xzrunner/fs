#ifdef __ANDROID__

#include "fs_android_helper.h"

#include <logger.h>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <stddef.h>
#include <string.h>
#include <assert.h>

struct fs_file {
	bool in_apk;

	AAsset* asset;	// in apk
	size_t offset;

	FILE* fp;	    // out apk
};

struct fs_file* 
fs_open(const char* path, const char* format) {
	if (!path) {
		LOGD("file open path err %s\n", path);		
		return NULL;
	}

	if (path[0] != '/') {
		AAssetManager* mgr = fs_get_asset_mgr();
		if (!mgr) {
			LOGD("android asset manager is NULL!\n");
			return NULL;
		}

		AAsset* asset = AAssetManager_open(mgr, path, AASSET_MODE_RANDOM);
		if (!asset) {
			LOGD("open file fail: %s\n", path);
			return NULL;
		}

		struct fs_file* f = malloc(sizeof(*f));
		f->in_apk = true;
		f->asset = asset;
		f->fp = NULL;
		f->offset = 0;
		return f;
	} else {
		if (!format) {
			LOGD("file format err %s\n", format);
			return NULL;
		}
		FILE* fp = fopen(path, format);
		if (!fp) {
			LOGD("open file fail: %s\n", path);
			return NULL;
		}
		
		struct fs_file* f = malloc(sizeof(*f));
		f->in_apk = false;
		f->asset = NULL;
		f->fp = fp;
		f->offset = 0;
		return f;
	}
}

void 
fs_close(struct fs_file* f) {
	if (!f) {
		return;
	}
	if (f->in_apk) {
		AAsset_close(f->asset);
	} else {
		fclose(f->fp);
	}
	free(f);
}

size_t 
fs_size(struct fs_file* f) {
	if (f->in_apk) {
		return AAsset_getLength(f->asset);
	} else {
		size_t save_pos = ftell(f->fp);
		fseek(f->fp, 0, SEEK_END);
		size_t sz = ftell(f->fp);
		fseek(f->fp, save_pos, SEEK_SET);
		return sz;
	}
}

int 
fs_read(struct fs_file* f, void* buffer, size_t size) {
	if (f->in_apk) {
		int ret = AAsset_read(f->asset, buffer, size);
		f->offset += ret;
		return ret;
	} else {
		return fread(buffer, 1, size, f->fp);
	}
}

int 
fs_write(struct fs_file* f, void* buffer, size_t size) {
	if (!f->in_apk)	{
		return fwrite(buffer, size, 1, f->fp);
	}
}

void 
fs_seek_from_cur(struct fs_file* f, int offset) {
	if (f->in_apk) {
		AAsset_seek(f->asset, offset, SEEK_CUR);
		f->offset += offset;
	} else {
		fseek(f->fp, offset, SEEK_CUR);
	}
}

void 
fs_seek_from_head(struct fs_file* f, int offset) {
	if (f->in_apk) {
		AAsset_seek(f->asset, offset, SEEK_SET);
		f->offset = offset;
	} else {
		fseek(f->fp, offset, SEEK_SET);
	}
}

int
fs_feof(struct fs_file* f) {
	if (f->in_apk) {
    	return (f->offset >= fs_size(f)) ? 1 : 0;
	} else {
		return feof(f->fp);
	}
}

#endif // __ANDROID__
