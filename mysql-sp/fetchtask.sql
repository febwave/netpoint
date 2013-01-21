-- --------------------------------------------------------------------------------
-- Routine DDL
-- Note: comments before and after the routine body will not be stored by the server
-- --------------------------------------------------------------------------------
DELIMITER $$

CREATE DEFINER=`dbuser`@`%` PROCEDURE `fetchtask`()
begin

declare curTime datetime;

set curTime = now();

select tid,tdata
from task
where tbegintime < curTime and texpiredtime > curTime
order by tnextruntime
limit 0,1;

end
