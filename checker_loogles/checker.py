#!/usr/bin/python
 
import os
import requests
import hashlib
from sys import argv
 
 
PORT="8087"
 
 
def sha1(string):
    s = hashlib.sha1()
    s.update(string.encode("utf-8"))
    return(s.hexdigest())
 
 
def check(hostname):
    try:
        response = os.system("ping -c 1 " + hostname + " > /dev/null 2>&1")
        if response != 0:
            print("Host unreachable")
            return(104)
        r = requests.get('http://'+hostname+':'+PORT)
        if "<html>" not in r.text:
            return(104)
        return(101)
    except:
        return(104)
 
 
def put(hostname, id, flag):
    try:
        if check(hostname)==101:
            url = 'http://' + hostname + ':' + PORT
            query = sha1('loogles' + id) + " " + flag
            r = requests.get(url+'/api/v1/search?query=' + query)
            if "meta" in r.text:
                return(101)
        else:
            print("Error 'check()'")
            return(104)
    except:
        print("Error 'try:'")
        return(104)
 
def get(hostname, id, flag):
    try:
        url = 'http://' + hostname + ':' + PORT
        query = sha1('loogles' + id) + " " + flag
        r = requests.get(url+'/api/v1/search?query=' + query)
        if (query in r.text):
            return(101)
        print("Flag is not found")
        return(104)
    except:
        print("Error 'try'")
        return(104)
 
       
if __name__ == '__main__':
    if len(argv) > 1:
        if argv[1] == "check":
            if len(argv) > 2:
                exit(check(argv[2]))
        elif argv[1] == "put":
            if len(argv) > 4:
                exit(put(argv[2], argv[3], argv[4]))
        elif argv[1] == "get":
            if len(argv) > 4:
                exit(get(argv[2], argv[3], argv[4]))
    exit(110)
