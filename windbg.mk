include Makefile

windbg/libmyutils.dll.a: $(addprefix windbg/, ${OBJS})
	ar -rsv $@ $^

windbg/%.o: %.cc ${HHS}
	clang++ $< -c -g -O2 -o $@ -std=c++2b -stdlib=libc++

clean:
	echo>windbg\libmyutils.dll.a
	echo>windbg\a.o
	del windbg\libmyutils.dll.a
	del windbg\*.o

.PHONY: clean
