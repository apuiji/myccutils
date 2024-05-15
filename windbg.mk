include Makefile

DEST = windbg

${DEST}/libmyutils.dll.a: $(addprefix ${DEST}/, ${OBJS})
	ar -rsv $@ $^

${DEST}/%.o: %.c ${HEADS}
	gcc $< -c -g -O2 -o $@

clean:
	echo>${DEST}\libmyutils.dll.a
	echo>${DEST}\a.o
	del ${DEST}\libmyutils.dll.a
	del ${DEST}\*.o

.PHONY: clean
