#!/usr/bin/python

import subprocess

def coroutine(func):
    def start(*args,**kwargs):
        cr = func(*args,**kwargs)
        cr.next()
        return cr
    return start

    
def get_ip_address(target):
    p = subprocess.Popen(['hostname', '-I'], stdout=subprocess.PIPE,
                         stderr=subprocess.PIPE)
    out, err = p.communicate()
    try:
        target.send(out)
    except StopIteration:
        print "end"


@coroutine
def send_email():
    import smtplib
    import email
    from email.mime.text import MIMEText
    import sys
    try:
        ip = (yield)

        msg = MIMEText("""
        hi,
        
        Raspberry pi's address is:

        %s

        Thank You,


        """ % (ip))

        msg['To'] = email.utils.formataddr(('username', 'target@domain.com'))
        msg['From'] = email.utils.formataddr(('username', 'source@domain.com'))
        msg['Subject'] = "raspberry pi's address"

        #print msg
        conn = smtplib.SMTP("smtp.server.com")
        #conn.set_debuglevel(True)
        conn.login("userName", "passwd")
        conn.sendmail('source@domain.name', ['target@domain.com'], msg.as_string())
        conn.quit()
    except StopIteration:
        pass

    
# exec
get_ip_address(send_email())
