import sys

iname, oname = sys.argv[1:]

with open(iname, 'rb') as f:
	orig = data = f.read()
data = 'X' + data[1:]

with open(oname, 'wb') as f:
	f.write(data)
