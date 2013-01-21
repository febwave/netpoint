#pragma once



class JsonReader
{
public:
	JsonReader();
	int GetValue(char* lpszKeyText,unsigned short* lpuRetValue);
	int GetValue(char* lpszKeyText,unsigned int* lpuRetValue);
	int GetValue(char* lpszKeyText,Json::Value& newValue);
	int GetValue(char* lpszKeyText,char* lpszBuffer,int nBufferSize);
	
public:
	int		InitJsonData(void* lpData,unsigned int uDataSize);
	int		ExistMember(char* lpszKeyText);
private:
	Json::Value	m_RootValue;
	Json::Reader	m_JsonReader;
	std::string	m_strJsonSrc;
	
};


