/*
 Author: Rutendo Musuka
 */

#include "lab2_in-lab_exercises.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define SMALL_ARRAY 25

 /*
  Main function drives the program.  Every C program must have one
  main function.  A project will not compile without one.
  PRE:    NULL (no pre-conditions)
  POST: NULL (no side-effects)
  RETURN: IF the program exits correctly
 THEN 0 ELSE 1
  */
void swap_ints(int* first_int, int* second_int);
void reverse_string(char* string);
int contains_sample(char* candidate, char* sample);
int find_index(char* candidate, char* sample);

int main(void)
{
	/* We start every function with a list of variables */
	int i = 0;
	int* pointer_to_some_ints;
	int* p1;
	int* p2;
	int num1;
	int num2;
	int a;

	num1 = 8;
	num2 = 10;
	p1 = &num1;
	p2 = &num2;

	printf("ummmmm \n");

	a = find_index("hello", "llo");

	printf ("position = %i",a);
	/* Then we do things, like invoke functions and assigns their return
	  values to variables.  You don't need to change these two lines */
	pointer_to_some_ints = (int*)malloc(SMALL_ARRAY * sizeof(int));
	for (i = 0; i < SMALL_ARRAY; ++i) {
		*(pointer_to_some_ints + i) = i;
	}

	swap_ints(p1, p2);
	char candidate[] = "soupspoon";
	char sample[] = "spoon";
	find_index(candidate, sample);

	//verse_string( char * string );

	// You can set a breakpoint here later in order to test the debugger 'watch' tab for pointers

	/* The system command forces the system to pause before closing executable window */
	system("pause");
	return 0;
}

/*
 * Let's start with something easy.  There are 3 unit tests
 * for this function.
 *
 * Swaps the contents of two integer variables using pointers
 *
 * PARAM:     first_int is a pointer to an int
 * PARAM:     second_int is a pointer to an int
 * PRE:       both pointers are valid pointers to int
 * POST:      the contents of two integer variables are swapped
 * RETURN:    VOID
 */
void swap_ints(int* first_int, int* second_int)
{
	int temp;

	temp = *first_int;
	*first_int = *second_int;
	*second_int = temp;

}

/*
 * Now let's try something a little more challenging.
 *
 * Reverses the contents of the string passed to the
 * function.  Does not move the terminating null '\0'
 * character.
 *
 * PARAM:     string is a pointer to an array of char (a string)
 * PRE:       the array of char terminates with a null '\0'
 * POST:      the char array has been reversed
 * RETURN:    VOID
 */
void reverse_string(char* string)
{
	int length;
	int i;
	int j;
	char p;

	length = 0;

	i = 0;
	while (*(string + i) != '\0') {
		length++;
		i++;
	}

	j = length - 1;

	for (i = 0; i < j; i++) {
		p = *(string + i);
		*(string + i) = *(string + j);
		*(string + j) = p;
		j--;
	}

}

/*
 * Let's see how well you read the lab document.
 *
 * Determines if candidate contains sample, and returns
 * 1 if  it does, and 0 if it does not.
 *
 * PARAM:     candidate is a pointer to an array of char (a string)
 * PARAM:     sample is a pointer to an array of char (a string)
 * PRE:       the arrays of char terminate with a null '\0'
 * PRE:       candidate != NULL; sample != NULL
 * POST:      N/A
 * RETURN:    IF candidate contains sample THEN 1
 *            ELSE 0.
 */
int contains_sample(char* candidate, char* sample)
{
	// Replace this return statement with your code

	//int length;
	//int i;
	//i=0;

	//length=strlen(candidate);


	if (strstr(candidate, sample) != NULL) {
		return 1;
	}

	return 0;
}

/*
 * Returns the first index where sample is located inside the
 * candidate.  For example:
 * IF candidate = "Hello", sample = "Hello", RETURNS 0
 * IF candidate = "soupspoon", sample = "spoon", RETURNS 4
 * IF candidate = "ACGTACGTA", sample = "CGT", RETURNS 1
 * IF candidate = "CGTACGTA", sample = "CGTT", returns -1
 *
 * PARAM:     candidate is a pointer to an array of char (a string)
 * PARAM:     sample is a pointer to an array of char (a string)
 * PRE:       the arrays of char terminate with a null '\0'
 * POST:      N/A
 * RETURN:    IF candidate contains sample
 *            THEN the index where the first letter of sample is inside candidate
 *            ELSE -1.
 */


int find_index(char* candidate, char* sample){
	char* result;
	if (strstr(candidate, sample) != NULL) {
		result = strstr(candidate, sample)-candidate;
		return result;
	}
	else
		return -1;
 
}



/*int find_index(char* candidate, char* sample)
{
	//variables
	int i;
	int j;
	int c_length;
	int s_length;
	int count;

	c_length = strlen(candidate); //length of candidate
	s_length = strlen(sample); //length of sample
	i = 0;

	if (strstr(candidate, sample) != NULL) { //only checks index position if sample is found in candidate 
		while (i <= c_length) { //running through candidate
			j = 0;
			
			while (j < s_length) { //chacking for sample within candidate 
				if (*(candidate + j + i) != *(sample + j)) {
					//*(candidate +j +i) == candidate[j + i]
					break; //if sample is not found with current index in the candidate >> move to check next position in candidate
				}

				j++; 
			}
			if (j == s_length) { //== vs = 
				return i;
			}
		i++;
		}
	}
	return -1;

}*/
