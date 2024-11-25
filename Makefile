CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -I./modules/system_env/

files = "vector.c"
 
verbose_mode = $(or $(verbose), $(v))

############ compile ############
# -p flag is used to ignore the error if the directory already exists
compile_files = $(files)
executable_dir = bin/
executable_file = main
compile: Makefile
	@mkdir -p $(executable_dir)
	@if [ $(verbose_mode) ]; then \
		echo "Created directory: $(executable_dir)"; \
	fi
	@if [ $(verbose_mode) ]; then \
		echo "Compiling files:"; \
		for file in $(compile_files); do \
			printf "	%s\n" "$$file"; \
		done; \
	fi
	@$(CC) $(compile_files) $(LDFLAGS) -o $(executable_dir)/$(executable_file) $(CFLAGS)


############ run ############
run:
	@./$(executable_dir)/$(executable_file)


############ valgrind ############
valgrind:
	@valgrind ./$(executable_dir)/$(executable_file)


############ generating preprocessed files ############
preprocess_files = $(files)
preprocess_dir = preprocess/
preprocess: Makefile
	@mkdir -p $(preprocess_dir)
	@if [ $(verbose_mode) ]; then \
		echo "Created directory: $(preprocess_dir)"; \
	fi
	@for file in $(preprocess_files); do \
		if [ $(verbose_mode) ]; then \
			echo "Processing file: $$file"; \
		fi; \
		dirname=$(preprocess_dir)/$$(dirname "$$file"); \
		mkdir -p "$$dirname"; \
		$(CC) $(LDFLAGS) -E "$$file" -o "$(preprocess_dir)/$$(dirname "$$file")/$$(basename "$$file" .c).i" $(CFLAGS); \
	done


############ generating assembly files ############
assembly_files = $(files)
assembly_dir = assembly/
assembly: Makefile
	@mkdir -p $(assembly_dir)
	@if [ $(verbose_mode) ]; then \
		echo "Created directory: $(assembly_dir)"; \
	fi
	@for file in $(assembly_files); do \
		if [ $(verbose_mode) ]; then \
			echo "Processing file: $$file"; \
		fi; \
		dirname=$(assembly_dir)/$$(dirname "$$file"); \
		mkdir -p "$$dirname"; \
		$(CC) $(LDFLAGS) -S "$$file" -o "$$dirname/$$(basename "$$file" .c).s" $(CFLAGS); \
	done


############ generating object files ############
object_files = $(files)
object_dir = object/
object: Makefile
	@mkdir -p $(object_dir)
	@if [ $(verbose_mode) ]; then \
		echo "Created directory: $(object_dir)"; \
	fi
	@for file in $(object_files); do \
		if [ $(verbose_mode) ]; then \
			echo "Processing file: $$file"; \
		fi; \
		dirname=$(object_dir)/$$(dirname "$$file"); \
		mkdir -p "$$dirname"; \
		$(CC) $(LDFLAGS) -c "$$file" -o "$$dirname/$$(basename "$$file" .c).o" $(CFLAGS); \
	done


############ linking object files together ############
link_object_files = $(shell for file in $(files); do echo \"object/$${file%.c}.o\"; done)

link: object Makefile
	@mkdir -p $(executable_dir)
	@if [ $(verbose_mode) ]; then \
		echo "Created directory: $(executable_dir)"; \
	fi
	@if [ $(verbose_mode) ]; then \
		for file in $(link_object_files); do \
			printf "Linking file: %s\n" "$$file"; \
		done; \
	fi
	@$(CC) $(LDFLAGS) $(link_object_files) -o $(executable_dir)/$(executable_file) $(CFLAGS)


############ all ############
all: preprocess assembly object compile


############ compiling and running ############
compile_run: compile run


############ libraries ############
lib_path = lib/
# learn about static and dynamic libraries: https://domiyanyue.medium.com/c-development-tutorial-4-static-and-dynamic-libraries-7b537656163e

# $(CC) files.c... /path/to/static/library.a -o exe
# $(CC) files.c... /path/to/dynamic/library.so -o exe

# 	if your static library exists alongside a shared library the linker will prefer the shared library over the static library
# 	but you can force the linker to use the static library by using the --static flag
# $(CC) --static files.c... /path/to/library/file.a -o exe

# $(CC) files.c... -L/path/to/library1 -lmylibrary1 -L/path/to/library2 -lmylibrary2 -o exe
# 	-L flag is used to specify the path to the library
# 	-l flag is used to specify the name of the library (without the lib prefix and the .a or .so suffix)
# $(CC) files.c... -L/path/to/library1 -lmylibrary1 -Wl,-rpath=/path/to/library1 -o exe
# 	-Wl flag is used to pass options to the linker
# 	-rpath flag is falg for the linker, used to specify the runtime path of the library
# 	OR you can use the environment variable LD_LIBRARY_PATH to specify the runtime path of the library
# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./dynamic/library/path
# 	LD_LIBRARY_PATH is an environment variable that tells the dynamic linker where to look for shared libraries
# 	./dynamic/library/path is the path to the directory containing the shared library


#   -I flag is used to specify the path to the header files if you ever wanted to use <> instead of ""


############ static library ############
static_lib_name = libvector.a
static: object
	@mkdir -p $(lib_path)
	@if [ $(verbose_mode) ]; then \
		echo "Created directory: $(lib_path)"; \
	fi
	@ar rcs $(lib_path)/$(static_lib_name) $(link_object_files)
	@if [ $(verbose_mode) ]; then \
		echo "Created static library: $(lib_path)/$(static_lib_name)"; \
	fi


############ dynamic library (shared) ############
shared_lib_name = libvector.so
shared: object
	@mkdir -p $(lib_path)
	@if [ $(verbose_mode) ]; then \
		echo "Created directory: $(lib_path)"; \
	fi
	@$(CC) -shared -o $(lib_path)/$(shared_lib_name) $(link_object_files)
	@if [ $(verbose_mode) ]; then \
		echo "Created shared library: $(lib_path)/$(shared_lib_name)"; \
	fi
dynamic: shared


############ linking with both static and dynamic libraries ############
export: static shared


############ cleaning ############
# -f flag is used to ignore the error if the directory does not exist
clean:
	@rm -rf $(assembly_dir)
	@rm -rf $(preprocess_dir)
	@rm -rf $(object_dir)
	@rm -rf $(executable_dir)
	@rm -rf $(lib_path)
	@if [ $(verbose_mode) ]; then \
		echo "Removed directories: "; \
		printf "	%s\n" "$(assembly_dir)" "$(preprocess_dir)" "$(object_dir)" "$(executable_dir)"; \
	fi
