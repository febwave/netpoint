-- --------------------------------------------------------------------------------
-- Routine DDL
-- Note: comments before and after the routine body will not be stored by the server
-- --------------------------------------------------------------------------------
DELIMITER $$

CREATE DEFINER=`dbuser`@`%` PROCEDURE `querytaskresult`(IN tskid int,IN trid int)
begin

select tryid,result,response
from tskresp
where tid = tskid and tryid > trid
order by tryid;


end
