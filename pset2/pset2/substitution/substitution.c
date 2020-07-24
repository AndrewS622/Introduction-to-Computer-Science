#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char encode(string key, char character);

int main(int argc, string argv[])
{
    //check for a single value entry
    if (argc != 2)
    {
        printf("Please enter a single command-line encoding sequence.\n");
        return 1;
    }
    string val = argv[1];

    if (strlen(val) != 26)
    {
        printf("Please make sure to enter 26 letters.\n");
        return 1;
    }

    //check each character in the entry to make sure they are all alphabetic and that each letter appears once
    int n = strlen(val);
    int letters[n];
    //use array of zeros and set to 1 once each character is detected
    for (int i = 0; i < n; i++)
    {
        letters[i] = 0;
    }
    for (int i = 0; i < n; i++)
    {
        int valtemp;
        //first, check to see if letter is alphabetic
        if (!isalpha(val[i]))
        {
            printf("Please make sure to enter 26 unique letters.\n");
            return 1;
        }
        //then, check to make sure the letter hasn't been detected before
        if (val[i] >= 'a' && val[i] <= 'z')
        {
            valtemp = (int)(val[i] % 'a');
        }
        else
        {
            valtemp = (int)(val[i] % 'A');
        }
        if (letters[valtemp] == 1)
        {
            printf("Please make sure to enter each letter once.\n");
            return 1;
        }
        //if it hasn't, set the value in the reference array to 1 so it cannot be duplicated
        else
        {
            letters[valtemp] = 1;
        }
    }

    //get text to be encoded
    string plaintext = get_string("plaintext: ");

    //iterate through each character and shift
    printf("ciphertext: ");
    for (int i = 0, m = strlen(plaintext); i < m; i++)
    {
        printf("%c", encode(val, plaintext[i]));
    }
    printf("\n");
    return 0;
}

//function will shift lowercase and uppercase letters separately and leave all other characters untouched
char encode(string key, char character)
{
    //if character is lowercase, mod down to 0-25, take the corresponding value of the key
    if (character >= 'a' && character <= 'z')
    {
        character = character % 'a';
        char shift = key[(int) character];
        //mod the case of the key down to 0-25
        if (shift >= 'a' && shift <= 'z')
        {
            shift = shift % 'a';
        }
        else
        {
            shift = shift % 'A';
        }
        //return the key shifted back to the correct case
        return (char)(shift + 'a');
    }

    //repeat for uppercase
    else if (character >= 'A' && character <= 'Z')
    {
        character = character % 'A';
        char shift = key[(int) character];
        if (shift >= 'a' && shift <= 'z')
        {
            shift = shift % 'a';
        }
        else
        {
            shift = shift % 'A';
        }
        return (char)(shift + 'A');
    }

    else
    {
        return character;
    }
}