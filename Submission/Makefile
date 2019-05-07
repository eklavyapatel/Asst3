all: WTF WTFserver

WTF: wtfclient.c
	gcc wtfclient.c -o WTF -lssl -lcrypto

WTFserver: wtfserver.c
	gcc wtfserver.c -o WTFserver -pthread

clean:
	rm -rf WTF WTFserver

test: WTFtest.c
	gcc WTFtest.c -o WTFtest 