#include <cs50.h>
#include <stdio.h>

const int n = 10;
int array[n] = {5,2,7,4,0,1,9,15,100,-2};

void bubblesort(void);

int main(void)
{
    bubblesort();
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", array[i]);
    }
}

void bubblesort(void)
{
    int swapcount = -1;
    int temp;
    while (swapcount != 0)
    {
        swapcount = 0;
        for (int i = 0; i < n-1; i++)
        {
            if (array[i] > array[i+1])
            {
                temp = array[i];
                array[i] = array[i+1];
                array[i+1] = temp;
                swapcount++;
            }
        }
    }
}