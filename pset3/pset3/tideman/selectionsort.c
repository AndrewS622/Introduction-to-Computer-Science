#include <cs50.h>
#include <stdio.h>

const int n = 10;
int array[n] = {5,2,7,4,0,1,9,15,100,-2};

void selectionsort(void);

int main(void)
{
    selectionsort();
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", array[i]);
    }
}

void selectionsort(void)
{
    int temp = 0;
    for (int i = 0; i < n; i++)
    {
        int minval = INT_MAX;
        int minloc = -1;
        for (int j = i; j < n; j++)
        {
            if (array[j] < minval)
            {
                minval = array[j];
                minloc = j;
            }
        }
        temp = array[minloc];
        array[minloc] = array[i];
        array[i] = temp;
    }
}