#include <stdio.h>
#include <string.h>
#include <vector>
using namespace std;

float get_cpu_clock_speed () {
    FILE* fp;
    char buffer[1024];
    size_t bytes_read;
    char* match;
    float clock_speed;

    fp = fopen("/proc/cpuinfo", "r");
    bytes_read = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    if(bytes_read == 0 || bytes_read == sizeof(buffer))
        return 0;
    buffer[bytes_read] = '\0';
    match = strstr(buffer, "cpu MHz");
    if (match == NULL)
        return 0;
    sscanf(match, "cpu MHz		: %f", &clock_speed);
    return clock_speed;  
}

char* get_kernel_version (char* s, int len) {
    FILE* fp;
    char buffer[512];
    size_t bytes_read;
    
    fp = fopen("/proc/version", "r");
    bytes_read = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    if (bytes_read == 0 || bytes_read == sizeof(buffer))
        return NULL;
    buffer[bytes_read] = '\0';
    sscanf(buffer, "Linux version %s (", s);
    s[len-1] = '\0'; 

    return   s; 
}

float get_amount_time_since_booted () {
    FILE* fp;
    float amount_time, rest_time;
    char buffer[128];
    size_t bytes_read;
    
    fp = fopen("/proc/uptime", "r");
    bytes_read = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    if (bytes_read == 0 || bytes_read == sizeof(buffer))
        return 0;
    buffer[bytes_read] = '\0';
    sscanf(buffer, "%f %f", &amount_time, &rest_time);
    return amount_time;
}

int main (){
    printf("%f\n", get_cpu_clock_speed());
    printf("%f\n", get_amount_time_since_booted());
    char s[30];
    printf("%s\n", get_kernel_version(s, 30));
    return 0;
}
