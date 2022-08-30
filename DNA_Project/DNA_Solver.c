/*
 File:        dna.c
 Purpose:     Consumes a formatted DNA sequence file
              to determine which of a group of candidate
              sequences of nucleotides best matches a
              specified sample.  The formatted DNA
              sequence file is a txt file (threes samples
              are provided in the Resource Files folder).
 */

  /* Preprocessor directives */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "dna.h"

/*
 * Main function drives the program.
 * PRE:       NULL (no pre-conditions)
 * POST:      NULL (no side-effects)
 * RETURN:    IF the program exits correctly
 *            THEN 0 ELSE 1
 */
int main(void)
{
  /* Variables */
  char* sample_segment = NULL;
  char** candidate_segments = NULL;
  char    main_menu_choices[NUMBER_OF_MENU_ITEMS][MAX_MENU_ITEM_SIZE]
    = { "1. Load file", "2. Perform analysis", "3. Exit" };
  int     number_of_candidates = 0;
  int     menu_choice = 0;
  int     number_of_main_menu_choices = NUMBER_OF_MENU_ITEMS;

  /* Generates menu and responds to user input */
  do {

    /* Prints the menu and asks the user to make a choice */
    menu_choice = get_menu_choice(PROGRAM_NAME, main_menu_choices, number_of_main_menu_choices);

    switch (menu_choice)
    {
      /* If the user chooses 1, we need to load a file */
    case 1:
      /* First clear any existing memory, then ask the user to enter the file name,
         and then try to load it */
      clear_memory(&sample_segment, &candidate_segments, &number_of_candidates);
      number_of_candidates = load_file(&sample_segment, &candidate_segments);
      break;

      /* If the user chooses 2, we want to process the file */
    case 2:
      /* But only if a file has been already loaded! */
      if (sample_segment != NULL) {
        analyze_segments(sample_segment, candidate_segments, number_of_candidates);
      }
      break;

      /* If the user chooses 3, we want to free dynamically allocated memory, and end the program */
    case 3:
      clear_memory(&sample_segment, &candidate_segments, &number_of_candidates);
      end_program(0);
      break;

      /* We have to have a default choice in a switch statement */
    default:
      ;
      break;
    }
  } while (1); /* Ad infinitum (1 is always true in C, 0 is always false, so while(1) is an infinite loop) */

  end_program(0);
}

/*
 * Displays a menu, asks the user to choose a menu item, and returns
 * a valid menu choice.
 *
 * Using fgets and sscanf_s ensures that we only accept correct
 * user input: an integer representing one of the numbered
 * menu items, that is neither preceded nor proceeded by any
 * additional input except for white space.
 *
 * PARAM:     Pointer to a null-terminated string representing the menu name
 * PARAM:     Array of strings (menu choices)
 * PARAM:     Number of menu choices
 * PRE:       NULL (no pre-conditions)
 * POST:      NULL (no side-effects)
 * RETURN:    INT representing valid menu choice
 */
int get_menu_choice(char* menu_name,
  char menu_choices[][MAX_MENU_ITEM_SIZE],
  int number_of_choices)
{
  /* Variable list */
  char line[BUFSIZE];
  char extra[BUFSIZE];
  int  i = 0, menu_item;

  do {
    printf("%s\n", menu_name);
    for (i = 0; i < number_of_choices; ++i) {
      printf("%s\n", menu_choices[i]);
    }
    printf("> ");

    /* If user enters EOF which is Control-C in Windows, then fgets returns a null pointer which is
       interpreted as 0.  The user probably wants to quit the program, so we clear the error in
       standard input and return the final menu item (quit, #3) */
    if (!(fgets(line, BUFSIZE, stdin))) {
      clearerr(stdin);
      return number_of_choices;
    }
  }
  /* Disallows incorrect menu choices by accepting [0, #ofchoices] and ignoring other input */
  while (sscanf_s(line, "%d%s", &menu_item, extra, BUFSIZE) != 1
    || menu_item < 0 || menu_item > number_of_choices);
  return menu_item;
}

/*
 * Clears any dynamically allocated memory and resets the candidate count.
 *
 * PARAM:     double pointer to the sample segment
 * PARAM:     triple pointer to the candidate segments
 * PARAM:     pointer to the int that stores the number of candidate segments
 * PRE:       pointers are valid pointers to dynamically allocated and populated memory
 * POST:      memory has been deallocated, number of candidates reset to 0
 * RETURN:    VOID
 */
void clear_memory(char** sample_segment, char*** candidate_segments, int* number_of_candidates)
{
  int i = 0;

  if (*sample_segment != NULL) {
    free(*sample_segment);
    *sample_segment = NULL;
  }
  if (*candidate_segments != NULL) {
    for (i = 0; i < *number_of_candidates; ++i) {
      free(*(*candidate_segments + i)); /*  Freeing triple pointers can be tricky! */
    }
    free(*candidate_segments);
    *candidate_segments = NULL;

    *number_of_candidates = 0; /* We don't want to forget to reset the candidate counter! */
  }
}

/*
 * Copies the specified file into main memory and returns the number of candidate
 * segments found.
 *
 * PARAM:     Double pointer to the sample segment
 * PARAM:     Triple pointer to a collection of candidate segments
 * PRE:       File with the user-specified name exists in the Resource Files folder
 * POST:      File has been copied to main memory
 * RETURN:    Number of candidate sequences in specified file
 */
int load_file(char** sample_segment, char*** candidate_segments)
{
  /* Variable list */
  char   file_name[BUFSIZE];
  FILE* fp = NULL;
  int    i = 0;
  int    error = 0;
  int    number_of_candidates = 0;

  /* Acquires file name from user */
  get_user_input("\nEnter file name: ", file_name);

  printf("Loading file %s\n", file_name);

  /* Opens file */
  error = fopen_s(&fp, file_name, "r");

  /* If the return value specifies that the file cannot be opened,
  prints a suitable message to standard output and returns 0 */
  if (error != 0) {
    fprintf(stderr, "File %s cannot be loaded\n", file_name);
    return 0;
  }

  /* Extracts contents of the file, and determines number of candidate segments */
  number_of_candidates = extract_dna(fp, sample_segment, candidate_segments);

  /* Closes the file and returns number of candidates extracted from the DNA */
  fclose(fp);
  return number_of_candidates;
}

/*
 * Ends the program.  Forces the system to pause first.
 * PARAM:     exit value
 * PRE:       NULL (no pre-conditions)
 * POST:      execution has ended
 * RETURN:    VOID
 */
void end_program(int exit_value)
{
  system("pause");
  exit(exit_value);
}

/*
 * Determines if the specified molecules (chars) are a base pair
 * PARAM:     a char
 * PARAM:     a char
 * PRE:       molecule is an alphanumeric ascii char
 * RETURN:    IF nucleotide_1 == nucleotide_2
 *			  THEN 1
 *            ELSE 0
 */
int is_base_pair(char nucleotide_1, char nucleotide_2)
{
  switch (nucleotide_1)
  {
  case 'A': case 'a':
    if (nucleotide_2 == 'T' || nucleotide_2 == 't')
      return 1;
    break;
  case 'T': case 't':
    if (nucleotide_2 == 'A' || nucleotide_2 == 'a')
      return 1;
    break;
  case 'C': case 'c':
    if (nucleotide_2 == 'G' || nucleotide_2 == 'g')
      return 1;
    break;
  case 'G': case 'g':
    if (nucleotide_2 == 'C' || nucleotide_2 == 'c')
      return 1;
    break;
  default:
    return 0;
    break;
  }
  return 0;
}

/*
 * Acquires and returns the specified codon code's index from the
 * codon_codes 2D array in the header file.
 *
 * PARAM:     A pointer to a sequence of characters
 * PRE:       Valid pointer to a sequence of 3 characters.
 * RETURN:    IF the specified codon_code is in the codon_codes 2D array in the header file
 *            THEN returns the index of the specified codon_code in the codon_codes 2D array
 *            ELSE -1
 */
int get_codon_index(char* codon_code)
{
  int i;
  for (i = 0; i < NUMBER_OF_CODONS; ++i) {

    if (codon_codes[i][0] == codon_code[0] &&
      codon_codes[i][1] == codon_code[1] &&
      codon_codes[i][2] == codon_code[2]) {
      return i;
    }
  }
  return -1;
}

/*
 * Displays specified message and asks the user for a response.
 *
 * PARAM:     Pointer to a null-terminated string representing the message
 * PARAM:     Pointer to a buffer where the response will be stored
 * PRE:       Response buffer is BUFSIZE characters in size
 * POST:      Response buffer contains the user response
 * RETURN:    VOID
 */
void get_user_input(char* message, char* response)
{
  char line[BUFSIZE];

  do {
    printf("%s", message);

    /* If user enters EOF which is Control-C in Windows
       then fgets returns a null pointer which is
       interpreted as 0.  We clear the error in
       standard input and end program */
    if (!(fgets(line, BUFSIZE, stdin))) {
      clearerr(stdin);
      printf("Error acquiring user input\n");
      end_program(1);
    }
  } while (sscanf_s(line, "%s", response, BUFSIZE) != 1);

  return;
}

/*
 * Extracts the DNA segment information stored in a correctly formatted file.
 *
 * This function does not copy newline characters, but it does append a terminating
 * null character to the end of each DNA segment stored in the heap.  This lets us
 * use functions from string.h (like strncmp and strlen) to process the DNA segments.
 *
 * PARAM:     Pointer to a filestream to copy to main memory
 * PARAM:     Double pointer to the sample segment
 * PARAM:     Triple pointer to a collection of candidate segments
 * PRE:       All the pointers are valid
 * POST:      sample_segment now points to dynamically allocated memory that
 *            contains the sample DNA sequence extracted from the specified file
 * POST:      candidate_segments now points to dynamically allocated memory that
 *            contains the candidate DNA sequences extracted from the specified file
 * RETURN:    the number of candidate_segments extracted from the specified file
 */
int extract_dna(FILE* file_pointer, char** sample_segment, char*** candidate_segments)
{
  /* Variables */
  int  i = 0, j = 0;
  int  return_value = 0;
  int  new_line = 0;
  int  line_length = 0;
  int  sequence_length = 0;
  int  number_of_candidates = 0;
  char character = ' ';
  char line_buffer[BUFSIZE];

  /* Moves to the beginning of the file */
  fseek(file_pointer, 0, SEEK_SET);

  /* Here's an easy way to ignore the first line (the header)! */
  while ((character = fgetc(file_pointer)) != '\n') { ; }

  /* Acquires sample sequence (we know it's in the second line) */
  while (new_line == 0 && fgets(line_buffer, BUFSIZE, file_pointer)) {

    /* Check if the line ends with a newline character and increments the sample length */
    if (line_buffer[strlen(line_buffer) - 1] == '\n') {
      new_line = 1;
      line_length = strlen(line_buffer) - 1;
    }
    else {
      line_length = strlen(line_buffer);
      new_line = 0;
    }

    /* (Re)allocates some space for the (additional) nucleotides */
    *sample_segment = (char*)realloc(*sample_segment, sizeof(char) * (sequence_length + line_length));

    /* Copies the contents of the line buffer to the end of the dynamically (re)allocated memory */
    for (i = 0; i < line_length; ++i) {
      *(*sample_segment + sequence_length + i) = line_buffer[i];
    }
    sequence_length += line_length;
  }

  /* Adds terminating null character to sample, so we can treat it as a null-terminated string */
  *sample_segment = (char*)realloc(*sample_segment, sizeof(char) * (sequence_length + 1));
  *(*sample_segment + sequence_length) = '\0';

  /* Acquires number of candidate sequences (from the third line of the file) */
  fgets(line_buffer, BUFSIZE, file_pointer);
  return_value = sscanf_s(line_buffer, "%d", &number_of_candidates);

  /* Allocates pointers for correct number of candidate sequences */
  *candidate_segments = (char**)malloc(sizeof(char*) * number_of_candidates);
  for (i = 0; i < number_of_candidates; ++i) {
    *(*candidate_segments + i) = NULL; /* What happens if we omit this line? */
  }

  /* Copies each candidate sequence, in order, from the file to the memory we just
     dynamically allocated */
  for (i = 0; i < number_of_candidates; ++i) {

    sequence_length = 0; /* Resets the variable */
    new_line = 0; /* Resets the variable */

    /* Here's an easy way to ignore each candidate sequence's header */
    while ((character = fgetc(file_pointer)) != '\n') { ; }

    /* Acquires candidate sequence */
    while (new_line == 0 && fgets(line_buffer, BUFSIZE, file_pointer)) {

      /* Check if the line ends with a newline character and sets length */
      if (line_buffer[strlen(line_buffer) - 1] == '\n') {
        new_line = 1;
        line_length = strlen(line_buffer) - 1;
      }
      else {
        line_length = strlen(line_buffer);
        new_line = 0;
      }

      /* (Re)allocates some space for the (additional) nucleotides */
      *(*candidate_segments + i) = (char*)realloc(*(*candidate_segments + i), sizeof(char) * (sequence_length + line_length));

      /* Copies the contents of the line buffer to the end of the dynamically (re)allocated memory */
      for (j = 0; j < line_length; ++j) {
        *(*(*candidate_segments + i) + sequence_length + j) = line_buffer[j];
      }
      sequence_length += line_length;
    }

    /* Adds terminating null character to candidate, so we can treat it as a null-terminated string */
    *(*candidate_segments + i) = (char*)realloc(*(*candidate_segments + i), sizeof(char) * (sequence_length + 1));
    *(*(*candidate_segments + i) + sequence_length) = '\0';
    new_line = 0;
  }

  return number_of_candidates;
}

/*
 * Analyzes the segments.  This is a simple and straight-forward algorithm which does
 * not include any optimization, except that it returns immediately if a perfect match is found.
 *
 * This function must do these things (though not necessarily in this order):
 *
 * a) Check if any candidate segments are perfect matches, and if so, report
 *    all of the perfect matches (and do nothing else) using a message of this format:
 *    "Candidate number %d is a perfect match\n" where %d is the candidate
 *    segment's number in its sample file, then return from the function
 * b) If no perfect matches are found, then find a best match using
 *    the scoring method described in the lab specification
 * c) After a best match is found, the scores for ALL of the candidates that are best
 *    matches is printed out and formatted like this:
 *    ("Candidate number %d matched with a best score of %d\n") where %d is the
 *    candidate segment's number in its sample file
 *
 * PARAM:     pointer to the sample segment
 * PARAM:     pointer to the candidate segments
 * PARAM:     the number of candidate segments
 * PRE:       pointers are valid pointers to dynamically allocated and populated memory
 * PRE:       number_of_candidates correctly reports the number of candidate sequences
 * POST:      NULL
 * RETURN:    VOID
 */
void analyze_segments(char* sample_segment, char** candidate_segments, int number_of_candidates)
{
  /* Some helpful variables you might want to use */
  int* scores = NULL;
  int sample_length = 0;
  int candidate_length = 0;
  int i = 0;
  int has_perfect_match = 0;
  int score = 0;

  /* Hint: Check to see if any candidate segment(s) are a perfect match, and report them
     (REMEMBER: don't ignore trailing nucleotides when searching for a perfect score) */

     // Insert your code here

     /* Hint: Return early if we have found and reported perfect match(es) */

     // Insert your code here

     /* Hint: sOtherwise we need to calculate and print all of the scores by invoking
        calculate_score for each candidate_segment and printing each result */
  for (i = 0; i < number_of_candidates; ++i) {

    // Insert your code here - maybe a call to calculate_score?
  }

  /* End of function */
  return;
}

/*
 * Compares the sample segment and the candidate segment and calculates a
 * score based on these rules:
 *
 * 1. The score begins at 0.
 * 2. Let LENGTH be the number of codons in sample segment (which is always <= the candidate's length)
 * 3. Ignore any trailing nucleotides (there may be 1 or 2, but not more)
 * 4. For each of the LENGTH codons:
 *    a) if the two codons are exactly the same, add 10 to the score
 *    b) else if the 2 codons are different but specify the same amino acid, add 5 to the score
 *    c) else if the two codons are different and do not specify the same amino acid, then
 *       examine the 3 nucleotides in the codon separately:
 *		 For each of the 3 nucleotides:
 *		i)   If the two nucleotides are the same, add 2 to the score
 *      ii)  If the 2 nucleotides belong to a matching base pair (A and T, or C and G), add 1
 *      iii) Otherwise, add zero to the score (e.g., do nothing)
 * 5. Store the result
 * 6. Now skip the first codon in the candidate, e.g., shift the sample by 1 codon,
 *    and check the score of this new alignment.  Compare this score to the score
 *    you calculated before shifting, and store the higher score.  Keep shifting
 *    the sample deeper into the candidate by one codon at a time, checking the score, and
 *    storing ONLY the top score, until itno longer fits. Once the sample (ignoring the
 *    trailing nucleotides) extends beyond the length of the candidate we stop.
 * PARAM:     pointer to the sample segment
 * PARAM:     pointer to the candidate segment
 * PRE:       pointers are valid pointers to dynamically allocated and populated memory
 * POST:      NULL
 * RETURN:    An int score representing to degree to which the two segments match.
 */
int calculate_score(char* sample_segment, char* candidate_segment)
{
  /* Some helpful variables you might (or might not) want to use */
  int temp_score = 0;
  int score = 0;
  int iterations = 0;
  int sample_length = strlen(sample_segment);
  int candidate_length = strlen(candidate_segment);
  int sample_length_in_codons = sample_length / 3;

  // Insert your code here (replace this return statement with your own code)
  return 0;
}
