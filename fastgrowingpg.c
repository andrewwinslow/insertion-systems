
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
	printf("(1, 1)\n\n");

	for (int i = 1; i <= k; ++i) {
		printf("# Step %d\n", i);
		printf("(%d, %d) -> (%d, %d) (%d, %d)\n", i, i, i, i+1, i+1, i);
		printf("(%d, %d) -> (%d, 0) (%d, %d)\n", i, i+1, i, i+1, i+1);
		printf("(%d, %d) -> (%d, %d) (0, %d)\n", i+1, i, i+1, i+1, i);
		printf("(%d, 0) -> %c\n", i, 'a');
		printf("(0, %d) -> %c\n", i, 'a');
		printf("\n");
	}
	printf("# Finishing rule\n");
	printf("(%d, %d) -> %c\n", k+1, k+1, 'a'); 

	return EXIT_SUCCESS;
}

