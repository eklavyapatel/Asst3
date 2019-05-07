all: foo

foo:
	gcc wtfclient.c -o WTF -lssl -lcrypto
	gcc wtfserver.c -o WTFserver -pthread
	gcc WTFtest.c -o WTFtest
clean:
	rm -rf WTFtest WTFserver WTF
