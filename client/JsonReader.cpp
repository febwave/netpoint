#include "../json/json.h"
#include "JsonReader.h"
#include <string.h>

JsonReader::JsonReader():m_JsonReader(Json::Features::strictMode())
{



}

int JsonReader::InitJsonData(void* lpData, unsigned int uDataSize)
{
	m_strJsonSrc			= std::string((char*)lpData,uDataSize);	
	if(m_strJsonSrc.empty() == 0 &&  m_JsonReader.parse(m_strJsonSrc,m_RootValue,0))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}



int JsonReader::GetValue(char* lpszKeyText,Json::Value& newValue)
{
	// default get value from root
	Json::Value curValue;
	if(m_RootValue.isMember(lpszKeyText))
	{
		curValue	= m_RootValue[lpszKeyText];
		if(curValue.isNull() == 0)
		{
			newValue = curValue;
			return 1;
		}
	}
	return 0;
}



int JsonReader::GetValue(char* lpszKeyText,unsigned short* lpuRetValue)
{
	Json::Value curValue;
	if(m_RootValue.isMember(lpszKeyText))
	{
		curValue	= m_RootValue[lpszKeyText];
		if(curValue.isNull() == 0)
		{
			*lpuRetValue	= curValue.asUInt();
			return 1;
		}
	}
	return  0;
}

int JsonReader::GetValue(char* lpszKeyText,unsigned int* lpuRetValue)
{
	Json::Value curValue;
	if(m_RootValue.isMember(lpszKeyText))
	{
		curValue	= m_RootValue[lpszKeyText];
		if(curValue.isNull() == 0)
		{
			*lpuRetValue	= curValue.asUInt();
			return 1;
		}
	}
	return  0;
}




int JsonReader::GetValue(char* lpszKeyText,char* lpszBuffer,int nBufferSize)
{
	// default get value from root
	Json::Value curValue;
	std::string curString;
	if(m_RootValue.isMember(lpszKeyText))
	{
		curValue	= m_RootValue[lpszKeyText];
		if(curValue.isNull() == 0)
		{
			curString	= curValue.asString();
			if(curString.length() < (size_t)nBufferSize)
			{
				strcpy(lpszBuffer,curString.c_str());
				return 1;
			}
			return 0;
		}
	}
	return 0;
}



int JsonReader::ExistMember(char* lpszKeyText)
{
	if(m_RootValue.isMember(lpszKeyText))
	{
		return 1;
	}
	return 0;
}
