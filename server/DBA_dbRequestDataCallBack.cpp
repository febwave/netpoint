


#define DB_STRING_BUFFER_LENGTH			65535




unsigned int db_Request_Data_CallBack(void* hDBWorkItemInfo,void* hUserBuffer, unsigned int uDBRecordBinding,void* lpDBRecord ,unsigned int uDBEncoding,char* lpDataBuffer)
{
	unsigned int				hRetCode				= 0;
	LPDBRecordBinding		pDBRecord				= (LPDBRecordBinding)lpDBRecord;
	DBDataCallBackInfo*		pDBDataCallBackInfo		= (DBDataCallBackInfo*)hUserBuffer;
	JsonWriter*				pJsonWriter				= pDBDataCallBackInfo->pJsonWriter;
	JsonValue*				pCurNode				= pDBDataCallBackInfo->pCurNode;
	JsonValue*				pArNode					= pDBDataCallBackInfo->pArNode;
	JsonValue				curJValue;
	char					szBuffer[DB_STRING_BUFFER_LENGTH];

	// if lpDataBuffer is empty ,then it is end of recordset 
	if(lpDataBuffer != NULL)
	{
		for (unsigned int uBindIndex = 0;uBindIndex< uDBRecordBinding;uBindIndex++)
		{
			ZeroMemory(szBuffer,DB_STRING_BUFFER_LENGTH);
			if(pDBRecord->uRetColSize > 0)
			{
				switch (pDBRecord->uDataType)
				{
				case DB_TYPE_DECIMAL:
					if(pDBDataCallBackInfo->bMultiRows == FALSE)
					{
						if(pCurNode != NULL)
						{
							pCurNode->SetValue(pDBRecord->szDataID,*(float*)(lpDataBuffer + pDBRecord->uRetColOffSet));
						}
						else
						{
							pJsonWriter->SetValue(pDBRecord->szDataID,*(float*)(lpDataBuffer + pDBRecord->uRetColOffSet));
						}
					}
					else
					{
						curJValue.SetValue(pDBRecord->szDataID,*(float*)(lpDataBuffer + pDBRecord->uRetColOffSet));
					}
					break;
				case DB_TYPE_TINY:
					if(pDBDataCallBackInfo->bMultiRows == FALSE)
					{
						if(pCurNode != NULL)
						{
							pCurNode->SetValue(pDBRecord->szDataID,*(unsigned int*)(lpDataBuffer + pDBRecord->uRetColOffSet));
						}
						else
						{
							pJsonWriter->SetValue(pDBRecord->szDataID,*(unsigned int*)(lpDataBuffer + pDBRecord->uRetColOffSet));
						}
					}
					else
					{
						curJValue.SetValue(pDBRecord->szDataID,*(unsigned int*)(lpDataBuffer + pDBRecord->uRetColOffSet));
					}
					break;
				case DB_TYPE_SHORT:
					if(pDBDataCallBackInfo->bMultiRows == FALSE)
					{
						if(pCurNode != NULL)
						{
							pCurNode->SetValue(pDBRecord->szDataID,*(short*)(lpDataBuffer + pDBRecord->uRetColOffSet));
						}
						else
						{
							pJsonWriter->SetValue(pDBRecord->szDataID,*(short*)(lpDataBuffer + pDBRecord->uRetColOffSet));
						}
					}
					else
					{
						curJValue.SetValue(pDBRecord->szDataID,*(short*)(lpDataBuffer + pDBRecord->uRetColOffSet));
					}
					break;
				case DB_TYPE_LONG:
					if(pDBDataCallBackInfo->bMultiRows == FALSE)
					{
						if(pCurNode != NULL)
						{
							pCurNode->SetValue(pDBRecord->szDataID,*(long*)(lpDataBuffer + pDBRecord->uRetColOffSet));
						}
						else
						{
							pJsonWriter->SetValue(pDBRecord->szDataID,*(long*)(lpDataBuffer + pDBRecord->uRetColOffSet));
						}
					}
					else
					{
						curJValue.SetValue(pDBRecord->szDataID,*(long*)(lpDataBuffer + pDBRecord->uRetColOffSet));
					}
					break;
				case DB_TYPE_FLOAT:
					if(pDBDataCallBackInfo->bMultiRows == FALSE)
					{
						if(pCurNode != NULL)
						{
							pCurNode->SetValue(pDBRecord->szDataID,*(float*)(lpDataBuffer + pDBRecord->uRetColOffSet));
						}
						else
						{
							pJsonWriter->SetValue(pDBRecord->szDataID,*(float*)(lpDataBuffer + pDBRecord->uRetColOffSet));
						}
					}
					else
					{
						curJValue.SetValue(pDBRecord->szDataID,*(float*)(lpDataBuffer + pDBRecord->uRetColOffSet));
					}
					break;
				case DB_TYPE_DOUBLE:
					if(pDBDataCallBackInfo->bMultiRows == FALSE)
					{
						if(pCurNode != NULL)
						{
							pCurNode->SetValue(pDBRecord->szDataID,*(double*)(lpDataBuffer + pDBRecord->uRetColOffSet));
						}
						else
						{
							pJsonWriter->SetValue(pDBRecord->szDataID,*(double*)(lpDataBuffer + pDBRecord->uRetColOffSet));
						}
					}
					else
					{
						curJValue.SetValue(pDBRecord->szDataID,*(double*)(lpDataBuffer + pDBRecord->uRetColOffSet));
					}
					break;
				case DB_TYPE_NULL:
					break;
				case DB_TYPE_TIMESTAMP:
					memcpy(szBuffer,DB_STRING_BUFFER_LENGTH,lpDataBuffer + pDBRecord->uRetColOffSet,pDBRecord->uRetColSize);
					if(pDBDataCallBackInfo->bMultiRows == 0)
					{
						if(pCurNode != NULL)
						{
							pCurNode->SetValue(pDBRecord->szDataID,szBuffer);
						}
						else
						{
							pJsonWriter->SetValue(pDBRecord->szDataID,szBuffer);
						}
					}
					else
					{
						curJValue.SetValue(pDBRecord->szDataID,szBuffer);
					}
					break;
				case DB_TYPE_LONGLONG:
				case DB_TYPE_INT24:
					if(pDBDataCallBackInfo->bMultiRows == FALSE)
					{
						if(pCurNode != NULL)
						{
							pCurNode->SetValue(pDBRecord->szDataID,*(unsigned int*)(lpDataBuffer + pDBRecord->uRetColOffSet));
						}
						else
						{
							pJsonWriter->SetValue(pDBRecord->szDataID,*(unsigned int*)(lpDataBuffer + pDBRecord->uRetColOffSet));
						}
					}
					else
					{
						curJValue.SetValue(pDBRecord->szDataID,*(unsigned int*)(lpDataBuffer + pDBRecord->uRetColOffSet));
					}
					break;
					break;
				case DB_TYPE_DATE:
				case DB_TYPE_TIME:
				case DB_TYPE_DATETIME:
				case DB_TYPE_YEAR:
				case DB_TYPE_NEWDATE:
				case DB_TYPE_VARCHAR:
					memcpy(szBuffer,DB_STRING_BUFFER_LENGTH,lpDataBuffer + pDBRecord->uRetColOffSet,pDBRecord->uRetColSize);
					if(pDBDataCallBackInfo->bMultiRows == FALSE)
					{
						if(pCurNode != NULL)
						{
							pCurNode->SetValue(pDBRecord->szDataID,szBuffer);
						}
						else
						{
							pJsonWriter->SetValue(pDBRecord->szDataID,szBuffer);
						}
					}
					else
					{
						curJValue.SetValue(pDBRecord->szDataID,szBuffer);
					}
					break;
				case DB_TYPE_BIT:
					if(pDBDataCallBackInfo->bMultiRows == 0)
					{
						if(pCurNode != NULL)
						{
							pCurNode->SetValue(pDBRecord->szDataID,*(unsigned int*)(lpDataBuffer + pDBRecord->uRetColOffSet));
						}
						else
						{
							pJsonWriter->SetValue(pDBRecord->szDataID,*(unsigned int*)(lpDataBuffer + pDBRecord->uRetColOffSet));
						}
					}
					else
					{
						curJValue.SetValue(pDBRecord->szDataID,*(unsigned int*)(lpDataBuffer + pDBRecord->uRetColOffSet));
					}
					break;
				case DB_TYPE_NEWDECIMAL:
					if(pDBDataCallBackInfo->bMultiRows == 0)
					{
						if(pCurNode != NULL)
						{
							pCurNode->SetValue(pDBRecord->szDataID,*(double*)(lpDataBuffer + pDBRecord->uRetColOffSet));
						}
						else
						{
							pJsonWriter->SetValue(pDBRecord->szDataID,*(double*)(lpDataBuffer + pDBRecord->uRetColOffSet));
						}
					}
					else
					{
						curJValue.SetValue(pDBRecord->szDataID,*(double*)(lpDataBuffer + pDBRecord->uRetColOffSet));
					}
					break;
				case DB_TYPE_ENUM:
				case DB_TYPE_SET:
				case DB_TYPE_TINY_BLOB:
				case DB_TYPE_MEDIUM_BLOB:
				case DB_TYPE_LONG_BLOB:
				case DB_TYPE_BLOB:
				case DB_TYPE_VAR_STRING:
				case DB_TYPE_STRING:
				case DB_TYPE_GEOMETRY:
					memcpy(szBuffer,DB_STRING_BUFFER_LENGTH,lpDataBuffer + pDBRecord->uRetColOffSet,pDBRecord->uRetColSize);
					if(pDBDataCallBackInfo->bMultiRows == FALSE)
					{
						if(pCurNode != NULL)
						{
							if(lstrcmpiA(JKEY_SVR_JSON_OBJ,pDBRecord->szDataID))
							{
								pCurNode->SetValue(pDBRecord->szDataID,szBuffer);
							}
							else
							{
								Json::Reader	jsonReader;
								Json::Value	tempValue;
								std::string	strJsonSrc;
								strJsonSrc		= std::string((char*)szBuffer,lstrlenA(szBuffer));	
								if(jsonReader.parse(strJsonSrc,tempValue,false) == true)
								{
									pCurNode->SetValue(tempValue);
								}

							}
						}
						else
						{
							if(lstrcmpiA(JKEY_SVR_JSON_OBJ,pDBRecord->szDataID))
							{
								pJsonWriter->SetValue(pDBRecord->szDataID,szBuffer);
							}
							else
							{
								Json::Reader	jsonReader;
								Json::Value		tempValue;
								std::string		strJsonSrc;
								strJsonSrc		= std::string((char*)szBuffer,lstrlenA(szBuffer));	
								if(jsonReader.parse(strJsonSrc,tempValue,false) == true)
								{
									pJsonWriter->SetValue(JKEY_SVR_JSON_OBJ,tempValue);
								}
							}
						}
					}
					else
					{
						if(lstrcmpiA(JKEY_SVR_JSON_OBJ,pDBRecord->szDataID))
						{
							curJValue.SetValue(pDBRecord->szDataID,szBuffer);
						}
						else
						{
							Json::Reader	jsonReader;
							Json::Value		tempValue;
							std::string		strJsonSrc;
							strJsonSrc		= std::string((char*)szBuffer,lstrlenA(szBuffer));	
							if(jsonReader.parse(strJsonSrc,tempValue,false) == true)
							{
								curJValue.SetValue(JKEY_SVR_JSON_OBJ,tempValue);
							}
						}
					}
					break;
				}
			}
			pDBRecord++;
		}
		if(pDBDataCallBackInfo->bMultiRows)
		{
			pArNode->SetValue(&curJValue);
		}
	}
	else
	{
		if(pDBDataCallBackInfo->bMultiRows && pDBDataCallBackInfo->bInsertLastFlag)
		{
			// 标识多行结束
			if(pCurNode != NULL)
			{
				pCurNode->SetValue();
				pCurNode->SetValue(pDBDataCallBackInfo->szDataID,*pArNode);
			}
			else
			{
				pArNode->SetValue();
				pJsonWriter->SetValue(pDBDataCallBackInfo->szDataID,*pArNode);
			}
		}
	}
	return hRetCode;
}
