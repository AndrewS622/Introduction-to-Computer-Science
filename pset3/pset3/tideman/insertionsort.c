#include <cs50.h>
#include <stdio.h>

const int n = 10;
int array[n] = {5,2,7,4,0,1,9,15,100,-2};

void insertionsort(void);

int main(void)
{
    insertionsort();
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", array[i]);
    }
}

void insertionsort(void)
{
    int temp = 0;
    for (int i = 1; i < n; i++)
    {
        int j = i - 1;
        temp = array[i];
        while (temp < array[j])
        {
            if (j == -1)
            {
                break;
            }
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = temp;
    }
}