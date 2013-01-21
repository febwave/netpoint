import httplib  
conn = httplib.HTTPConnection("www.online.sh.cn")  
conn.request('get', '/')  
print conn.getresponse().read()  
conn.close()  