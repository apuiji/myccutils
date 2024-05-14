include Makefile

DEST = windbg

${DEST}/libmyutils.dll.a: $(addprefix ${DEST}/, ${OBJS})
	ar -rsv $@ $^

${DEST}/%.o: %.cc ${HHS}
	clang++ $< -c -g -O2 -o $@ -std=c++2b -stdlib=libc++

clean:
	echo>${DEST}\libmyutils.dll.a
	echo>${DEST}\a.o
	del ${DEST}\libmyutils.dll.a
	del ${DEST}\*.o

.PHONY: clean
