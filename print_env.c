#include "main.h"

/**
 * print_env - Prints all environment variables
 *
 * Return: nothing
 */
void print_env(void)
{
	char **env = environ;	/* Start of the environ array */

	while (*env != NULL)
	{
		write(STDOUT_FILENO, *env, strlen(*env));
		write(STDOUT_FILENO, "\n", 1);
		env++;
	}
}
