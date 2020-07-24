#include <cs50.h>
#include <stdio.h>

const int n = 10;
int array[n] = {5,2,7,4,0,1,9,15,100,-2};
int array_sorted[n];

void mergesort(int l, int r);

int main(void)
{

    for (int i = 1; i < n; i++)
    {
        array_sorted[i] = array[i];
    }
    mergesort(0, n-1);
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", array[i]);
    }
}

void mergesort(int l, int r)
{
    if (l == r)
    {
        return;
    }
    int m = (l + r)/2;
    mergesort(l, m);
    mergesort(m + 1, r);

    int arrayloc = l;
    int i = l;
    int j = m + 1;
    int n1 = m + 1;
    int n2 = r + 1;
    while (i < n1 || j < n2)
    {
        if (i >= n1)
        {
            array_sorted[arrayloc] = array[j];
            j++;
        }
        else if (j >= n2)
        {
            array_sorted[arrayloc] = array[i];
            i++;
        }
        else
        {
            if (array[i] < array[j])
            {
                array_sorted[arrayloc] = array[i];
                i++;
            }
            else
            {
                array_sorted[arrayloc] = array[j];
                j++;
            }
        }
        arrayloc++;
    }
    for (int k = l; k <= r; k++)
    {
        array[k] = array_sorted[k];
        printf("%i\n",array[k]);
    }
    printf("\n");
}