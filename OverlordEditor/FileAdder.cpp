#include "OverlordEditorPCH.h"
#include "FileAdder.h"
#include "OverlordGame.h"



#include <windows.h>      // For common windows data types and function headers
#include <shlobj.h>
#include <objbase.h>      // For COM headers
#include <shlwapi.h>

#include <strsafe.h>      // for StringCchPrintfW
#include <shtypes.h>      // for COMDLG_FILTERSPEC

FileAdder::FileAdder()
{

	
}


FileAdder::~FileAdder()
{
}

ModelReflectionStruct* FileAdder::GetOvmFile()
{
	auto settings = OverlordGame::GetGameSettings();
	LPWSTR wpath;

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	
	if (SUCCEEDED(hr))
	{
		IFileOpenDialog *pFileOpen;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
		COMDLG_FILTERSPEC spec = { L"overlord mesh",L"*.ovm" };
		pFileOpen->SetFileTypes(1, &spec);
		if (SUCCEEDED(hr))
		{
			// Show the Open dialog box.
			hr = pFileOpen->Show(settings.Window.WindowHandle);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &wpath);


				}
				pItem->Release();

			}
			else
			{
				pFileOpen->Release();
				CoUninitialize();
				return nullptr;
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}



	std::wstring path = wpath;
	std::wstring wname;


	int index = path.rfind('\\');
	wname = path.substr(index+1,path.length()-index-5);
	
	ModelReflectionStruct* str = new ModelReflectionStruct(std::string(wname.begin(),wname.end()),path);

	return str;
}

TextureStruct* FileAdder::GetTexture()
{
	auto settings = OverlordGame::GetGameSettings();
	LPWSTR wpath = L"";
	

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileOpenDialog *pFileOpen;


		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));


		
		

		COMDLG_FILTERSPEC spec[] = {
			{L"All Graphic types",L"*.jpg;*.bmp;*.dds;*.png;*.tga"},
			{ L"jpeg",L"*.jpg" } ,
			{ L"BMP",L"*.bmp" },
			{ L"DDS",L"*.dds" },
			{ L"png",L"*.png" },
			{ L"tga",L"*.tga" }
			
		
		};
		pFileOpen->SetFileTypes(6, spec);
		if (SUCCEEDED(hr))
		{
			// Show the Open dialog box.
			hr = pFileOpen->Show(settings.Window.WindowHandle);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &wpath);


					pItem->Release();
				}

			}
			else
			{
				pFileOpen->Release();
				CoUninitialize();
				return nullptr;
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}



	std::wstring path = wpath;
	std::wstring wname;


	int index = path.rfind('\\');
	wname = path.substr(index + 1, path.length() - index - 5);

	TextureStruct* str = new TextureStruct( path, std::string(wname.begin(), wname.end()));

	return str;
}







