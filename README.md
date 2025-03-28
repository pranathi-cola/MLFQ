# MLFQ
# Question in OS Course, CS2012E taken by Professor Sumesh T.A and Professor Vasudevan A.R

In this assignment, you will implement an MLFQ scheduling algorithm for a problem mentioned below.

An operating system implements multilevel feedback queue scheduling with four queues. The top-level queue (highest priority) (Q0) employs round-robin scheduling. The second-level queue (Q1) first-come, first-served scheduling. The third-level queue (Q2) uses priority scheduling, where priority is taken as the inverse of the time needed by the jobs present in this queue. The last level queue (Q3) employs runs the shortest job first scheduling. Each job is allotted a maximum of 5ms in each queue. That means each job in any queue runs for 5ms on the CPU regardless of the algorithm’s nature and then each job is moved to the next lower level queue. If the job are not finished after the completion of execution of all jobs in the last queue, then all remaining jobs are put back to the topmost queue (Q0) again. The scheduling in a particular queue happens only after all the processes arrive in each queue. The process will repeat until all the jobs are completed. Calculate the average turnaround time taken for the sample workload given below. (Assumption: all the algorithms are assumed to be preemptive.)

The CPU execution of each job may be simulated as a a reduction of time required by the amount of allocation from the total time required.

 _____________________________________
|Process  |   Total CPU time needed   |
|---------|---------------------------|
|A        |   300                     |
|B        |   150                     |
|C        |   250                     |
|D        |   350                     |
|E        |   450                     |
|_________|___________________________|

You 'may' implement the solution in C using multithreading. You should have option to take CPU time needed for each job as the user input. The output should also display the individual job’s total turnaround time. The workload and allocation time should be chosen wisely. 


# Solution Process

1. Take an input of the number of the processes(n) and each of their CPU time needed.
2. Next, create n threads each opening a function called fun.
3. There exists a global variable called state which shows the current queue the process is in.
4. If a process is in Q0, then the process runs it for 1 sec and then increments the local variable of the function robin which keeps track of the 5 sec mark, once 5 secs are met the state of the process changes to Q1, adds it a queue q1, updates the corresponding place of q2 based on maintining increasing order of remaining time and sets robin to 0.
5. If a process is in Q1, it checks whether any process is in Q0. If so, it leaves and waits for its turn again else, it checks if it is the first element in the queue, q1. if it isn't, it leaves else it runs, updates q2 and changes its state to Q2.
6. If a process is in Q2, it checks whether any process is in Q2. If so it leaves else it checks if its first in queue, q2. If it isnt, it leaves, else it runs and changes its state to Q3.
7. If a process is in Q3, it checks if any process is in Q3. If so it leaves else it runs and changes its state to Q0.
8. The loop breaks once the cpu time required is met.
9. Q2, Q3 are initialised to be the indices of processes in increasing order of the cpu times required.
10. Here, runs means the corresponding queue would dequeue and enqueue if the cpu time required isnt met yet.