CC=gcc

#CFLAGS=-g3 -O0 -DVERBOSE 
 CFLAGS=-g3 -O0  

check: all
	rm -f myckpt.dat		#remove older checkpoint image files
	./checkpoint ./counting-test 828 &		#running counting test in bg
	sleep 4
	echo CHECKPOINTING ...
	kill -12 `pgrep -n counting-test`		#send a SIGUSR2 to pid of counting-test
	sleep 2
#	echo READING...
#	./read_ckpt_image
	echo RESTARTING...
	./restart




all: counting-test checkpoint  read_ckpt_image restart

counting-test: counting-test.c 
	${CC} ${CFLAGS} -rdynamic -o $@ $<

checkpoint: checkpoint.c libckpt.so
	${CC} ${CFLAGS} -o $@ $<

libckpt.so: ckpt.o
	${CC} ${CFLAGS} -shared -fPIC -o $@ $<

ckpt.o: ckpt.c
	${CC} ${CFLAGS}  -fPIC -c $<

read_ckpt_image: read_ckpt_image.c
	${CC} ${CFLAGS} -o $@ $<

restart: restart.c
	${CC} -static -Wl,-Ttext-segment=5000000  -Wl,-Tdata=5100000 -Wl,-Tbss=5200000  ${CFLAGS} -o $@ $<

clean:
	rm -f counting-test read_ckpt_image checkpoint restart
	rm -f *.o *.so *.dat

dist: 
	dir=`basename $$PWD` && cd .. && tar czvf $$dir.tar.gz ./$$dir
	dir=`basename $$PWD` && ls -l ../$$dir.tar.gz

.PRECIOUS: libckpt.so ckpt.so 	
