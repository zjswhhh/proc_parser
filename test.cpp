#include <stdio.h>
#include <string.h>
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


int main (){
    char s[100];
    printf("%s\n", get_processor_type(s));
    printf("%f\n", get_amount_time_since_booted());
    printf("%s\n", get_kernel_version(s));
    printf("%f KB\n", get_total_mem());
    return 0;
}
