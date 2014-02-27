#!/bin/bash
#made by vbird
# please input the right net device name
EXTIF="ppp0"              # external net interface, used to connected to outside.
INIF="eth0"             # internel LAN net interface
INNET="192.168.1.0/24" # internal LAN net cfg
export EXTIF INIF INNET
#######
## Step 1: Basic net settings and clean iptable rules
# 1. setup net interface 
echo "1" > /proc/sys/net/ipv4/tcp_syncookies
echo "1" > /proc/sys/net/ipv4/icmp_echo_ignore_broadcasts
for i in /proc/sys/net/ipv4/conf/*/rp_filter; do
      echo "1" > $i
done
for i in /proc/sys/net/ipv4/conf/*/log_martians; do
      echo "1" > $i
done
for i in /proc/sys/net/ipv4/conf/*/accept_source_route; do
      echo "0" > $i
done
for i in /proc/sys/net/ipv4/conf/*/accept_redirects; do
      echo "0" > $i
done
for i in /proc/sys/net/ipv4/conf/*/send_redirects; do
      echo "0" > $i
done
## 2. cleanup iptable rules
PATH=/sbin:/usr/sbin:/bin:/usr/bin; export PATH
iptables -F
iptables -X
iptables -Z
iptables -P INPUT ACCEPT
iptables -P OUTPUT ACCEPT
iptables -P FORWARD ACCEPT
iptables -A INPUT -i lo -j ACCEPT
iptables -A INPUT -m state --state RELATED -j ACCEPT
## Setup 2: setup iptable rules
# 1. insmod the necessary kernel modules
modules="ip_tables iptable_nat ip_nat_ftp ip_nat_irc ip_conntrack
ip_conntrack_ftp ip_conntrack_irc"
for mod in $modules
do
      testmod=`lsmod | grep "${mod} "`
      if [ "$testmod" == "" ]; then
            modprobe $mod
      fi
done
## 2. clean up NAT table
iptables -F -t nat
iptables -X -t nat
iptables -Z -t nat
iptables -t nat -P PREROUTING ACCEPT
iptables -t nat -P POSTROUTING ACCEPT
iptables -t nat -P OUTPUT ACCEPT
# 3. setup as gateway host 
if [ "$INIF" != "" ]; then
   iptables -A INPUT -i $INIF -j ACCEPT
   echo "1" > /proc/sys/net/ipv4/ip_forward
   if [ "$INNET" != "" ]; then
     for innet in $INNET
     do
      iptables -t nat -A POSTROUTING -s $innet -o $EXTIF -j MASQUERADE
     done
   fi
fi
#### If your MSN cannot connected, or your access denied by some website
#### that maybe the problem of MTU
# iptables -A FORWARD -p tcp -m tcp --tcp-flags SYN,RST SYN -m tcpmss \
#         --mss 1400:1536 -j TCPMSS --clamp-mss-to-pmtu
# 4. internel service setting
# iptables -t nat -A PREROUTING -p tcp -i $EXTIF --dport 80 \
#        -j DNAT --to 192.168.1.210:80
