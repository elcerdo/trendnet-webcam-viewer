#!/usr/bin/env python2
# coding: utf-8

import re

input_filename = "ip-to-country.csv"
output_filename = "ip_to_country.cc"
#re_line = re.compile(r'''^"(\d+)","(\d+)","(\w+)","(\w+)","(\w+)"\n$''')
re_line = re.compile(r'''^"(\d+)","(\d+)","(\w+)","(\w+)","([^"]+)".*$''')

items = []
items.append("static const Record records[] = {")
recordsSize = 0
for line in open(input_filename,"r").readlines():
    match = re_line.match(line)
    groups = match.groups()
    items.append("{%d,%d,\"%s\"}," % (int(groups[0]),int(groups[1]),groups[3]))
    recordsSize += 1
items.append("};")
items.append("static unsigned long int recordsSize = %d;" % recordsSize)

open(output_filename,"w").write("\n".join(items))

