#include "fs_android_helper.h"

#include <unzip.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static char APKPATH[255];

void 
fs_set_apk_path(const char* apk_path) {
	strcpy(APKPATH, apk_path);
}

bool 
fs_is_file_exist(const char* file) {
	if (!APKPATH || !file) return false;

	if (file[0] != '/')
	{
		// read from apk
		char filepath[255] = "assets/";

		if (!APKPATH) return false;
		unzFile pFile = unzOpen(APKPATH);
		if (!pFile) return false;
		int nRet = unzLocateFile(pFile, strcat(filepath, file), 1);
		if (UNZ_OK != nRet) return false;
		unzClose(pFile);
		return true;
	}
	else
	{
		// read from other path than user set it
		FILE *fp = fopen(file, "rb");
		if (fp) {
			fclose(fp);
			return true;
		} else {
			return false; 
		}
	}

	return false;
}

static unsigned char* 
fs_get_file_data_from_zip(const char* zipfilepath, const char* filename, unsigned long* size)
{
	unsigned char * buffer = NULL;
	unzFile file = NULL;
	*size = 0;

	do 
	{
		if (!zipfilepath || !filename) break;

		if (strlen(zipfilepath) == 0) break;

		file = unzOpen(zipfilepath);
		if (!file) break;

		int nRet = unzLocateFile(file, filename, 1);
		if (UNZ_OK != nRet) break;

		char szFilePathA[260];
		unz_file_info FileInfo;
		nRet = unzGetCurrentFileInfo(file, &FileInfo, szFilePathA, sizeof(szFilePathA), NULL, 0, NULL, 0);
		if (UNZ_OK != nRet) break;

		nRet = unzOpenCurrentFile(file);
		if (UNZ_OK != nRet) break;

		buffer = (unsigned char*)malloc(FileInfo.uncompressed_size);
		if(!buffer) {
			return NULL;
		}
		int nSize = 0;
		nSize = unzReadCurrentFile(file, buffer, FileInfo.uncompressed_size);

		*size = FileInfo.uncompressed_size;

		unzCloseCurrentFile(file);

	} while (0);

	if (file)
	{
		unzClose(file);
	}

	return buffer;
}

unsigned char* 
fs_get_file_data(const char* filename, const char* mode, unsigned long* size) {
	unsigned char* data = 0;

	if ((! filename) || (! mode))
	{
		return NULL;
	}

	if (filename[0] != '/')
	{
		// read from apk
		char filepath[255] = "assets/";
		data =  fs_get_file_data_from_zip(APKPATH, strcat(filepath, filename), size);
	}
	else
	{
		do 
		{
			// read from other path than user set it
			FILE *fp = fopen(filename, mode);
			if (!fp) break;

			unsigned long sz;
			fseek(fp,0,SEEK_END);
			sz = ftell(fp);
			fseek(fp,0,SEEK_SET);
			data = (unsigned char*) malloc(sz);
			if(!data) {
				return NULL;
			}
			sz = fread(data,sizeof(unsigned char), sz,fp);
			fclose(fp);

			if (size)
			{
				*size = sz;
			}            
		} while (0);        
	}

	return data;
}
