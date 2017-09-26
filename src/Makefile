Scaner:test.o device_stat.o function.o protocol_pack.o Queue.o radiotap.o VarArray.o
	$(CC) $(LDFLAGS) test.o device_stat.o VarArray.o function.o protocol_pack.o Queue.o radiotap.o -o Scaner -lpcap -lpthread
test.o:test.c
	$(CC) $(CFLAGS) -c test.c
device_stat.o:device_stat.c
	$(CC) $(CFLAGS) -c device_stat.c
function.o:function.c
	$(CC) $(CFLAGS) -c function.c
protocol_pack.o:protocol_pack.c
	$(CC) $(CFLAGS) -c protocol_pack.c
Queue.o:Queue.c
	$(CC) $(CFLAGS) -c Queue.c
radiotap.o:radiotap.c
	$(CC) $(CFLAGS) -c radiotap.c
VarArray.o:VarArray.c
	$(CC) $(CFLAGS) -c VarArray.c
clean:
	rm *.o Scaner
