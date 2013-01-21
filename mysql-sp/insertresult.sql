-- --------------------------------------------------------------------------------
-- Routine DDL
-- Note: comments before and after the routine body will not be stored by the server
-- --------------------------------------------------------------------------------
DELIMITER $$

CREATE DEFINER=`dbuser`@`%` PROCEDURE `insertresult`(IN tskid int,IN retcode int, resp varchar(1024))
begin

insert into tskresp
(
tid
,result
,response
)
values
(
tskid
,retcode
,resp
);


update task
set tnextruntime = now() +tinterval
where tid = tskid;

select last_insert_id() as tryid;

end