#include <stdio.h>
#include <time.h>
#include <string.h>

int main(void) {

	time_t t;
	struct tm * td;
	char buffer[80];

	time(&t);

	td = localtime(&t);

	printf("%s\n", ctime(&t));

	printf("%d\n", td->tm_hour);


	return 0;
}
