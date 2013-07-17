#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "prefixStat.h"

/*
 * -p parameter of prefixStat.
 * The function takes the word/prefix input by the user and prints all words in which the given input word appears as prefixes.
 * The function also takes the given file to use in the function.
 * The function gets the words on each line and does a case insensitive check to see if the input word prefix matches any of the words listed.
 * If it does match, then that word from the file is printed.
 * This process continues for all words and all lines in the file.
 */
void prefix(char *inputWord, FILE *inputFile)
{
	char wordList[1000];
	char *currentLine;
	char *currentWord;
	char tempWord[1000];
	int i;
	int foundWords = 0;
	currentLine = fgets(wordList, 1000, inputFile);
	while(currentLine != NULL) /*While the current line has words*/
	{
		currentWord = strtok(currentLine, " "); /*Tokenize the word, separated by a space.*/
		while(currentWord != NULL) /*While the word exists*/
		{
			for(i = 0; currentWord[i] != '\0'; i++) /*Copy the currentWord to the tempWord character array*/
				tempWord[i] = currentWord[i];
			for (i = 0; inputWord[i] != '\0'; i++)
				inputWord[i] = (char)tolower(inputWord[i]); /*Convert the inputWord to lower case*/
			for (i = 0; tempWord[i] != '\0'; i++)
			    tempWord[i] = (char)tolower(tempWord[i]); /*Convert tempWord to lower case*/
			if(strncmp(inputWord,tempWord,strlen(inputWord)) == 0) /*Do a case insensitive compare between the word input by the user and the current word. If the two strings match, the foundWords counter increases and the matching word is printed.*/
			{
				foundWords++;
				printf("%s ",currentWord);
			}
			currentWord = strtok(NULL, " "); /*Tokenizes the next word separated by a space*/
		}
		currentLine = fgets(wordList, 1000, inputFile); /*Moves onto the next line*/
		printf("\n"); /*Keeps thing neater looking at execution.*/
	}
	if (foundWords == 0)
		printf("Sorry, no words have your input '%s' as a prefix.\n", inputWord);
	fclose(inputFile);
	exit(0);
}

/*
 * -w parameter of prefixStat.
 * The function initializes a word counter and goes through all words and all lines of the given file, printing the word and increasing the word counter by 1 each time a word is found.
 */
void wordCount(FILE *inputFile)
{
	int wordCount = 0;
	char wordList[1000];
	char *currentLine;
	char *currentWord;
	currentLine = fgets(wordList, 1000, inputFile);
	while(currentLine != NULL) /*While the currentLine is not blank*/
	{
		currentWord = strtok (currentLine, " "); /*Tokenizes and prints the first word, increases wordCounter by 1 and moves onto the next word.*/
		printf ("%s", currentWord);
		wordCount++;
		currentWord = strtok(NULL, " ");
		while (currentWord != NULL) /*This while loop ensures all the other words on the line are counted by the function. If first while loop was excluded, first word on each line would not be counted.*/
		{
			printf(" %s", currentWord);
			wordCount++;
			currentWord = strtok(NULL, " ");
		}
		currentLine = fgets(wordList, 1000, inputFile); /*Moves onto next line*/
	}
	printf("\n%d\n",wordCount);
	fclose(inputFile); /*Closes file when done*/
	exit(0);
}

/*
 * -l parameter of prefixStat.
 * The function initializes a line counter and goes through all lines of the given file, printing the line of words and increasing the line counter by 1 each time a new line is found.
 */
void lineCount(FILE *inputFile)
{

	int lineCount = 0;
	char wordList[1000];
	char *currentLine;
	currentLine = fgets(wordList, 1000, inputFile); /*Retrieves first line from inputFile*/
	while(currentLine != NULL) /*While the currentLine is not blank, print the line, increase the lineCounter by 1 and move onto the next line in the file.*/
	{
		printf(currentLine);
		lineCount++;
		currentLine = fgets(wordList, 1000, inputFile);
	}
	printf("\n%d\n",lineCount);
	fclose(inputFile); /*Closes the inputFile at end of function execution.*/
	exit(0);
}

/* -h parameter of prefixStat. Also prints whenever an stderr is printed.
 * Prints the help for how a user can run the program and quits.
 */
void help()
{
	printf("prefixStat Help:\nprefixStat <option> <input word>\nValid options are:\n-h: Help\n-w: Output words and word count.\n-l: Output lines and line count.\n-p <input word>: Outputs the words in which the given input word appears as\nprefixes.\n");
	exit(0); /*Ensures proper termination of program.*/
}

/*
 * Main function of the program. First checks if a valid number of arguments is given. If there are more than 2 arguments and the first argument is not -p (where a 2nd argument HAS to be given) or there is an additional argument after the -p <word> arguments, an error is printed.
 * Otherwise, the program opens the test.dat file in read mode.
 * If the file cannot be found, an error is printed and the program exits.
 * Otherwise, the file is closed (for memory conservation) and the program loops through the command line arguments to evaluate which function should be called and executed.
 */
int main(int argc, char *argv[])
{
	int i;
	int functionCount = 0;
	if(argc == 1 || (argc > 2 && (strcmp(argv[1],"-p")!=0 || argv[3] != NULL))) /*If no input arguments are given, if there's more than 2 arguments given and the first argument is not a -p (meaning another argument has to be included with it) OR there's more than 3 arguments passed to the program:*/
	{
			fprintf(stderr,"Error: Invalid number of arguments passed.\n"); /*Standard error is printed and help function is run as dictated by instructions.*/
			help();
	}
	else /*If argument checks are OK, open the test.dat file in read only mode.*/
	{
		FILE *testdat = fopen("test.dat","r");
		if(testdat == NULL) /*If the test.dat file does not exist, an error is printed and program is terminated.*/
		{
			fprintf(stderr,"Error: File cannot be found.\n");
			exit(0);
		}
		else /*If the program can open the file, it closes it to save space.*/
		{
			fclose(testdat);
			for(i=0; i < argc; i++) /*Loops through each input argument*/
			{
				if(strcmp(argv[i],"-p") == 0) /*If argument is -p and a word is given after -p by the user, the test.dat file is opened and the prefix function is run.*/
				{
					if(argv[i+1] != NULL)
					{
						testdat = fopen("test.dat","r");
						functionCount++;
						prefix(argv[i+1], testdat);
					}
					else /*Otherwise, an error is given.*/
					{
						functionCount++;
						fprintf(stderr,"Error: Need to define an input word.\n");
					}
				}
				if(strcmp(argv[i],"-w") == 0) /*If argument is -w, the test.dat file is opened and the wordCount function is executed.*/
				{
					testdat = fopen("test.dat","r");
					functionCount++;
					wordCount(testdat);
				}
				if(strcmp(argv[i],"-l") == 0) /*If argument is -l, the test.dat file is opened and the lineCount function is executed.*/
				{
					testdat = fopen("test.dat","r");
					functionCount++;
					lineCount(testdat);
				}
				if(strcmp(argv[i],"-h") == 0) /*If argument is -h, the help function is executed.*/
				{
					functionCount++;
					help();
				}
			}
			if (functionCount == 0) /*If no valid input arguments are passed to the program,*/
			{
				fprintf(stderr,"Error: No valid input arguments given.\n");
				help();
			}
		}
	}
	return 0; /*In order to meet C standards, the main function must return an int. 0 indicates successful program execution.*/
}
