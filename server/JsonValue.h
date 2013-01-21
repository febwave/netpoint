#pragma once


#define IMS_JSON_VALUE_NAME_LENGTH			8

class JsonValue
{
public:

	Json::Value&				GetJValue();
	char*						GetJValueName();	
	void						ClearData();

	void 						SetValue();
	void						SetValue(char* lpszKeyText, JsonValue& lpJValue);
	void						SetValue(Json::Value& lpJValue);
	void						SetValue(JsonValue* lpJValue);
	void 						SetValue(char* lpszKeyText,char* lpszValue);
	void						SetValue(char* lpszKeyText,int uValue);
	void 						SetValue(char* lpszKeyText,unsigned int uValue);
	void 						SetValue(char* lpszKeyText,unsigned short uValue);
	void						SetValue(char* lpszKeyText,long uValue);
	void						SetValue(char* lpszKeyText,short uValue);
	void						SetValue(char* lpszKeyText,float uValue);
	void						SetValue(char* lpszKeyText,double uValue);
	void						SetValue(char* lpszKeyText, Json::Value& JsValue);
public:
	int						IsEmpty();



private:
	Json::Value					m_JValue;



};
