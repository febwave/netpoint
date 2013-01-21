#include "../json/json.h"
#include "JsonWriter.h"
#include "JsonValue.h"
#include <string.h>





JsonWriter::JsonWriter()
{
	
}

int JsonWriter::GetJsonData2(ppfnAllocMemory pfnAllocMemory, unsigned int uDataOffset, void** lplpDataRet,unsigned int* lpuDataRetSize)
{
	Json::FastWriter 		jsonWriter;
	std::string				curString;
	void*					pData		= NULL;
	unsigned int					uDataSize	= 0;
	if(m_RootValue.isNull() == 1 || m_RootValue.empty() == 1)
	{
		*lpuDataRetSize = 0;
		return 1;
	}
	curString		= jsonWriter.write(m_RootValue);
	uDataSize		= (unsigned int)curString.length();
	pData			= pfnAllocMemory(uDataSize+uDataOffset);
	if(pData == NULL) return 0;
	memcpy((char*)pData+uDataOffset,curString.c_str(),uDataSize);
	*lplpDataRet	= pData;
	*lpuDataRetSize	= uDataSize+uDataOffset;
	return 1;
}

int JsonWriter::GetJsonData(ppfnAllocMemory pfnAllocMemory, void** lplpDataRet,unsigned int* lpuDataRetSize)
{
	Json::FastWriter 		jsonWriter;
	std::string				curString;
	void*					pData		= NULL;
	unsigned int					uDataSize	= 0;
	if(m_RootValue.isNull() == 1 || m_RootValue.empty() == 1)
	{
		*lpuDataRetSize = 0;
		return 1;
	}
	curString	= jsonWriter.write(m_RootValue);
	uDataSize	= (unsigned int)curString.length();
	pData		= pfnAllocMemory(uDataSize);
	if(pData == NULL) return 0;
	memcpy(pData,curString.c_str(),uDataSize);
	*lplpDataRet	= pData;
	*lpuDataRetSize	= uDataSize;
	return 1;
}




void JsonWriter::SetValue(char* lpszKeyText,unsigned int uValue)
{
	m_RootValue[lpszKeyText]	= uValue;
}

void JsonWriter::SetValue(char* lpszKeyText,unsigned short uValue)
{
	m_RootValue[lpszKeyText]	= uValue;
}

void JsonWriter::SetValue(char* lpszKeyText,char* lpszValue)
{
	m_RootValue[lpszKeyText]	= lpszValue;
}

void JsonWriter::SetValue(char* lpszKeyText, JsonValue& lpJValue)
{
	m_RootValue[lpszKeyText] = lpJValue.GetJValue();	
}


void JsonWriter::SetValue(char* lpszKeyText, Json::Value& NewValue)
{
	m_RootValue[lpszKeyText] = NewValue;	
}


void JsonWriter::SetValue(JsonValue& lpJValue)
{
	m_RootValue.append(lpJValue.GetJValue());	
}

void JsonWriter::SetValue(Json::Value& lpJValue)
{
	m_RootValue.append(lpJValue);	
}

void JsonWriter::SetValue()
{
	Json::Value	lastValue;
	m_RootValue.append(lastValue);	

}




void JsonWriter::SetValue(char* lpszKeyText,short uValue)
{
	m_RootValue[lpszKeyText]	= uValue;
}



void JsonWriter::SetValue(char* lpszKeyText,int uValue)
{
	m_RootValue[lpszKeyText]	= uValue;
}

void JsonWriter::SetValue(char* lpszKeyText,double uValue)
{
	m_RootValue[lpszKeyText]	= uValue;
}

void JsonWriter::SetValue(char* lpszKeyText,float uValue)
{
	m_RootValue[lpszKeyText]	= uValue;
}






