# This is a net speed determine program
# Using shell cmmand ifocnifg and awk

import subprocess
import time

last_RX_Bytes = 0;
cur_RX_Bytes = 0;

cmd1 = "ifconfig eth0 | grep \"RX\" | awk '{if(NR==1)'print'}' | awk '{print $2}' | awk '{gsub(\"packets:\", \"\");print}'"
cmd2 = "cat /proc/net/arp | grep : | grep ^192.168.100 | grep 0x2 | awk '{print NR}'"

def get_netSpeed():

  last_RX_Bytes = int(subprocess.Popen(cmd1, shell=True, stdout=subprocess.PIPE).stdout.read())
  time.sleep(3)
  cur_RX_Bytes = int(subprocess.Popen(cmd1, shell=True, stdout=subprocess.PIPE).stdout.read())
  netSpeed = (cur_RX_Bytes - last_RX_Bytes) / 3
  
  print netSpeed, "kb/s"
  
  return str(netSpeed)
  
def get_wifi_user_number():
  data = subprocess.Popen(cmd2, shell=True, stdout=subprocess.PIPE).stdout.read()
  
  user_number = len(data)/2
  print user_number
  
  return str(user_number)

def saveFile(path, data):
  fs = open(path, "wb")
  fs.write(data)

if __name__=="__main__":
  while True:
    saveFile("/tmp/netSpeed", get_netSpeed())
    saveFile("/tmp/wifi_user_number", get_wifi_user_number())
    
    
    