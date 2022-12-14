CC = gcc
#CFLAGS = -Wall -Werror -Wextra
CFLAGS =
GCOVFLAGS = -lgcov --coverage

SOURCES = $(wildcard s21_*.c)
TESTS_SOURSES = $(wildcard ./tests/*.check)
TESTS = $(TESTS_SOURSES:.check=.test)
OBJECTS = $(SOURCES:.c=.o)

all: s21_decimal.a

rebuild: clean all

s21_decimal.a: $(OBJECTS) s21_decimal.h
	@ar rc $@ $(OBJECTS) 
	@ranlib $@

lib_cov.a: $(SOURCES) s21_decimal.h
	@$(CC) $(CFLAGS) -c *.c -g $(GCOVFLAGS)	
	@ar rc $@ *.o
	ranlib $@

%.o: %.c
	@$(CC) $(CFLAGS) -c $^ -o $@ -g

%.test: %.check s21_decimal.a
	@checkmk $< > $@.c
	@$(CC) $@.c -L. -lcheck -lm s21_decimal.a $(GCOVFLAGS) -o $@
	@rm -f $@.c
	@./$@ 

gcov_report: clean $(TESTS_SOURSES) lib_cov.a
	@checkmk $(TESTS_SOURSES) > .test.c
	@grep -v -e"#line " .test.c > test.c
	@$(CC) test.c -c
	@$(CC) test.o -g -L. -lcheck lib_cov.a -o test $(GCOVFLAGS)
	@./test &> /dev/null
	@lcov -t "t" -o test.info -c -d .  &> /dev/null
	@genhtml -o report test.info  &> /dev/null
	@open ./report/index.html
	@rm -f test.c

test: clean $(TESTS_SOURSES) s21_decimal.a
	csc /out:tests/gen.exe tests/generator.cs
	mono tests/gen.exe
	checkmk $(TESTS_SOURSES) > .test.c
	$(CC) .test.c -lcheck -lm s21_decimal.a -o test 
	./test

decimalizator: clean $(TESTS_SOURSES) s21_decimal.a
	csc /out:tests/gen.exe tests/generator.cs
	mono tests/gen.exe
	checkmk $(TESTS_SOURSES) > .test.c
	$(CC) .test.c -lcheck -lm s21_decimal.a -o test -D"DECIMALIZATOR"
	-./test &> /dev/null
	csc /out:tests/dec.exe tests/decimalizator.cs
	mono tests/dec.exe

clean:
	@rm -rf *.a *.o *.test *test.c .test.c *.gcda *.gcno *.info test.c report test *.input *.output

main: s21_decimal.a
	gcc main.c s21_decimal.a -g
# ./a.out

# brsw
