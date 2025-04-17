all: rps_v2

rps_v2: main.c
	$(CC) -D_POSIX_C_SOURCE=200809L main.c -o rps_v2 -pthread

clean:
	rm -f rps_v2
