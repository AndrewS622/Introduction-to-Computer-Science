#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char encode(int key, char character);

int main(int argc, string argv[])
{
    //check for a single value entry
    if (argc != 2)
    {
        printf("Please enter a single command-line positive integer.\n");
        return 1;
    }
    string val = argv[1];

    //check each character in the entry to make sure they are all numeric
    for (int i = 0, n = strlen(val); i < n; i++)
    {
        if (isalpha(val[i]))
        {
            printf("Please make sure to enter a positive number.\n");
            return 1;
        }
    }
    int key = atoi(val);

    //convert to integer and make sure value is positive
    if (key <= 0)
    {
        printf("Please make sure to enter a positive number.\n");
        return 1;
    }
    
    //convert value to a key between 1 and 26
    key = key % 26;

    //get text to be encoded
    string plaintext = get_string("plaintext: ");

    //iterate through each character and shift
    printf("ciphertext: ");
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        printf("%c", encode(key, plaintext[i]));
    }
    printf("\n");
    return 0;
}

//function will shift lowercase and uppercase letters separately and leave all other characters untouched
char encode(int key, char character)
{
    if (character >= 'a' && character <= 'z')
    {
        character = character % 'a';
        character += key;
        character = character % 26;
        return (char)(character + 'a');
    }

    else if (character >= 'A' && character <= 'Z')
    {
        character = character % 'A';
        character += key;
        character = character % 26;
        return (char)(character + 'A');
    }

    else
    {
        return character;
    }
}