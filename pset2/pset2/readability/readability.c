#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string input);
int count_words(string input);
int count_sentences(string input);

int main(void)
{
    //Note: input should be a string of letters, numbers, spaces and punctuation without a leading or trailing space.
    string input = get_string("Text: ");

    //Count letters, words, and sentences
    int letters = count_letters(input);
    int words = count_words(input);
    int sentences = count_sentences(input);

    //Normalize letters and sentences to 100 words
    float L = 100 * (float) letters / words;
    float S = 100 * (float) sentences / words;

    //Calculate Coleman-Liau index and round
    float index_flt = 0.0588 * L - 0.296 * S - 15.8;
    int index = round(index_flt);

    //Print result
    //Use "before grade 1" and "grade 16+" for outer bounds
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

//Calculate number of letters by using isalpha function on each entry in the string
int count_letters(string input)
{
    int letters = 0;
    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (isalpha(input[i]))
        {
            letters++;
        }
    }
    return letters;
}

//Calculate number of words by counting number of spaces.
//Note: words is initialized to 1 to account for the first word, assuming there are no leading or trailing spaces
//This would have to be changed to account for leading or trailing spaces
int count_words(string input)
{
    int words = 1;
    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (isspace(input[i]))
        {
            words++;
        }
    }
    return words;
}

//Count number of sentences by counting number of punctuation marks.
int count_sentences(string input)
{
    int sentences = 0;
    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (input[i] == '.' || input[i] == '!' || input[i] == '?')
        {
            sentences++;
        }
    }
    return sentences;
}