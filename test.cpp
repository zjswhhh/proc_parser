#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
using namespace std;

char* get_kernel_version (char* s) {
    FILE* fp;
    
    fp = fopen("/proc/version", "r");
    fscanf(fp, "Linux version %s (", s);
    fclose(fp);

    return   s; 
}

char* get_processor_type (char* s) {
    FILE* fp;
    char buffer[1024];
    size_t bytes_read;
    char* match;

    fp = fopen("/proc/cpuinfo", "r");
    bytes_read = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    // if(bytes_read == 0 || bytes_read == sizeof(buffer))
    //     return s;
    buffer[bytes_read] = '\0';
    match = strstr(buffer, "model name");
    if (match == NULL)
        return s;
    sscanf(match, "model name      : %[^\t\n] stepping ", s);
    return s;
}

double get_amount_time_since_booted () {
    FILE* fp;
    double amount_time, rest_time;
    
    fp = fopen("/proc/uptime", "r");
    fscanf(fp, "%lf %lf", &amount_time, &rest_time);
    fclose(fp);

    return amount_time;
}

float get_total_mem () {
    FILE* fp;
    float total_mem;
    
    fp = fopen("/proc/meminfo", "r");
    fscanf(fp, "MemTotal:        %f", &total_mem);
    fclose(fp);

    return total_mem;
}

void get_time_percentage (float result[3]) {
    FILE* fp;
    double user_time, nice, sys_time, idle, total;
    double t1, t2, t3, t4;

    fp = fopen("/proc/stat", "r");
    fscanf(fp, "cpu %lf %lf %lf %lf", &user_time, &nice, &sys_time, &idle);
    /* Across All Cores */ 
    while(fscanf(fp, "cpu%d %lf %lf %lf %lf", &t1, &t2, &t3, &t4)) {
        user_time += t1;
        printf("%d ", t1);
        sys_time += t2;
        idle += t4;
    }
    fclose(fp);

    total = user_time + sys_time + idle;

    result[0] = ((float)(user_time/total));
    result[1] = ((float)(sys_time/total));
    result[2] = ((float)(idle/total));
    return;
}

void get_free_mem_amount_and_percentage (float result[2]) {
    FILE* fp;
    float mem_total, mem_free;
    
    fp = fopen("/proc/meminfo", "r");
    fscanf(fp, "MemTotal:       %f kB \nMemFree:        %f kB", &mem_total, &mem_free);
    fclose(fp);

    result[0] = mem_free;
    result[1] = mem_free/mem_total;

    return;
}

int main (int argc, char** argv){
    if(argc == 1) {
    	char s[100];
        printf("Processor type: %s \n", get_processor_type(s));
        printf("Kernel Version: %s \n", get_kernel_version(s));
        printf("The amount of memory configured into this computer: %.1f KB \n", get_total_mem());
	printf("Amount of time since the system was last booted: %.1lf Seconds \n", get_amount_time_since_booted());
    }
    else if(argc == 3) {
        float t[3];
        get_time_percentage(t);
        printf("The percentage of time the processor spend in User Mode, System Mode, and Idle will be: \n");
        for(int i = 0; i < 3; i++)
            printf("%f%%\t", t[i]*100.0);
        printf("\n");

        float m[2];
        get_free_mem_amount_and_percentage(m);
        printf("%.0f KB %f\n", m[0], m[1]*100.0);

    }
    else {
    	printf("Tip: \nPlease run without parameter to call Version 1, or run with <read_rate> <print_out> to call Version 2.\n");
    }
    
    return 0;
}


