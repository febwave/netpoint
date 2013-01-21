netpoint
========

target:self study in linux plateform.

design:
user can edit python script and setup schedule.
upload to service.
when condition matches,service will call dlopen , execute it and write result into mysql.
user can query results at any time.

tool:

compiler:g++

editor:emacs23

ui:glade2

database: work bench.



implement:

server mode:          task per thread

server storage:       mysql

transport data:       tcp/ip

datas encoded:         json

client storage:       sqlite

client user interface:glade+gtk

config format:        xml2
