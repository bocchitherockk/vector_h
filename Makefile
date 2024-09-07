files = main.c vector.c
compiler = gcc
# compiler = clang
# compiler = tcc

compile: $(files) vector.h Makefile
	@echo "Compiling..."
	$(compiler) $(files) -o main
	@echo "Finished compiling."

run: compile
	./main

pre: $(files) vector.h Makefile
	@echo "Preprocessing..."
	$(compiler) -E main.c vector.c > main.i
	@echo "Finished preprocessing."

clean:
	@echo "Cleaning..."
	@cleaning=false; \
	if [ -f ./main ]; then rm ./main; echo "removed ./main"; cleaning=true; fi; \
	if [ -f ./test ]; then rm ./test; echo "removed ./test"; cleaning=true; fi; \
	if [ -f ./main.i ]; then rm ./main.i; echo "removed ./main.i"; cleaning=true; fi; \
	if [ "$$cleaning" = true ]; then echo "Finished cleaning."; else echo "Nothing to clean."; fi

test: test.c vector.c vector.h Makefile
	$(compiler) test.c vector.c -o test
