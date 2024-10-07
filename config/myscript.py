#!/usr/bin/env python


def main():
	File=open('application.config','w')
	for i in range(0,64):
		File.write(str(i)+' CBR.so\n')

## execution of programs start from here
if __name__=='__main__':
	print "Starting...."
	
	main()
	print "Done"
