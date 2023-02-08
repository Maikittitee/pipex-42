
MAIN = main.c
EXEC = main
LIBS_FLAG = -lft -lftprintf  
FLAG = -Wall -Werror -Wextra

all : compile

compile:
	gcc ${FLAG} -L./libs ${MAIN} ${LIBS_FLAG} -o main

run: compile
	./main

clean :
	rm main
fclean :

re : clean
	all


