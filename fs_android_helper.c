#ifdef __ANDROID__

#include "fs_android_helper.h"

#include <logger.h>

#include <string.h>

static AAssetManager* ASSET_MGR = NULL;

static char APK_PATH[255];
static char MEM_PATH[255];

void 
fs_set_asset_mgr(AAssetManager* mgr) {
	ASSET_MGR = mgr;
}

AAssetManager* 
fs_get_asset_mgr() {
	return ASSET_MGR;
}

unsigned char* 
fs_get_file_data(const char* path, const char* mode, unsigned long* size) {
	if (!path) {
		return NULL;
	}
	if (path[0] != '/') {
		if (!ASSET_MGR) {
			LOGD("android asset manager is NULL!\n");
			return NULL;
		}

		AAsset* asset = AAssetManager_open(ASSET_MGR, path, AASSET_MODE_BUFFER);
		if (!asset) {
			LOGD("open asset fail: %s\n", path);
			return NULL;
		}

		int len = AAsset_getLength(asset);

		unsigned char* buffer = (char*)malloc(len);
		AAsset_read(asset, buffer, len); 
		AAsset_close(asset);

		if (size) {
			*size = len;
		}

		return buffer;
	} else {
		FILE* fp = fopen(path, "rb");
		if(!fp) {
		    LOGD("open file fail: %s\n", path);
		    return NULL;
		}
		fseek(fp, 0, SEEK_END);
		long length = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* buffer = malloc(length);
	    if(!buffer) {
		    fclose(fp);
			LOGD("read file no memory: %s\n", path);
		    return NULL;
		}
		fread(buffer, 1, length, fp);
		fclose(fp);
		return buffer;
	}
}

bool 
fs_is_file_exist(const char* path) {
	if (!path) {
		return false;
	}	
	if (path[0] != '/') {
		if (!ASSET_MGR) {
			LOGD("android asset manager is NULL!\n");
			return false;
		}
		AAsset* asset = AAssetManager_open(ASSET_MGR, path, AASSET_MODE_BUFFER);
		if (asset) {
			AAsset_close(asset);
			return true;
		}
		return false;
	} else {
	    FILE* file;
	    if (file = fopen(path, "rb")) {
	        fclose(file);
	        return true;
	    }
	    return false;
	}
}

void 
fs_set_apk_path(const char* apk_path) {
	strcpy(APK_PATH, apk_path);
}

void 
fs_set_mem_path(const char* mem_path) {
	strcpy(MEM_PATH, mem_path);
}

const char* 
fs_get_mem_path() {
	return MEM_PATH;
}

#endif // __ANDROID__