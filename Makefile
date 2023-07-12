FILE=lower-half
CC=gcc
CFLAGS=-g3 -O0

all: ${FILE} upper-half

check: ${FILE} upper-half
	./${FILE} -a 0x800000 ./upper-half

check_ls:  ${FILE}
	./${FILE} -a 0x800000 /bin/ls
gdb: ${FILE}
	gdb --args ./$< -a 0x800000 /bin/ls

# Compile code with kernel-loader to be in high memory, to avoid address conflicts.
${FILE}: ${FILE}.c get-symbol-offset.o  copy-stack.o patch-trampoline.o
	gcc -g3 -static -DSTANDALONE -Wl,-Ttext-segment=0x1000000,-section-start,.custom_section=0x5000000 -o $@ $^

get-symbol-offset: get-symbol-offset.c
	gcc -g3 -static -DSTANDALONE -o $@ $<

copy-stack: copy-stack.c
	gcc -g3 -static -DSTANDALONE -o $@ $<

patch-trampoline: patch-trampoline.c
	gcc -g3 -static -DSTANDALONE -o $@ $<

upper-half: upper-half.c
	${CC} ${CFLAGS} -o $@ $<
vi vim:
	vim ${FILE}.c

dist: clean
	dir=`basename $$PWD` && cd .. && tar zcvf $$dir.tgz $$dir
	dir=`basename $$PWD` && ls -l ../$$dir.tgz

clean:
	rm -f ${FILE} upper-half get-symbol-offset  copy-stack *.o patch-trampoline a.out

.PHONY: dist vi vim clean gdb gdb_test
