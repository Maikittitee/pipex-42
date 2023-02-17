
SRCS = pipex.c
EXEC = pipex
LIBS_FLAG = -lft -lftprintf  
FLAG = -Wall -Werror -Wextra

all : run

compilef:
	gcc ${LIBS_FLAG} -L./libs -g ${LIBS_FLAG} ${SRCS} -o pipex

compile:
	gcc -L./libs -g ${SRCS} ${LIBS_FLAG} -o pipex

run: compilef
	./${EXEC} "file1.txt" "cat" "grep line" "file2.txt" 

clean :
	rm pipex
fclean :

re : clean
	all


