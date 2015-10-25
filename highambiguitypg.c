
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Provide a parameter k>0 for the size of the pair grammar.\n");
		return EXIT_FAILURE;
	}

	int k = atoi(argv[1]);

	printf("# Start symbol\n");
	printf("(1, 6)\n\n");

	for (int i = 0; i < k; ++i) {
		printf("# Step %d\n", i);

		if (i != 0)
		{
			printf("(%d, %d) -> (%d, %d) (%d, %d)\n", 6*i-1, 6*i, 6*i-1, 6*i+6, 6*i+1, 6*i);
			printf("(%d, %d) -> %c\n", 6*i-1, 6*i+6, 'a');
			printf("(%d, %d) -> (%d, %d) (%d, %d)\n", 6*i+1, 6*i, 6*i+1, 6*i+6, 6*i, 6*i);
			printf("(%d, %d) -> %c\n", 6*i, 6*i, 'a');
		}

		printf("(%d, %d) -> (%d, %d) (%d, %d)\n", 6*i+1, 6*i+6, 6*i+1, 6*i+2, 6*i+3, 6*i+6);
		printf("(%d, %d) -> (%d, %d) (%d, %d)\n", 6*i+1, 6*i+6, 6*i+1, 6*i+4, 6*i+5, 6*i+6);

		printf("(%d, %d) -> (%d, %d) (%d, %d)\n", 6*i+1, 6*i+4, 6*i+1, 6*i+2, 6*i+3, 6*i+4);
		printf("(%d, %d) -> (%d, %d) (%d, %d)\n", 6*i+3, 6*i+6, 6*i+3, 6*i+4, 6*i+5, 6*i+6);

		printf("(%d, %d) -> %c\n", 6*i+1, 6*i+2, 'a');
		printf("(%d, %d) -> %c\n", 6*i+3, 6*i+4, 'a');

		if (i == k-1)
			printf("(%d, %d) -> %c\n", 6*i+5, 6*i+6, 'a'); 


		printf("\n");
	}

	return EXIT_SUCCESS;
}

