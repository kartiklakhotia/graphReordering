#include "sort.h"

void merge (int* arr, int* key, int low, int high, int mid)
{
    int i = low;
    int j = mid+1;
    int k = 0;
    int* temp = new int [high-low+1];
    int* tempKey = new int [high-low+1];
    while(i<=mid && j<=high)
    {
        if (arr[i] >= arr[j])
        {
            tempKey[k] = key[i];
            temp[k++] = arr[i++];
        }
        else
        {
            tempKey[k] = key[j];
            temp[k++] = arr[j++];
        }
    }
    while(i<=mid)
    {
        tempKey[k] = key[i];
        temp[k++] = arr[i++];
    }
    while(j<=high)
    {
        tempKey[k] = key[j];
        temp[k++] = arr[j++];
    }
    for (i=low; i<=high; i++)
    {   
        key[i] = tempKey[i-low];
        arr[i] = temp[i-low];
    }
    delete[] temp;
    delete[] tempKey;
}

void mergeSort (int* arr, int* key, int low, int high)
{

    if (low >= high)
        return;
    if ((high - low) == 1)
    {
        if (arr[high] > arr[low])
        {
            int temp = arr[high];
            arr[high] = arr[low];
            arr[low] = temp;
            temp = key[high];
            key[high] = key[low];
            key[low] = temp;
        }
        return;
    }
    int mid = (low + high)/2;
    mergeSort(arr, key, low, mid);
    mergeSort(arr, key, mid + 1, high);
    merge(arr, key, low, high, mid);
    
} 

void mergeWOkey (double* arr, int low, int high, int mid)
{
    int i = low;
    int j = mid+1;
    int k = 0;
    double* temp = new double [high-low+1];
    while(i<=mid && j<=high)
    {
        if (arr[i] >= arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }
    while(i<=mid)
        temp[k++] = arr[i++];
    while(j<=high)
        temp[k++] = arr[j++];
    for (i=low; i<=high; i++)
        arr[i] = temp[i-low];
    delete[] temp;
}

void mergeSortWOkey (double* arr, int low, int high)
{

    if (low >= high)
        return;
    if ((high - low) == 1)
    {
        if (arr[high] > arr[low])
        {
            double temp = arr[high];
            arr[high] = arr[low];
            arr[low] = temp;
        }
        return;
    }
    int mid = (low + high)/2;
    mergeSortWOkey(arr, low, mid);
    mergeSortWOkey(arr, mid + 1, high);
    mergeWOkey(arr, low, high, mid);
    
} 
