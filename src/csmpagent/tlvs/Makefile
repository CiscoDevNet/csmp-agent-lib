PROTOC=protoc-c
PROTOPATH += --proto_path=.

CsmpTlvs.pb-c.h: CsmpTlvs.proto
	$(PROTOC) --c_out=. CsmpTlvs.proto
clean:
	-rm -f *.pb-c.h *.pb-c.c
