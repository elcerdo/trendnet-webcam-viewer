#! /bin/sh

for i in `cat trendnet_security_cams.txt`;
do
  wget --spider --timeout=5 --tries=1 `echo -n $i | sed -e s+.$++` 2> /dev/null && echo $i >> trendnet_security_cams_up.txt
done
