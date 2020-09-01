struct Task
{
    int period;
    int exe_time;
    int rel_deadline;
    struct Task *next;
};
struct Frame
{
    int slack_start_time;
    int frame_length;
    int frame_start_time;
    int fr_end_time;
    bool seen_frame;
    int free_slack_time;
    int slack_end_time;

};
struct job
{
    int abs_deadline;
    int arr_time;
    int task_num;
    int job_num;
    int exec_time;
    int completion_time;
    bool flag;
    int frame_no;
    int response_time;
    int start_time;
};
struct aperiodic_job
{
    int arr_time;
    int exe_time;
    int completion_time;
    int response_time;
    bool seen;
    int sjob_no;
    bool accepted;
    struct aperiodic_job *next;
};

int max(int a, int b);
int gcd(int a, int b);
int find_hyperperiod(int a, int b);
void swap(struct job *a, struct job *b);
void swap2(struct aperiodic_job *a, struct aperiodic_job *b);
int partition(struct job j1[], int low, int high);
void sort_with_deadlines(struct job j1[], int low, int high);
int partition2(struct aperiodic_job js[], int low, int high);
void sort_with_exe_time(struct aperiodic_job js[], int low, int high);
int min(int a,int b);