#include "pipex.h"

int	main()
{
	int id;
	int id2;
	int status;
	int childpid;

	id = fork();

	if (id == 0)
	{
		//childpid = getpid();
		ft_printf("Hello From 1 Child process\n");
		exit(127);
	}
	id2 = fork();
	if (id2 == 0)
	{
		ft_printf("Hello From 2 Child process\n");
		exit(2);
	}
	waitpid(id,NULL,0);
	waitpid(id2,&status,0);
	ft_printf("Hello From Parent Process\nstatus of 2 child is %d \n",status);

}