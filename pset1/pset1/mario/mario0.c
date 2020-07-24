#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int n = 0;
    while (n < 1 || n > 8)                                              //make sure positive integer between 1 and 8 is obtained
    {
        n = get_int("Please enter the number of steps: ");
    }
    
     for (int i = n - 1; i >= 0; i--)                                    
    {                                                   
        //start with top row with 1 hash on either side and decrement i by 1 each time
        for (int j = 0; j < i; j++)                                     //fill in i spaces
        {
            printf(" ");
        }
        for (int j = i; j < n; j++)                                     //fill in n-i hashes
        {
            printf("#");
        }
        printf("\n"); 
    }
}