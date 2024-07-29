#
# Top level makefile
#
# Usage:
# make <target>
#
# Valid targets:
# linux    - POSIX Linux
# freertos - FreeRTOS
# efr32_wisun - EFR32 with Wi-SUN stack and FreeRTOS
# clean    - clean binaries
#
.PHONY : linux freertos efr32_wisun clean

linux:
	make -f linux.target

freertos:
	make -f freertos.target

efr32_wisun:
	make -f efr32_wisun.target

clean:
	-rm -rf build/
	-rm -f *.o
	-rm -f *.a
