#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int find_s(const char *this, const char *str, size_t pos)
{
	char *result = NULL;

	result = strstr(this + pos, str);
	if (result == NULL)
		return (-1);

	return (strlen(this) - strlen(result));
}

int main(int ac, char **av)
{
	int a = find_s(av[1], av[2], atoi(av[3]));
	printf("%d\n", a);
}