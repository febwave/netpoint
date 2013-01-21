

enum db_field_types { 
	DB_TYPE_DECIMAL
	,DB_TYPE_TINY
	,DB_TYPE_SHORT
	,DB_TYPE_LONG
	,DB_TYPE_FLOAT
	,DB_TYPE_DOUBLE
	,DB_TYPE_NULL
	,DB_TYPE_TIMESTAMP
	,DB_TYPE_LONGLONG
	,DB_TYPE_INT24
	,DB_TYPE_DATE
	,DB_TYPE_TIME
	,DB_TYPE_DATETIME
	,DB_TYPE_YEAR
	,DB_TYPE_NEWDATE
	,DB_TYPE_VARCHAR
	,DB_TYPE_BIT
	,DB_TYPE_NEWDECIMAL=246
	,DB_TYPE_ENUM=247
	,DB_TYPE_SET=248
	,DB_TYPE_TINY_BLOB=249
	,DB_TYPE_MEDIUM_BLOB=250
	,DB_TYPE_LONG_BLOB=251
	,DB_TYPE_BLOB=252
	,DB_TYPE_VAR_STRING=253
	,DB_TYPE_STRING=254
	,DB_TYPE_GEOMETRY=255
};




void	IMS_ClearDBError(DBErrorInfo* lpDBErrorInfo);
void	IMS_GetDBParamBindingCopy(DBParamBinding* lpSrcBinding,DBParamBinding* lpNewBindingCopy,int nParamCount);
void	IMS_GetDBRecordBindingCopy(DBRecordBinding* lpSrcBinding,DBRecordBinding* lpNewBindingCopy,int nParamCount);
void	IMS_FillDBParamBindingData(DBParamBinding* lpDBParamBinding1st,int uParamIndex, void* lpData,unsigned int uDataLen,int bIsNULLFlag);
void	IMS_InitDBParamBinding(DBParamBinding* lpNewBindingCopy,int nParamCount);
void	IMS_SetUpdateMode(DBParamBinding* lpDBParamBinding1st,unsigned int uParamIndex, unsigned int UpdateMode);
int	ConvertSystemTimeToMYSQLTime(struct tm* lpSystemtime,MYSQL_TIME* lpMysqlTime);



