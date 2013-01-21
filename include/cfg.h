


#define svr_addr_length		128
#define db_addr_length		32
#define db_acc_name_length	32
#define db_acc_pwd_length	32
#define db_inst_name_length	32

/* json key define */
#define JSK_REQ_SEQ		"rs"
#define JSK_REQ_ID		"ri"
#define JSK_JOB_NAME		"n"
#define JSK_JOB_BEGIN_TIME	"bt"
#define JSK_JOB_EXPIRED_TIME	"et"
#define JSK_JOB_INTERVAL	"iv"
#define JSK_JOB_CONTEXT		"c"
#define JSK_RESPONSE_CODE	"rc"
#define JSK_JOB_ID		"i"
#define JSK_TRY_ID		"ti"
#define JSK_ARRAY		"ar"
#define JSK_JOB_EXECUTE_CODE	"ec"
#define JSK_JOB_RESULT		"r"
#define JSK_JOB_RESPONSE_DATA	"rd"
#define JKEY_SVR_JSON_OBJ	"_sjo"

/* define request id */
#define REQ_CREATE	1
#define REQ_QUERY	2



typedef struct tagNetCfg
{
     char	szServeraddress[svr_addr_length];
     int	nport;     
}NetCfg,*lpNetCfg;

typedef struct
{
     char	szaddr[db_addr_length];
     char	szaccname[db_acc_name_length];
     char	szaccpwd[db_acc_pwd_length];
     char	szinstname[db_inst_name_length];
     int	nport;
}dbCfg;

typedef struct
{
     int	nApp;
     int	nEnableDebug;
     int	nExitApp;
     int	nWait;
     int	nkillsemid;     
}bootCfg;

typedef struct
{
     NetCfg	nc;
     dbCfg	dbc;     
}appsetting,*lpappsetting;

typedef struct
{
     dbCfg	dbc;
     int	sock_id;     
}workitemCfg,*lpworkitemCfg;

typedef struct
{
     int	nReqId;
     int	nSeq;
}jobbase;

#define JOB_NAME_LENGTH 48


typedef struct
{
     jobbase	base;
     int	nInterval;
     char	szName[JOB_NAME_LENGTH];
     struct tm	tbegintime;
     struct tm	texpiredtime;
     int	nContextSize;
     char	context[1];     
}jobCreateReq;

typedef struct
{
     jobbase	base;     
     int	nRsp;
     int	nId;     
}jobCrtResp;

typedef struct
{
     jobbase	base;
     int	nId;
     int	nTrId;     
}jobQueryReq;

typedef struct
{
     jobbase	base;
     int	nRsp;
     int	nResultSize;
     char	result[1];
}jobQueryResp;


int	initxmlcfg(void** lpCtxt,void** lpxmlDoc);
void	freexmlcfg(void* lpCtxt,void* lpxmlDoc);
int	getservercfg(void* lpxmlDoc,NetCfg* lpcfg);
int	getdatabasecfg(void* lpxmlDoc,dbCfg* lpCfg);
void	getoptions(int argc,char** argv,bootCfg* lpCfg);
int	getsempkey(void* lpxmlDoc,int* lpResult);
void*	svrthreadproc(void* obj);


extern "C" int	AppQueryInterface(void** lpAppIf );
extern "C" void	AppReleaseInterface(void* lpAppIf);



int	createresponse(int nReq,int nSeq,int nResp,int nJobId, char** lpResult,unsigned int* nResultSize);


int	getrequest(char* lpBuffer,unsigned int uSize,jobbase** lpjob);






