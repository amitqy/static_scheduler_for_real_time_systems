#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sched.h"
#define MAX 10000
#define MIN -10000

int min_response_time_periodic_job = MAX;
int max_response_time_periodic_job = MIN;
int min_response_time_aperiodic_job = MAX;
int max_response_time_aperiodic_job = MIN;


int main(int argc, char *argv[])
{
    int num;
    FILE *fptr;
    struct Task *head = NULL;
    struct Task *tail = NULL;

    fptr = fopen(argv[1], "r");

    int ch;
    int arr[3];
    int index = 0;
    int count = 0;
    while (fscanf(fptr, "%d", &ch) != EOF)
    {

        if (index != 3)
        {
            arr[index] = ch;
            index++;
        }

        if (index == 3)
        {

            if (head == NULL)
            {
                struct Task *temp = (struct Task *)malloc(sizeof(struct Task));
                temp->period = arr[0];
                temp->exe_time = arr[1];
                temp->rel_deadline = arr[2];
                if (temp->rel_deadline == 0)
                {
                    temp->rel_deadline = temp->period;
                }
                head = temp;
                tail = head;
                count++;
                head->next = NULL;
            }
            else
            {
                struct Task *newNode = (struct Task *)malloc(sizeof(struct Task));
                newNode->period = arr[0];
                newNode->exe_time = arr[1];
                newNode->rel_deadline = arr[2];
                newNode->next = NULL;
                if (newNode->rel_deadline == 0)
                {
                    newNode->rel_deadline = newNode->period;
                }
                tail->next = newNode;
                count++;
                tail = newNode;
            }

            index = 0;
        }
    }
    fclose(fptr);
    index = 0;
    struct Task *temp;
    temp = head;

    int arr_deadlines[count]; // array of deadlines
    int arr_periods[count];   // array of periods
    int arr_exectimes[count]; // array of execution times
    temp = head;
    int index2 = 0; //used for making array of execution times and periods
    while (temp != NULL)
    {
        arr_deadlines[index2] = temp->rel_deadline;
        arr_exectimes[index2] = temp->exe_time;
        arr_periods[index2] = temp->period;
        temp = temp->next;
        index2++;
    }
    temp = head;
    float util_fact = 0.0;
    float ex = 0;
    float pr = 0;
    while (temp != NULL)
    {
        ex = temp->exe_time;
        pr = temp->period;
        util_fact = util_fact + (ex / pr);
        temp = temp->next;
    }

    FILE *fp;
    if (util_fact > 1.0)
    {
        fp = fopen("periodicSchedule.txt", "w");
        printf(" taskset is not schedulable\n");
        fprintf(fp, " taskset is not schedulable\n ");
        return 0;
    }

    temp = head;

    int hypp = 0; // hyperperiod of the task set
    hypp = arr_periods[0];

    for (int i = 1; i < count; i++)
    {
        hypp = find_hyperperiod(hypp, arr_periods[i]);
    }
    int max_execution_time = -1; // maximum execution time of the largest job possible.

    for (int i = 0; i < count; i++)
    {
        max_execution_time = max(max_execution_time, arr_exectimes[i]);
    }
    int divs_of_hypp = 0;

    for (int i = 2; i <= hypp; i++)
    {
        if (hypp % i == 0)
            divs_of_hypp++;
    }
    int arr_divisors[divs_of_hypp]; // array of divisors of hyperperiod

    int index3 = 0; // used for making the divisors of hyperperiod array

    for (int i = 2; i <= hypp; i++)
    {
        if (hypp % i == 0)
        {
            arr_divisors[index3] = i;
            index3++;
        }
    }
    bool flag = false;
    int f = 0; // frame size
    for (int i = divs_of_hypp - 1; i >= 0; i--)
    {
        f = arr_divisors[i];
        if (f >= max_execution_time)
        {
            for (int j = 0; j < count; j++)
            {
                int pi = arr_periods[j];
                int di = arr_deadlines[j];
                if (2 * f - gcd(pi, f) <= di)
                {
                    flag = true;
                }
                else
                {
                    flag = false;
                    break;
                }
            }
            if (flag == true)
            {
                break;
            }
        }
    }

    if (flag == false)
    {
        printf("This code does not work for task splitting\n");
        fprintf(fp, "This code does not work for task splitting\n");
        return 0;
    }

    /*

   code for splitting a task

   */

    temp = head;
    int total_periodic_jobs = 0;
    while (temp != NULL)
    {
        total_periodic_jobs = total_periodic_jobs + hypp / temp->period;
        temp = temp->next;
    }

    temp = head;
    struct job j1[total_periodic_jobs]; // structure of array of all jobs of all tasks
    int index4 = 0;                     //index to insert job inside jobs array
    int task_var = 1;                   //variable to update task_number
    char str1[3];
    char str2[3];
    while (temp != NULL)
    {
        int job_count = hypp / temp->period;
        int rel_var = 0; // variable to update release time of a job
        int num = 1;
        while (job_count != 0)
        {
            j1[index4].flag = false;
            j1[index4].exec_time = temp->exe_time;
            if (job_count == hypp / temp->period)
            {
                j1[index4].arr_time = 0;
            }
            else
            {
                j1[index4].arr_time = rel_var;
            }
            rel_var = rel_var + temp->period;
            sprintf(str1, "%d", task_var);
            sprintf(str2, "%d", num);
            j1[index4].task_num = task_var;
            j1[index4].job_num = num;
            j1[index4].abs_deadline = j1[index4].arr_time + temp->rel_deadline;
            index4++;
            job_count--;
            num++;
        }
        rel_var = 0;
        task_var = task_var + 1;
        temp = temp->next;
    }
    sort_with_deadlines(j1, 0, total_periodic_jobs - 1);
    printf("\n");
    int total_frames = hypp / f;
    struct Frame fr_array[total_frames];

    fptr = fopen("periodicSchedule.txt", "w");
    int timer = 0;
    int end_of_frame = f;
    int start_of_frame = 0;
    bool s = false;
    int frame_num = 0;

    fprintf(fptr, "Frame Size:%d", f);
    fprintf(fptr, "         HyperPeriod:%d\n", hypp);
    fprintf(fptr, "Fno");
    
    while (timer <= hypp)
    {
        frame_num++;
        fprintf(fptr, "\n\n");
        // fprintf(fptr,"\n");
        
        if (end_of_frame <= hypp)
            fprintf(fptr, "%d ", frame_num);
        for (int i = 0; i < total_periodic_jobs; i++)
        {
            if (j1[i].arr_time <= start_of_frame && j1[i].exec_time + timer <= end_of_frame && j1[i].flag == 0)
            {
                j1[i].start_time = timer;
                j1[i].frame_no = frame_num;
                timer = timer + j1[i].exec_time;
                j1[i].flag = true;
                fprintf(fptr, "      j%d%d %d-%d", j1[i].task_num, j1[i].job_num, j1[i].start_time, j1[i].start_time + j1[i].exec_time);
                j1[i].completion_time = j1[i].start_time + j1[i].exec_time;
                j1[i].response_time = (j1[i].start_time + j1[i].exec_time) - j1[i].arr_time;
                if (timer == end_of_frame)
                {
                    break;
                }
            }
        }
        start_of_frame = start_of_frame + f;
        timer = start_of_frame;
        end_of_frame = start_of_frame + f;
    }
    fprintf(fptr, "\n");
    fprintf(fptr, "In the slack times Aperiodic jobs executing from :");
    fprintf(fptr, "\n\n");
    int index5 = 0;
    int arr_slack_time[total_frames];
    for (int i = 0; i < total_frames; i++)
    {
        fr_array[i].frame_start_time = i * f;
        fr_array[i].fr_end_time = fr_array[i].frame_start_time + f;
        fr_array[i].seen_frame = false;
        fr_array[i].slack_start_time = fr_array[i].frame_start_time;
        fr_array[i].frame_length = f;
    }

    for (int i = 0; i < total_periodic_jobs; i++)
    {
        index5 = j1[i].frame_no - 1;
        fr_array[index5].frame_length = f;
        fr_array[index5].fr_end_time = fr_array[index5].frame_start_time + f;
        if (fr_array[index5].seen_frame == false)
        {
            fr_array[index5].seen_frame = true;
            fr_array[index5].slack_start_time = fr_array[index5].frame_start_time + j1[i].exec_time;
        }
        else
        {
            fr_array[index5].slack_start_time = fr_array[index5].slack_start_time + j1[i].exec_time;
        }
    }

    for (int i = 0; i < total_frames; i++)
    {
        fr_array[i].free_slack_time = fr_array[i].fr_end_time - fr_array[i].slack_start_time;
        fr_array[i].slack_end_time = fr_array[i].slack_start_time + fr_array[i].free_slack_time;
        //   printf(" %d ", fr_array[i].slack_end_time);
    }

    int arr_start_slack_times[total_frames];
    int arr_free_slack_times[total_frames];

    for (int i = 0; i < total_frames; i++)
    {
        arr_start_slack_times[i] = fr_array[i].slack_start_time;
        arr_free_slack_times[i] = fr_array[i].free_slack_time;
    }

    ///////////////////////////////////////// reading the aperiodic job ///////////////////////////////////////////////////////////////////////////
    FILE *fptr2;
    int arr2[2];
    fptr2 = fopen("aperiodicJobs.txt", "r");
    struct aperiodic_job *head2 = NULL;
    struct aperiodic_job *tail2 = NULL;
    int total_aperiodic_jobs = 0;
    ch = 0;
    while (fscanf(fptr2, "%d", &ch) != EOF)
    {

        if (index != 2)
        {
            arr2[index] = ch;

            index++;
        }

        if (index == 2)
        {

            if (head2 == NULL)
            {
                struct aperiodic_job *temp2 = (struct aperiodic_job *)malloc(sizeof(struct aperiodic_job));

                temp2->arr_time = arr2[0];
                temp2->exe_time = arr2[1];
                total_aperiodic_jobs++;
                temp2->sjob_no = total_aperiodic_jobs;
                temp2->accepted = true;
                head2 = temp2;
                tail2 = head2;
                head2->next = NULL;
            }
            else
            {
                struct aperiodic_job *newNode = (struct aperiodic_job *)malloc(sizeof(struct aperiodic_job));
                newNode->arr_time = arr2[0];
                newNode->exe_time = arr2[1];
                newNode->next = NULL;
                newNode->accepted = true;
                total_aperiodic_jobs++;
                newNode->sjob_no = total_aperiodic_jobs;
                tail2->next = newNode;
                tail2 = newNode;
            }

            index = 0;
        }
    }
    struct aperiodic_job js[total_aperiodic_jobs];
    int index6 = 0;
    int s_exec_times[total_aperiodic_jobs];
    struct aperiodic_job *temp2;
    temp2 = head2;
    while (temp2 != NULL)
    {
        js[index6].arr_time = temp2->arr_time;
        js[index6].exe_time = temp2->exe_time;
        s_exec_times[index6] = temp2->exe_time;
        js[index6].seen = false;
        js[index6].sjob_no = temp2->sjob_no;
        js[index6].accepted = true;
        index6++;
        temp2 = temp2->next;
    }
    int arr_s_exec_times[total_aperiodic_jobs];

    for (int i = 0; i < total_aperiodic_jobs; i++)
    {
        arr_s_exec_times[i] = js[i].exe_time;
    }

    ///////////////////////////////////////////////// sorting aperiodic jobs on the basis of execution time ///////////////////////////////////////////////////////

    sort_with_exe_time(js, 0, total_aperiodic_jobs - 1);

    ////////////////////////////////////////////////// checking if an aperiodic job can get enough time //////////////////////////////////////////////////////////////////

    for (int i = 0; i < total_aperiodic_jobs; i++)
    {
        for (int j = 0; j < total_frames; j++)
        {
            if (js[i].arr_time <= fr_array[j].frame_start_time && js[i].exe_time != 0 && fr_array[j].free_slack_time != 0)
            {
                if (fr_array[j].slack_start_time + js[i].exe_time <= fr_array[j].fr_end_time)
                {
                    fr_array[j].slack_start_time += js[i].exe_time;
                    fr_array[j].free_slack_time = fr_array[j].fr_end_time - fr_array[j].slack_start_time;
                    js[i].exe_time = 0;
                }
                else
                {
                    if (js[i].exe_time == 0)
                        break;
                    js[i].exe_time -= fr_array[j].free_slack_time;
                    fr_array[j].slack_start_time = fr_array[j].slack_start_time + fr_array[j].free_slack_time;
                    fr_array[j].free_slack_time = fr_array[j].fr_end_time - fr_array[j].slack_start_time;
                }
            }
        }
    }

    for (int i = 0; i < total_aperiodic_jobs; i++)
    {
        if (js[i].exe_time != 0)
        {
            js[i].accepted = false;
            fprintf(fptr,"\nA%d is not accepted\n",js[i].sjob_no );
        }
    }

    for (int i = 0; i < total_aperiodic_jobs; i++)
    {
        js[i].exe_time = arr_s_exec_times[i];
    }
    for (int i = 0; i < total_frames; i++)
    {
        fr_array[i].free_slack_time = arr_free_slack_times[i];
        fr_array[i].slack_start_time = arr_start_slack_times[i];
    }

    for (int i = 0; i < total_aperiodic_jobs; i++)
    {

        for (int j = 0; j < total_frames; j++)
        {
            if (js[i].arr_time <= fr_array[j].frame_start_time && js[i].exe_time != 0 && fr_array[j].free_slack_time != 0)
            {
                if (fr_array[j].slack_start_time + js[i].exe_time <= fr_array[j].fr_end_time)
                {
                    if (js[i].accepted == true)
                        fprintf(fptr, "A%d:%d-%d  ", js[i].sjob_no, fr_array[j].slack_start_time, fr_array[j].slack_start_time + js[i].exe_time);
                    js[i].completion_time = fr_array[j].slack_start_time + js[i].exe_time;
                    js[i].response_time = (fr_array[j].slack_start_time + js[i].exe_time) - js[i].arr_time;
                    fr_array[j].slack_start_time += js[i].exe_time;
                    fr_array[j].free_slack_time = fr_array[j].fr_end_time - fr_array[j].slack_start_time;
                    js[i].exe_time = 0;
                }
                else
                {
                    if (js[i].exe_time == 0)
                        break;
                    if (js[i].accepted == true)
                        fprintf(fptr, "A%d:%d-%d  ", js[i].sjob_no, fr_array[j].slack_start_time, fr_array[j].slack_start_time + fr_array[j].free_slack_time);
                    js[i].exe_time -= fr_array[j].free_slack_time;
                    fr_array[j].slack_start_time = fr_array[j].slack_start_time + fr_array[j].free_slack_time;
                    fr_array[j].free_slack_time = fr_array[j].fr_end_time - fr_array[j].slack_start_time;
                }
            }
        }
    }
    fprintf(fptr, "\n");
    fprintf(fptr, "\nThe Response Times of Periodic Jobs are: \n");
    fprintf(fptr, "\n");

    for (int i = 0; i < total_periodic_jobs; i++)
    {
        fprintf(fptr, " j%d%d: %d \n", j1[i].task_num, j1[i].job_num, j1[i].response_time);
    }
    for (int i = 0; i < total_periodic_jobs; i++)
    {
        min_response_time_periodic_job = min(min_response_time_periodic_job, j1[i].response_time);
    }
    for (int i = 0; i < total_periodic_jobs; i++)
    {
        max_response_time_periodic_job = max(max_response_time_periodic_job, j1[i].response_time);
    }
    fprintf(fptr, "\n");
    fprintf(fptr, "Absolute Response Time Jitter of Periodic Taskset: %d", max_response_time_periodic_job - min_response_time_periodic_job);

    fprintf(fptr, "\n");
    fprintf(fptr, "\nThe Response Times of Aperiodic Jobs are: \n");
    fprintf(fptr, "\n");
    for (int i = 0; i < total_aperiodic_jobs; i++)
    {
        fprintf(fptr, " A%d: %d \n", js[i].sjob_no, js[i].response_time);
    }

    for (int i = 0; i < total_aperiodic_jobs; i++)
    {
        if(js[i].accepted == true)
        min_response_time_aperiodic_job = min(min_response_time_aperiodic_job, js[i].response_time);
    }
    for (int i = 0; i < total_aperiodic_jobs; i++)
    { 
         if(js[i].accepted == true)
        max_response_time_aperiodic_job = max(max_response_time_aperiodic_job, js[i].response_time);
    }
    fprintf(fptr, "\n");
    fprintf(fptr, "Absolute Response Time Jitter of Aperiodic Taskset: %d", max_response_time_aperiodic_job - min_response_time_aperiodic_job);

    return 0;
}
