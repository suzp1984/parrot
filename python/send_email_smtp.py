import smtplib
import sys

WEBMASTER = "suzp1984@gmail.com"
SMTP_SERVER = "smtp.163.com"

filename = sys.argv[1]
try:
	message = open(filename)
	comment = message.read()
	message.close()
except:
	print "this a problem reading file"
	comment = "reading file bugs"
	
#comment = "send this mail using python stmplib modules"
msg = """\
From: suzh1984@163.com                                                                                                                                          
Subject: feedback
To: %s

I have the following comment:

%s

Thank You,


""" % (WEBMASTER, comment)

		
try:
	conn = smtplib.SMTP(SMTP_SERVER)
	conn.login("usename", "password")
	conn.sendmail('suzh1984@163.com', [WEBMASTER], msg)
	conn.quit()
except:
	print "There was a problem sending the email!"
	sys.exit

