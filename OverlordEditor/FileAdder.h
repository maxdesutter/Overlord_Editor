#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>



class FileAdder
{
public:
	FileAdder();
	~FileAdder();
	ModelReflectionStruct* GetOvmFile();
	TextureStruct* GetTexture();
	//char m_filename[MAX_PATH];

};

