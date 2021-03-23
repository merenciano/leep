#!/usr/bin/python3

import sys

fin = open(str(sys.argv[1]), "r")
fout = open(str(sys.argv[2]), "w")

max_value = 0.01

#First pass in order to get the greater value
for l in fin:
    line = l.split()
    if not line:
        continue
    if line[0] == "v":
        if abs(float(line[1])) > max_value:
            max_value = abs(float(line[1]))
        if abs(float(line[2])) > max_value:
            max_value = abs(float(line[2]))
        if abs(float(line[3].rstrip())) > max_value:
            max_value = abs(float(line[3]))

print('Max value is ' + str(max_value))
maxinv = 1 / max_value

fin.seek(0)

for l in fin:
    line = l.split()
    if not line:
        continue
    if l[:2] == "v ":
        fout.write('v ' + str(float(line[1]) * maxinv) + ' ' + str(float(line[2]) * maxinv) + ' ' + str(float(line[3]) * maxinv) + '\n')
    else:
        fout.write(l)

fin.close()
fout.close()

