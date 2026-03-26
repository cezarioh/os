#include <stdio.h>

#define MAX 20

typedef struct {
    int id, at, bt, rt, pr;
    int ct, wt, tat;
} Process;


float fcfs(Process p[], int n) {
    int time = 0;
    float total_wt = 0;

    for (int i = 0; i < n; i++) {
        if (time < p[i].at)
            time = p[i].at;

        p[i].wt = time - p[i].at;
        time += p[i].bt;
        total_wt += p[i].wt;
    }
    return total_wt / n;
}


float srtf(Process p[], int n) {
    int complete = 0, time = 0, min, shortest;
    float total_wt = 0;
    int visited[MAX] = {0};

    while (complete != n) {
        min = 9999;
        shortest = -1;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rt > 0 && p[i].rt < min) {
                min = p[i].rt;
                shortest = i;
            }
        }

        if (shortest == -1) {
            time++;
            continue;
        }

        p[shortest].rt--;
        time++;

        if (p[shortest].rt == 0) {
            complete++;
            p[shortest].ct = time;
            p[shortest].wt = p[shortest].ct - p[shortest].bt - p[shortest].at;
            total_wt += p[shortest].wt;
        }
    }
    return total_wt / n;
}


float priority_np(Process p[], int n) {
    int completed = 0, time = 0;
    float total_wt = 0;
    int done[MAX] = {0};

    while (completed < n) {
        int idx = -1, max_pr = -1;

        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].at <= time && p[i].pr > max_pr) {
                max_pr = p[i].pr;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        p[idx].wt = time - p[idx].at;
        time += p[idx].bt;
        done[idx] = 1;
        completed++;
        total_wt += p[idx].wt;
    }
    return total_wt / n;
}


float rr(Process p[], int n, int q) {
    int time = 0, completed = 0;
    float total_wt = 0;

    while (completed < n) {
        int done = 1;

        for (int i = 0; i < n; i++) {
            if (p[i].rt > 0 && p[i].at <= time) {
                done = 0;

                if (p[i].rt > q) {
                    time += q;
                    p[i].rt -= q;
                } else {
                    time += p[i].rt;
                    p[i].wt = time - p[i].bt - p[i].at;
                    total_wt += p[i].wt;
                    p[i].rt = 0;
                    completed++;
                }
            }
        }

        if (done)
            time++;
    }

    return total_wt / n;
}

int main() {
    int n;
    Process p[MAX], temp[MAX];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("P%d (AT BT Priority): ", i);
        scanf("%d %d %d", &p[i].at, &p[i].bt, &p[i].pr);
        p[i].id = i;
    }

    
    for (int i = 0; i < n; i++) temp[i] = p[i];
    float fcfs_wt = fcfs(temp, n);


    for (int i = 0; i < n; i++) {
        temp[i] = p[i];
        temp[i].rt = temp[i].bt;
    }
    float srtf_wt = srtf(temp, n);

    
    for (int i = 0; i < n; i++) temp[i] = p[i];
    float pr_wt = priority_np(temp, n);

    
    for (int i = 0; i < n; i++) {
        temp[i] = p[i];
        temp[i].rt = temp[i].bt;
    }
    float rr_wt = rr(temp, n, 3);

    printf("\nAverage Waiting Times:\n");
    printf("FCFS     : %.2f\n", fcfs_wt);
    printf("SRTF     : %.2f\n", srtf_wt);
    printf("Priority : %.2f\n", pr_wt);
    printf("RR       : %.2f\n", rr_wt);

    
    float min = fcfs_wt;
    char *best = "FCFS";

    if (srtf_wt < min) { min = srtf_wt; best = "SRTF"; }
    if (pr_wt < min)   { min = pr_wt; best = "Priority"; }
    if (rr_wt < min)   { min = rr_wt; best = "RR"; }

    printf("\nBest Algorithm: %s (Minimum Avg Waiting Time)\n", best);

    return 0;
}
