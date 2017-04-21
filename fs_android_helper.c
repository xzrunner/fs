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
	AAssetManager* mgr = fs_get_asset_mgr();
	if (!mgr) {
		LOGD("android asset manager is NULL!\n");
		return NULL;
	}

	AAsset* file = AAssetManager_open(mgr, path, AASSET_MODE_BUFFER);
	if (!file) {
		LOGD("load file fail: %s\n", path);
		return NULL;
	}

	int len = AAsset_getLength(file);

	unsigned char* buffer = (char*)malloc(len);
	AAsset_read(file, buffer, len); 
	AAsset_close(file);

	if (size) {
		*size = len;
	}

	return buffer;
}

bool fs_is_file_exist(const char* path) {
	if (!ASSET_MGR) {
		LOGD("android asset manager is NULL!\n");
		return false;
	}

	AAsset* file = AAssetManager_open(ASSET_MGR, path, AASSET_MODE_BUFFER);
	return file == NULL ? false : true;
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