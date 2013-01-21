#pragma once


class JsonValue;

typedef void* (*ppfnAllocMemory)(unsigned int size);
typedef void (*ppfnReleaseMemory)(void* lpBuffer);


class  JsonWriter
{
public:
	JsonWriter();
	void 			SetValue(char* lpszKeyText, JsonValue& lpJValue);
	void 			SetValue(JsonValue& lpJValue);
	void			SetValue(Json::Value& lpJValue);
	void 			SetValue();

	void 			SetValue(char* lpszKeyText,char* lpszValue);
	void 			SetValue(char* lpszKeyText,unsigned int uValue);
	void 			SetValue(char* lpszKeyText,int uValue);
	void 			SetValue(char* lpszKeyText,double uValue);
	void 			SetValue(char* lpszKeyText,float uValue);
	void 			SetValue(char* lpszKeyText,unsigned short uValue);
	void			SetValue(char* lpszKeyText,short uValue);
	void			SetValue(char* lpszKeyText,long uValue);
	void			SetValue(char* lpszKeyText, Json::Value& NewValue);


public:

	int			GetJsonData(ppfnAllocMemory pfnAllocMemory, void** lplpDataRet,unsigned int* lpuDataRetSize);
	int			GetJsonData2(ppfnAllocMemory pfnAllocMemory,unsigned int uDataOffset, void** lplpDataRet,unsigned int* lpuDataRetSize);
private:
	Json::Value		m_RootValue;

	std::string		m_strJsonDst;
};


