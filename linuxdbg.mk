include Makefile

linuxdbg/libmyutils.a: $(addprefix linuxdbg/, ${OBJS})
	ar -rsv $@ $^

linuxdbg/%.o: %.cc ${HHS}
	clang++ $< -c -g -O2 -o $@ -std=c++2b -stdlib=libc++

clean:
	touch linuxdbg/libmyutils.a linuxdbg/a.o
	rm linuxdbg/libmyutils.a linuxdbg/*.o

.PHONY: clean
