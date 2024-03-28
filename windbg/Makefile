include ../Makefile

${PROD}: ${OBJS}
	ar -rsv $@ $^

%.o: ../%.cc $(addprefix ../, ${HHS})
	clang++ -o $@ $< -I .. -std=c++2b -c -g

clean:
	echo > ${PROD}
	del ${PROD} *.o

.PHONY: clean
