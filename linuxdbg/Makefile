include ../Makefile

${PROD}: $(addsuffix .o, ${OBJS})
	ar -rsv $@ $^

%.o: ../%.cc $(addprefix ../, $(addsuffix .hh, ${HHS}))
	clang++ -o $@ $< -I .. -std=c++2b -c -g

clean:
	touch ${PROD}
	rm ${PROD} *.o

.PHONY: clean
