#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
using namespace std;

char* get_processor_type (char* s) {
    FILE* fp;
    char buffer[1024*10];
    size_t bytes_read;
    char* match;

    fp = fopen("/proc/cpuinfo", "r");
    bytes_read = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    if(bytes_read == 0 || bytes_read == sizeof(buffer))
        return s;
    buffer[bytes_read] = '\0';
    match = strstr(buffer, "model name");
    if (match == NULL)
        return s;
    sscanf(match, "model name	: %[^\t\n]Stepping ", s);
    return s;
}

char* get_kernel_version (char* s) {
    FILE* fp;
    
    fp = fopen("/proc/version", "r");
    fscanf(fp, "Linux version %s (", s);
    fclose(fp);

    return   s; 
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
    float user_time, nice, sys_time, idle, total;

    fp = fopen("/proc/stat", "r");
    fscanf(fp, "cpu %f %f %f %f", &user_time, &nice, &sys_time, &idle);
    fclose(fp);

    total = user_time + sys_time + idle;

    result[0] = ((float)(user_time/total));
    result[1] = ((float)(sys_time/total));
    result[2] = ((float)(idle/total));

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
        for(int i = 0; i < 3; i++)
            printf("%f ", t[i]);
        printf("\n");
    }
    else {
    	printf("Tip: \nPlease run without parameter to call Version 1, or run with <read_rate> <print_out> to call Version 2.\n");
    }
    
    return 0;
}
