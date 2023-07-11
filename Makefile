FILE=lower_half

${FILE}: ${FILE}.c
	gcc -o $@ -Wl,-section-start,.custom_section=0x5000000 ./$<

clean: 
	rm -f ${FILE}

