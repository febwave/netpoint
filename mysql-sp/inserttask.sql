

alter definer='dbuser' procedure inserttask(IN tskname varchar(45)
								,IN tskbegtintime datetime
								,IN tskexpiredtime datetime
								,IN tskinterval int
								,IN tskData varchar(1024))
begin

insert into task
(
	tname
	,tbegintime
	,texpiredtime
	,tinterval
	,tdata
)
value
(
	tskname
	,tskbegintime
	,tskexpiredtime
	,tskinterval
	,tskData
);

select last_insert_id() as tskid;


end 

