#include <cs50.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
    //start off with negative value to prompt user input
    float money = -1;
    //keep prompting until positive value obtained
    while (money < 0)
    {
        money = get_float("Change owed: ");
    }
    
    //convert to cents and truncate
    int cents = round(money * 100);
    int coin_num = 0;
    
    //iterate through, subtracting largest value until 0 remains
    while (cents > 0)
    {
        if (cents >= 25)
        {
            cents -= 25;
        }
        else if (cents >= 10)
        {
            cents -= 10;
        }
        else if (cents >= 5)
        {
            cents -= 5;
        }
        else
        {
            cents -= 1;
        }
        //increment each time a coin value is subtracted
        coin_num++;
    }
    printf("%i\n", coin_num);
}