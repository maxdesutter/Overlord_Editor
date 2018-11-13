#pragma once

class BinaryWriter
{
public:
	BinaryWriter();
	~BinaryWriter();

	void Open(std::wstring binaryFile);
	void Close();
	bool IsValid()
	{
		if (m_pFileWriter)
			return true;

		return false;
	}
	template<class T>
	void Write(T data)
	{
		if (m_pFileWriter == nullptr)
		{
			Logger::LogError(L"BinaryWriter doesn't exist!\nUnable to Write binary data...");
			return;
		}

		
		m_pFileWriter->write((char*)&data, sizeof(T));
		return;
	}
	void WriteString(std::string data);
	void WriteLongString(std::wstring data);

private:
	std::ofstream* m_pFileWriter = nullptr;
	bool m_Exists;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	BinaryWriter(const BinaryWriter& yRef);
	BinaryWriter& operator=(const BinaryWriter& yRef);
};

