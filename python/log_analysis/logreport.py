#!/usr/bin/python
# nginx/apache web server log parser by David Beazley's python generator
# http://www.dabeaz.com/generators-uk/
# python logreport.py | sort -n -k1
# following is the sample log
# 192.168.100.2 - - [02/Feb/2016:08:23:32 +0800] "GET /cashbox/ HTTP/1.1" 404 68 "-" "Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Win64; x64; Trident/5.0)" "112.124.127.53" 0.009 192.168.202.108:8010 0.009


import re
from datetime import datetime

logpats = r'(\S+) (\S+) (\S+) \[(.*?)\] "(\S+) (\S+) (\S+)" (\S+) (\S+) "(.*?)" "(.*?)" "(.*?)" (\S+) (\S+) (\S+)'
logpat = re.compile(logpats)

def field_map(dictseq, name, func):
    for d in dictseq:
        d[name] = func(d[name])
        yield d

def gen_lines_from_file(filename):
    lines = open(filename)
    return lines
    

def ngx_log_dict(lines):
    groups = (logpat.match(line) for line in lines)
    tuples = (g.groups() for g in groups if g)

    columes = ('host', 'referrer', 'user', 'datetime', 'method', 'request', 'proto', 'status', 'bytes', 'unknown1', 'unknown2', 'ip1', 'float1', 'ip2', 'float2')
    dict_logs = (dict(zip(columes, t)) for t in tuples)
    dict_logs = field_map(dict_logs, "bytes", lambda s: int(s) if s != '-' else 0)
    dict_logs = field_map(dict_logs, "float1", lambda s: float(s) if s != '-' else 0)
    dict_logs = field_map(dict_logs, "float2", lambda s: float(s) if s != '-' else 0)

    return dict_logs

    
def gen_filter_time_intervel(logs, after, before):
    for r in logs:
        t = r['datetime']
        t = re.sub("\s\+.*?$", "", t)
        t = datetime.strptime(t, "%d/%b/%Y:%H:%M:%S")
        if before > t and t > after:
            yield r
    
def sum_request_bytes(logs):
    sum_bytes = {}
    for r in logs:
        if r['request'] not in sum_bytes:
            sum_bytes[r['request']] = r['bytes']
        else:
            sum_bytes[r['request']] += r['bytes']
            
    return sum_bytes




def filter_sum_request_bytes(sums):
    ret_sum = {}
    for k in sums:
        short_k = re.sub("\?.*?$", "", k)
        if short_k in ret_sum:
            ret_sum[short_k] += sums[k]
        else:
            ret_sum[short_k] = sums[k]
    return ret_sum


if __name__ == "__main__":
    lines = gen_lines_from_file('cashbox_access.log')
    logs = ngx_log_dict(lines)
    logs = gen_filter_time_intervel(logs, datetime.strptime("02/Feb/2016:10:30:00", "%d/%b/%Y:%H:%M:%S"), datetime.strptime("02/Feb/2016:11:00:00", "%d/%b/%Y:%H:%M:%S"))
    sums = sum_request_bytes(logs)
            
    filtered_sum = filter_sum_request_bytes(sums)

    for k in filtered_sum:
        print(str(filtered_sum[k]) + " " + k)
