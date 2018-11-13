#include "stdafx.h"
#include "BinaryWriter.h"


BinaryWriter::BinaryWriter()
{
}


BinaryWriter::~BinaryWriter()
{
	Close();
}

void BinaryWriter::Open(std::wstring binaryFile)
{
	Close();

	auto temp = new std::ofstream();
	temp->open(binaryFile, std::ios::binary);
	if (temp->is_open())
	{
		m_pFileWriter = temp;
		m_Exists = true;
	}
	else
	{
		std::wstringstream ss;
		ss << L"BinaryWriter::Open> Failed to open the file!\nBinaryFile: ";
		ss << binaryFile;
		Logger::LogWarning(ss.str());
		Close();
	}
}

void BinaryWriter::Close()
{
	m_Exists = false;
	SafeDelete(m_pFileWriter);
}

void BinaryWriter::WriteString(std::string data)
{
	
		if (m_pFileWriter == nullptr)
		{
			Logger::LogError(L"BinaryWriter doesn't exist!(writeString) \nUnable to write binary data...");
			return;
		}
		char length = (char)data.length();
		m_pFileWriter->write(&length, sizeof(char));
		m_pFileWriter->write(&data[0], length);

	
}
void BinaryWriter::WriteLongString(std::wstring data)
{
	if (m_pFileWriter == nullptr)
	{
		Logger::LogError(L"BinaryWriter doesn't exist!(writeString) \nUnable to write binary data...");
		return;
	}
	UINT length = (UINT)data.length();
	m_pFileWriter->write((char*)&length, sizeof(UINT));
	for (UINT i = 0; i < length; ++i)
	{
		m_pFileWriter->write((char*)&data[i], sizeof(wchar_t));

	}
}
