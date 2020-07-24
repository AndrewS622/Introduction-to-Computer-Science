#include <stdio.h>
#include <cs50.h>

int main(void)
{
    long number = get_long("Please enter the card number: ");                                   //get long card number
    int length = 0;
    int sum = 0; 
    bool even = 0;
    int lastTwo;
    while (number > 0)                                                                          //loop through each digit
    {
        int digit = number % 10;                                                                //extract current ones digit
        number = number / 10;                                                                   //move everything to the right one place
        if (even)                                                                               //if place is even, multiply by 2
        {
            digit = 2 * digit;
        }
        if (digit > 9)                                                                          //if above 10, add 1 + X for 1X 
        {
            sum = sum + 1 + (digit % 10);
        }
        else
        {
            sum += digit;                                                                       //otherwise, just add X
        }
        length ++;                                                                              //keep track of length
        even = !even;                                                                           //next digit will be in opposite spot (even or odd)
        if (number < 100 && number >= 10)                                                       //keep track of last two digits for ID later
        {
            lastTwo = number;
        }
    }
    
    if (sum % 10)                                                                               //always invalid if it doesn't meet the sum criterion
    {
        printf("INVALID\n");
    }
    else if ((length == 15) && (lastTwo == 34 || lastTwo == 37))                                //check for AMEX
    {
        printf("AMEX\n");
    }
    else if (length == 16 && lastTwo > 50 && lastTwo <= 55)                                     //check for MasterCard
    {
        printf("MASTERCARD\n");
    }
    else if ((length == 13 || length == 16) && ((lastTwo / 10) == 4))                           //check for Visa
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");                                                                    //invalid otherwise
    }
}