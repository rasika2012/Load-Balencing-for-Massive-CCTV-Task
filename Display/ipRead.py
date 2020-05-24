fileName = 'ipAddresses.txt'

lineList = list()
with open(fileName) as f:
  for line in f:
    lineList.append(line)

print(lineList[0])