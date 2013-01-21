#include "../json/json.h"
#include "JsonValue.h"








Json::Value& JsonValue::GetJValue()
{ 
	return m_JValue;
}




void JsonValue::SetValue(char* lpszKeyText,short uValue)
{
	m_JValue[lpszKeyText]	= uValue;
}

void JsonValue::SetValue(char* lpszKeyText,float uValue)
{
	m_JValue[lpszKeyText]	= uValue;
}

void JsonValue::SetValue(char* lpszKeyText,double uValue)
{
	m_JValue[lpszKeyText]	= uValue;
}

void JsonValue::SetValue(char* lpszKeyText,unsigned int uValue)
{
	m_JValue[lpszKeyText]	= uValue;
}

void JsonValue::SetValue(char* lpszKeyText,int uValue)
{
	m_JValue[lpszKeyText]	= uValue;
}

void JsonValue::SetValue(char* lpszKeyText,unsigned short uValue)
{
	m_JValue[lpszKeyText]	= uValue;
}

void JsonValue::SetValue(char* lpszKeyText,char* lpszValue)
{
	m_JValue[lpszKeyText]	= lpszValue;
}


void JsonValue::SetValue(JsonValue* lpJValue)
{
	m_JValue.append(lpJValue->GetJValue());	

} 

void JsonValue::SetValue(Json::Value& lpJValue)
{
	m_JValue.append(lpJValue);
}

void JsonValue::SetValue(char* lpszKeyText, JsonValue& lpJValue)
{
	m_JValue[lpszKeyText] = lpJValue.GetJValue();	
}

void JsonValue::SetValue(char* lpszKeyText, Json::Value& JsValue)
{
	m_JValue[lpszKeyText] = JsValue;	
}


void JsonValue::ClearData()
{
	m_JValue.clear();
}


void JsonValue::SetValue()
{
	Json::Value	lastValue;
	//	lastValue["eof"]	= NULL;
	m_JValue.append(lastValue);	

}





int JsonValue::IsEmpty()
{
	return m_JValue.isNull() ? 1 : 0;
}
