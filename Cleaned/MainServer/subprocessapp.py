from __future__ import print_function # Only Python 2.x
import subprocess
import sys
def execute(command):
    sub_process = subprocess.run(command, shell=True, stdout=subprocess.PIPE)

    while True:
        myline = str(sub_process.stdout.readline())
        print(myline)

# Example
cmd = 'python3 cvapp.py'
for path in execute(cmd):
    print(path, end="")