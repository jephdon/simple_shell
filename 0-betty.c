#include <unistd.h>
#include "main.h"

/**
 * print_t - Prints a message to the standard output using write.
 * @str: string to print
 *
 * Return: Always 0 (Success)
 */
void print_t(char *str)
{
	ssize_t len = 0;

	while (str[len] != '\0')
		len++;
	write(STDOUT_FILENO, str, len);
}
