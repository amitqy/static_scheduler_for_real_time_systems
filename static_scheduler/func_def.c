#include <stdio.h>
#include <stdbool.h>
#include "sched.h"

int total_jobs;
int total_aperiodic_jobs; //varibale indicating total no. of periodic jobs in taskset

int max(int a, int b)
{
    if (a > b)
        return a;
    if (b > a)
        return b;
    if (a == b)
        return a;
    return -1;
}
int gcd(int a, int b)
{
    if (b == 0)
        return a;
    return gcd(b, a % b);
}
int find_hyperperiod(int a, int b)
{
    int m = 1;
    while (m % a || m % b)
    {
        m++;
    }
    return m;
}
void swap(struct job *a, struct job *b)
{
    struct job t = *a;
    *a = *b;
    *b = t;
}
void swap2(struct aperiodic_job *a, struct aperiodic_job *b)
{
    struct aperiodic_job t = *a;
    *a = *b;
    *b = t;
}
int partition(struct job j1[total_jobs], int low, int high)
{
    int pivot = j1[high].abs_deadline;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {

        if (j1[j].abs_deadline <= pivot)
        {
            i++;
            swap(&j1[i], &j1[j]);
        }
    }
    swap(&j1[i + 1], &j1[high]);
    return (i + 1);
}

void sort_with_deadlines(struct job j1[total_jobs], int low, int high) //to sort with absolute deadlines
{
    if (low < high)
    {

        int pi = partition(j1, low, high);
        sort_with_deadlines(j1, low, pi - 1);
        sort_with_deadlines(j1, pi + 1, high);
    }
}
void sort_with_exe_time(struct aperiodic_job js[total_aperiodic_jobs], int low, int high)
{
    if (low < high)
    {

        int pi = partition2(js, low, high);
        sort_with_exe_time(js, low, pi - 1);
        sort_with_exe_time(js, pi + 1, high);
    }
}
int partition2(struct aperiodic_job js[total_aperiodic_jobs], int low, int high)
{
    int pivot = js[high].exe_time;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {

        if (js[j].exe_time <= pivot)
        {
            i++;
            swap2(&js[i], &js[j]);
        }
    }
    swap2(&js[i + 1], &js[high]);
    return (i + 1);
}
int min(int a, int b)
{
    if (a < b)
        return a;
    if (b < a)
        return b;
    if (a == b)
        return a;
    return -1;
}
