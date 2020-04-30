import subprocess
import smtplib
import socket
import datetime
import sys
from email.mime.text import MIMEText

# Accountinformationen zum Senden der E-Mail
Empfaenger = ""
Absender = ""
Passwort = ""
smtpserver = smtplib.SMTP("smtp.1und1.de", 587)
smtpserver.ehlo()
smtpserver.starttls()
smtpserver.ehlo

# In Account einloggen
smtpserver.login(Absender, Passwort)

# Aktuelles Datum holen
Datum = datetime.date.today()

# Text
Wert = ""

# Alle Argumente einlesen und in einem String speichern
for Argument in range(1, len(sys.argv)):
	Wert += str(sys.argv[Argument])
	Wert += " "

msg = MIMEText(Wert)

# Betreff + Datum
msg["Subject"] = "Nachricht vom Raspberry Pi - %s" % Datum.strftime("%b %d %Y")

# Absender
msg["From"] = Absender

#Empfaenger
msg["To"] = Empfaenger

# E-Mail abschicken
smtpserver.sendmail(Absender, [Empfaenger], msg.as_string())
smtpserver.quit()
