FILE=lower-half
CC=gcc
CFLAGS=-g3 -fPIC -O0

all: ${FILE} libmpiStub.so upper-half

check: ${FILE} upper-half
	./${FILE} -a 0x800000 ./upper-half

check_ls:  ${FILE}
	./${FILE} -a 0x800000 /bin/ls
gdb: ${FILE}
	gdb --args ./$< -a 0x800000 ./upper-half 

# Compile code with kernel-loader to be in high memory, to avoid address conflicts.
${FILE}: ${FILE}.c get-symbol-offset.o  copy-stack.o patch-trampoline.o
	${CC} ${CFLAGS} -Wl,-Ttext-segment=0x2000000,-section-start,.custom_section=0x5000000 -o $@ $^ -ldl

get-symbol-offset: get-symbol-offset.c
	${CC} ${CFLAGS} -o $@ $<
copy-stack: copy-stack.c
	${CC} ${CFLAGS} -o $@ $<

patch-trampoline: patch-trampoline.c
	${CC} ${CFLAGS} -o $@ $<

upper-half: upper-half.c 
	${CC} ${CFLAGS} -o $@ $< -L. -lmpiStub

libmpiStub.so: mpiStub.o
	${CC} ${CFLAGS} -shared -o $@ $<

mpiStub.o: mpiStub.c
	${CC} ${CFLAGS} -c $<

vi vim:
	vim ${FILE}.c

status: clean
	git status

dist: clean
	dir=`basename $$PWD` && cd .. && tar zcvf $$dir.tgz $$dir
	dir=`basename $$PWD` && ls -l ../$$dir.tgz

clean:
	rm -f ${FILE} upper-half get-symbol-offset  copy-stack *.o patch-trampoline a.out *.so

.PHONY: dist vi vim clean gdb gdb_test
