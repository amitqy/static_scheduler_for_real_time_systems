
---------------------------------------------------------------------------------------------------------

How to Compile

gcc main.c func_def.c -o sched

-----------------------------------------------------------------------------------------------------------------

How to execute

./ sched input.txt

-----------------------------------------------------------------------------------------------------------

Details of the files

main.c : main file only containing the main function

func_def.c : This c file contains all the functions definition

sched.h : This contains all the header files used and all the structures declaration

input.txt: contains the  periodic tasks


---------------------------------------------------------------------------------------------------------

input.txt : This file contains periodic tasks in the format 'a  b  c'

a,b,c are integers

a : period of the task

b : worst case execution of a job in the task

c : relative deadline of the  task


if the task is of the format 'a  b' then c has to supplied zero value i.e 'a b 0'

-------------------------------------------------------------------------------------------------------------------------

aperiodicJobs.txt : This text file contains aperiodic jobs of the format 'a b'

a: arrival time of the jon

b: worst case execution time of the jon


----------------------------------------------------------------------------------------------------------

First input from the input.txt file is read and its execution output is printed in the file 'periodicSchedule.txt'

Then in the subsequent part of the program, the aperiodic jobs are taken from the file aperiodicJobs.txt
This file is not supplied in command line.

The final output is printed in periodicSchedule.txt file itself.

----------------------------------------------------------------------------------------------------------

THINGS NOT DONE

a.This program does schedule the sporadic jobs and hence does not read the sporadic jobs from a file.
b.This program does not calculate relative response time jitter of the task
c.This program does not split the task in case frame size is not found the theorem


BUGS IN THE PROGRAM

If a aperiodic job is not scheduled,then its corresponding calculations print garbage values
Each paramter like period,execution time,deadline only take integer input. So it may behave wrongly with non-integer values


------------------------------------------------------------------------------------------------------



