#
# Makefile for sample CSMP application that uses
# CSMP-Agent Library (csmp_agent_lib.a)
#
# Valid targets:
# linux    - POSIX Linux application
# freertos - FreeRTOS application
# efr32_wisun - EFR32 WISUN application
# clean    - Clean binaries
#

linux:
	make -f linux.target

freertos:
	make -f freertos.target

efr32_wisun:
	make -f efr32_wisun.target

clean:
	-rm -rf *.o CsmpAgentLib_sample
	-rm -f *.a
	-rm -f *.hex
	-rm -f *.s37
	-rm -f *.out
	-rm -rf ../build
