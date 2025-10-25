#include<stdio.h>
#include <stdlib.h>
#include <time.h>

//function for filling the square matrix with random numbers
void fillMatrixWithRandomNumber(int numberOfRows, int numberOfColumns, int (*matrix)[numberOfColumns])
{
    const int max = 225;
    const int min = 0;


    for (int rowIterator = 0; rowIterator < numberOfRows; rowIterator++) {
        for (int columnIterator = 0; columnIterator < numberOfColumns; columnIterator++) {
            *(*(matrix + rowIterator) + columnIterator) = (rand() % (max - min + 1)) + min;
        }
    }
}

// function for rotating the matrix 90 degree
void rotateMatrixClockwise(int numberOfRows, int numberOfColumns, int (*matrix)[numberOfColumns])
{

    // first transpose the matrix
    for (int rowIterator = 0; rowIterator < numberOfRows; rowIterator++)
    {
        for (int columnIterator = rowIterator; columnIterator < numberOfColumns; columnIterator++)
        {
            int *firstTemp = (*(matrix + rowIterator) + columnIterator);
            int *secondTemp = (*(matrix + columnIterator) + rowIterator);

            int swapVariable = *firstTemp;
            *firstTemp = *secondTemp;
            *secondTemp = swapVariable;

        }
    }

    //after transpose reversing each row
    for (int rowIterator = 0; rowIterator < numberOfRows; rowIterator++)
        {
            int start = 0;
            int end = numberOfColumns - 1;

            while (start < end)
            {
                int *firstValue = (*(matrix + rowIterator) + start);
                int *lastValue = (*(matrix + rowIterator) + end);

                int swapVariable = *firstValue;
                *firstValue = *lastValue;
                *lastValue = swapVariable;

                start++;
                end--;
            }

        }
}

// function for applying smoothening filter after rotation of matrix
void smootheningFilter(int numberOfRows, int numberOfColumns, int (*matrix)[numberOfColumns])
{
    int maxValue = 256; // maxValue according to range of number in the matrix for encoding the original and smoothen value in-place
    for (int rowIterate = 0; rowIterate < numberOfRows; rowIterate++)
    {
         for(int columnIterate = 0; columnIterate < numberOfColumns; columnIterate++)
         {
            int sum = 0;
            int count = 0;

            for (int outerIterator = rowIterate -1; outerIterator <= rowIterate + 1; outerIterator++)
            {
                for (int innerIterator = columnIterate - 1; innerIterator <= columnIterate + 1; innerIterator++)
                {
                    if (outerIterator >= 0 && outerIterator < numberOfRows && innerIterator >= 0 && innerIterator < numberOfColumns)
                    {
                        sum = sum + *(*(matrix + outerIterator) + innerIterator) % maxValue; // decoding the original value
                        count++;
                    }
                }
            }
            int average = sum / count;
            *(*(matrix + rowIterate) + columnIterate) += average * maxValue; // encoding the original and smoothen value in the same cell
         }
    }


    for (int rowIterate = 0; rowIterate < numberOfRows; rowIterate++)
    {
        for (int columnIterate = 0; columnIterate < numberOfColumns; columnIterate++)
        {
            *(*(matrix + rowIterate) + columnIterate) /= maxValue; //decoding the new smoothen value
        }
    }


}

int main()
{
    srand(time(NULL)); // changing the seed value

    int matrixSize = 0;
    printf("Enter Matrix Size between 2 and 10: ");
    scanf("%d", &matrixSize);

    if (matrixSize <= 2 || matrixSize > 10) {
        printf("Invalid Matrix Size\n");
        return 1;
    }

    int matrix[matrixSize][matrixSize];
    fillMatrixWithRandomNumber(matrixSize, matrixSize, matrix);
    printf("Original Matrix:\n");
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            printf("%d ", *(*(matrix + i) + j));
        }
        printf("\n");
    }
    printf("\n");

    rotateMatrixClockwise(matrixSize, matrixSize, matrix);
    printf("Rotated Matrix:\n");
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            printf("%d ", *(*(matrix + i) + j));
        }
        printf("\n");
    }
    printf("\n");

    smootheningFilter(matrixSize, matrixSize, matrix);
    printf("Smoothed Matrix:\n");
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            printf("%d ", *(*(matrix + i) + j));
        }
        printf("\n");
    }
    printf("\n");
    return 0;
}
