
SRCS = pipex.c pipex_utils.c pipex_free_utils.c
EXEC = pipex
LIBS_FLAG = -lft -lftprintf  
FLAG = -Wall -Werror -Wextra

all : run

compilef:
	gcc ${FLAG} -L./libs -g  ${LIBS_FLAG}${SRCS} -o pipex

compile:
	gcc -L./libs -g ${SRCS} ${LIBS_FLAG} -o pipex

run: compilef
	./${EXEC} "file1.txt" "cat" "wc -l" "file2.txt" 

test: compile
	./pipex "file1.txt" "ls -l" "wc -l" "file2.txt"

clean :
	rm pipex
fclean :

re : clean
	all


