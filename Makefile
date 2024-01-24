#
# Top level makefile
#
# Usage:
# make <target>
#
# Valid targets:
# linux			- POSIX Linux
# freertos 	- FreeRTOS
#

linux:
	make -f linux.target

freertos:
	make -f freertos.target

clean:
	-rm -rf *.o $(LIB_OBJECT)
