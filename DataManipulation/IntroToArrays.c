/*
* File:            main.c
* Purpose : Brief description
* Authors : Rutendo Musuka


/* Preprocessor directives */
#include <stdlib.h> // So we can use the system command below
#include <stdio.h>

/* We try to avoid using numbers in code.  Programmers will often
   call numbers in your code 'magic numbers'.  We avoid using magic
   numbers  because it is easier to debug code that doesn't use them.
   We do this by defining constants under your include preprocessor
   directives. We define a constant like this:
   #define CONSTANT_NAME value
   This way you don't have to comb your way through your code
   looking for 'magic numbers.'  You only have to change the values
   of your constants, which you use in your code.  Note that 0, 1, and
   2 may be ok to use in your code and are not always considered magic
   numbers. */
#define A_NUMBER 17
#define MAX 100

   /* Function prototypes.  A function prototype tells the compiler
	  a) the name of the function you are writing
	  b) what type it returns (or void, if it returns nothing)
	  c) the type(s) of its arguments (or void, if there are none)*/
	void sieve(int list_of_numbers[], int length);

/*
 Main function drives the program.  Every C program must have one and
 only one main function.
 * PRE:       NULL (no pre-conditions)
 * POST:      NULL (no side-effects)
 * RETURN:    IF the program exits correctly
 *            THEN 0 ELSE 1
 */
int main(void)
{
	/* Variables */
	int i = A_NUMBER;
	int j;
	int numbers[MAX];

	/* Remember this? */
	printf("The address of the variable i (in decimal): %d\n", &i);
	printf("The address of the variable i (in hex): %x\n", &i);
	printf("The value stored in the variable i (in decimal): %d\n", i);
	printf("The value stored in the variable i (in hex): %x\n", i);

	/* Initializes all the cells in the array of ints called numbers to 0 */
	for (j = 0; j < MAX; ++j) {
		numbers[j] = 0;
	}
	printf("Cell number %d contains the value %d\n", j, numbers[(j-1)]);

	/* Invokes the sieve of Eratosthenes */
	sieve(numbers, MAX);

	/* Prints the results */
	printf("The primes numbers between 1 and %d are:\n", MAX);
	for (i = 2; i < MAX; ++i) {
		if (numbers[i] == 0) {
			printf("%d ", i);
		}
	}
	printf("\n"); // What is this for?

	/* And that's it */
	system("pause");
	return 0;
}

/*
 * Sieve of Eratosthenes
 * PARAM:  array_of_zeros is an int array
 *         populated with zeros
 * PARAM:  length of the array
 * PRE:    length is the correct length of
 *         array_of_zeros
 * POST:   array_of_zeros contains 1s in cells
 *         whose indices are primes, and 0s
 *         elsewhere
 * RETURN: void (nothing)
 */
void sieve(int array_of_zeros[], int length)
{
	/* Variables */
	int i = 0, j = 0;

	array_of_zeros[0] = 1; // 0 is not prime
	array_of_zeros[1] = 1; // 1 is not prime

	/* Finds primes.  0 and 1 aren't primes. */
	for (i = 2; i * i < length; ++i) {
		if (array_of_zeros[i] == 0) {
			for (j = i + i; j < length; j += i) {
				array_of_zeros[j] = 1;
			}
		}
	}

	/* End of function*/
	return;
}
