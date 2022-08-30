/*
File:         tide_analyzer.c
Purpose:      Consumes a semi-formatted tide measurement file and
determines if the corresponding tide is once- or
twice-daily using a fast discrete Fourier transformation.
The tide measurement file is a txt file whose name
corresponds to the name defined in the preprocessor
directive.  It is a series of NUMBER_OF_READINGS tidal
readings (in mm) taken hourly.
Author:       Rutendo Musuka and Hazel Chongoti
Student #s:   76822683 and 53106662
CS Accounts:  k7i2b and m8j2b
Date:         10 Sept 2019
*/

//stack overflow -#3


/******************************************************************
PLEASE EDIT THIS FILE

Sections of code in this file are missing.  You must fill them in.
Comments that start with // should be replaced with code
Comments that are surrounded by * are hints
******************************************************************/

/* Preprocessor directives */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tide_analyzer.h"
#include "discrete_fourier_transform.h"

#define NUMBER_OF_READINGS  131072 /* This = 2^17 (hint!) */
#define NOISE_FILTER        0.01   /* Used to eliminate noise from observation */
#define EXPONENT            17     /* Can be used for call to fft */
#define LINESIZE            128    /* Maximum line length in data file */
#define SAMPLING_FREQUENCY  24     /* Number of tidal readings taken day */
#define MIN_VALUES_PER_LINE 1      /* Min # of integers on each line of data file */
#define MAX_VALUES_PER_LINE 5      /* Max # of integers on each line of data file */
#define FFT_ALGOR_BUFFER    4      /* Prevents Run Time Check Failure #2

If you experience this error upon
closing your program, increase the size
of the readings array to
NUMBER_OF_READINGS + FFT_ALGOR_BUFFER */
#define FILE_NAME           "puddlejump.txt"
#define RESULT_FILE_NAME    "result.txt"

/*
Main function drives the program.
PRE:       NULL (no pre-conditions)
POST:      NULL (no side-effects)
RETURN:    IF the program exits correctly THEN 0 ELSE 1
*/
int main(void)
{
  /* Variables */
  double readings[NUMBER_OF_READINGS]; /* Copy the data file to this array */
  double complex_component[NUMBER_OF_READINGS]; /* Can be used for the complex results of fft */
  double omega[NUMBER_OF_READINGS]; /* Tidal frequencies */
  double frequency = 0.0;                /* Tides occur with this frequency the most... */
  double amplitude = 0.0;                /* ...and that frequency occured this many times */
  int i = 0;                  /* Helpful iterator variable */
  FILE* file_pointer = NULL;               /* A "pointer" to a file */

                                     /* Starts by initializing the elements in the tidal frequency array so that each omega[i] equals
                                      i * SAMPLING_FREQUENCY / NUMBER_OF_READINGS.  The sampling frequency that we have to use when
                                            analyzing the discrete Fourier transform *has* to be the one that was used to measure the signal.
                                            There is only one correct value (in this case it's hourly, so SAMPLING_FREQUENCY = 24). Otherwise
                                            it will give us the wrong frequencies. Remember we're mixing ints and doubles here, and we want
                                            the results to be doubles, so you need to do some casting.
                                            */
                                            // for ( i = 0...
                                            //	omega[i] = ...
  for (i = 0; i < NUMBER_OF_READINGS; i++) {
	 omega[i] = i *((double) SAMPLING_FREQUENCY / NUMBER_OF_READINGS);
  }
                                            /* Opens the file (a text file, not a binary file) for reading, and not writing,
                                          by invoking the fopen_s function with the correct parameters. */
                                            // fopen_s...  
  fopen_s(&file_pointer, FILE_NAME, "r");

                                          /* Invokes the process file function, passing the the data readings array and the file pointer */
                                            // process_file(...
  process_file(readings, file_pointer);

	  /* If the file pointer does not equal NULL THEN closes the pointer */
	// if (file_pointer != ...
  if (file_pointer != NULL) {
	  fclose(file_pointer);
  }

	  /* Performs the Fourier transformation by passing the data readings, the complex result
	  array, and two other parameters to the fft function. Since arrays are pass by reference
	  and not pass by value, the function will be able to directly change the values in the
	  cells.  Contrast this with the idea of pass by value, where we pass the value of a
	  variable and if we change this value, the original variable remains unchanged. */
	  // fft(...
   fft(1, EXPONENT, readings, complex_component);

	  /* Since the Microsoft Visual Studio 2012 compiler doesn't support complex
	  numbers without some tweaking, let's square the real and the complex components,
	  add them, and take the square root.  Iterate through the elements of the readings
	  array and change each readings[i] to equal (readings[i]^2 + complex_component[i]^2)^(1/2)
	  */

	  // for (i = 0...
	  // 	readings[i] = ...
   for (i = 0; i < NUMBER_OF_READINGS; i++) {
	  readings[i] = sqrt((readings[i] * readings[i]) + (complex_component[i] * complex_component[i]));
  }
	  /*
	  Searches through the results for the tidal frequency with the greatest amplitude.
	  We needn't examine every value stored in the transformed readings array.  It is enough
	  to look through the first NUMBER_OF_READINGS / 2.  Why?  The discrete Fourier transform
	  does not accurately represent the Fourier coefficients for values of omega large than
	  NUMBER_OF_READINGS / 2 (it actually gives you the same as the first half, but in reverse
	  order), and that is why they shouldn't be considered.

	  So for each of the first NUMBER_OF_READINGS / 2 readings, make sure that the value in the
	  frequency array omega is >=  NOISE_FILTER.  If the frequency array value is greater than this
	  minimum bound, then check if the value in the readings array is greater than any other so far.
	  If it is, store both the frequency and the amplitude.
	  */
	  // for (i = 0...
  
  for (i = 0; i <= (NUMBER_OF_READINGS / 2); i++) {
	  if (omega[i] >= NOISE_FILTER) {
		  if (readings[i] > amplitude ) {
			   frequency = omega[i];
			   amplitude= readings[i] ;
		  }
	 }
  }
	  /* You can use this for debugging, or (even better) you can set a breakpoint
	  on this line, and look at the values of frequency and amplitude using
	  the debugger
	  */
	  //printf("Max Frequency = %f Max Amplitude = %f\n", frequency, amplitude);

	  /* Creates (opens) a result file using fopen_s */
	  // fopen_s...
  fopen_s(&file_pointer, RESULT_FILE_NAME, "w");

	  /* Writes the result to the file */
	  // fprintf( file_pointer, ...
  fprintf(file_pointer,"%f %f",frequency ,amplitude);

	  /* Closes the result file */
	  // if (file_pointer ...
  if (file_pointer != NULL) {
	  fclose(file_pointer); //
  }
	  /* And that's it */
	  printf("Analysis complete, result.txt created\n");
  system("pause");
  return 0;
}

/*
* Processes the data file
* PARAM:     array_to_populate is an array which is at least large enough
*            to contain the data in the specified file
* PARAM:     pointer_to_data_file is a valid and open file pointer
* PRE:       The file pointer is a reference to an open file
* PRE:       The file is a text file which contains between 1 and
*            5 real integers per line
* POST:      The contents of the file have been copied to the
*            specified array
* RETURN:    VOID
*/
void process_file(double array_to_populate[], FILE* pointer_to_data_file)
{
	/* Variable list */
	char  line_buffer[LINESIZE];
	int extracted_values[MAX_VALUES_PER_LINE];
	int readings_processed = 0;
	int values_per_line = 0;
	int i = 0;

	//???????????????///
	/* Copies the file, line by line, to line buffer using fgets in a while loop */
	// while( fgets ( line_buffer, LINESIZE, pointer_to_data_file ) ) {
	while (fgets(line_buffer, LINESIZE, pointer_to_data_file)) {

		/* Tries to extract MAX_VALUES_PER_LINE ints from the line buffer and assign
		them to local array cells using sscanf_s or equivalent.  Stores the return
		value in a local int */
		// values_per_line = ...
		values_per_line = sscanf_s(line_buffer, "%d",&extracted_values[i]);
		/* Copies the extracted integers to our data array.  Use a for loop for each
		for each of the values_per_line cells in the local array, and add the value
		stored in the cell to the end of the array we are popuolating with data value. */
		// for ( i = 0...
		for (i = 0; i < values_per_line; i++) {
			array_to_populate[i] = extracted_values[i];
		}

		/* Keep track of what has been processed.  Increment the number of readings processed
		by the number of values successfully extracted from the line in the file. */
		// readings_processed +=

		readings_processed += values_per_line;

		/* End of function */
		return;
	}
}
