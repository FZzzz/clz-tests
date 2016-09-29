CC = gcc
CFLAGS = -O0 -Wall -std=gnu99
EXECUTABLE = clz_test

default:
	$(CC) $(CFLAGS) clz-tests.c -lm -o clz_test
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@ 

run: default
	for i in `seq 0 10 123457`; do \
		./clz_test $$i; \
	done
clean:
	rm -f $(EXECUTABLE) *.o *.s time.csv

plot:
	gnuplot my_plot.gp

