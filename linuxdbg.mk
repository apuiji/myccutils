include Makefile

DEST = linuxdbg

${DEST}/libmyutils.a: $(addprefix ${DEST}/, ${OBJS})
	ar -rsv $@ $^

${DEST}/%.o: %.c
	gcc $< -c -g -I . -O2 -o $@

clean:
	touch ${DEST}/libmyutils.a ${DEST}/a.o
	rm ${DEST}/libmyutils.a ${DEST}/*.o

.PHONY: clean
