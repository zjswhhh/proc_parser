#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
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
    char str[100];
    long int user_time = 0, sys_time = 0, idle = 0, total = 0;
    const char s[2] = " ";
    char *token;

    fp = fopen("/proc/stat", "r");

    /* Across All Cores */ 
    /* Filed 2, 4, 5 */ 
    while(fgets(str, 100, fp)!= NULL){
        // puts(str);

        if(str[0] == 'i')   /* Stop when come across "intr" */
            break;

        token = strtok(str, s);
        for(int i=0; i<5; i++) {
            if(i==1){
                user_time += atoi(token);
                // printf( " %s\n", token );
            }
            if(i==3) {
                sys_time += atoi(token);
                // printf( " %s\n", token );
            }
            if(i==4){
                idle += atoi(token);
                // printf( " %s\n", token );
            }
            token = strtok(NULL, s);
        }
    }

    fclose(fp);

    total = user_time + sys_time + idle;

    result[0] = (float)user_time / (float)total * 100.0;
    result[1] = (float)sys_time/ (float)total * 100.0;
    result[2] = (float)idle/ (float)total * 100.0;
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

void get_rate_of_disk (double result[2]) {
    FILE* fp;
    char str[100];
    const char s[2] = " ";
    char *token;
    long int r1 = 0, r2 = 0, w1 = 0, w2 = 0;
    
    fp = fopen("/proc/diskstats", "r");

    /* Acrosss all disks */
    /* Filed 6, 7, 10, 11*/ 
    while(fgets(str, 100, fp)!= NULL) {
        // puts(str);
        token = strtok(str, s);
        for (int i=0; i<11; i++){
            if(i==5){
                r1 += atoi(token);
                // printf( " %s\n", token );
            }
            if(i==6){
                r2 += atoi(token);
                // printf( " %s\n", token );
            }
            if(i==9){
                w1 += atoi(token);
                // printf( " %s\n", token );
            }
            if(i==10){
                w2 += atoi(token);
                // printf( " %s\n", token );
            }
            token = strtok(NULL, s);
        }
    }

    fclose;
    // printf("%d %d %d %d", r1, r2, w1, w2);

    if(r1!=0 && r2!=0) 
        result[0] = (double)r1 / (double)r2 * 1000.0; /* number per second*/
    if(w1!=0 && w2!=0)
        result[1] = (double)w1 / (double)w2 * 1000.0;

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
        int timestamp = 0;
        float t[3];
        int time1 = atoi(argv[1]);
        int time2 = atoi(argv[2]);
        int time3 = time2 / time1;

        while(1){
            timestamp += time1;

            /* %CPU */
            float tmp[3];
            get_time_percentage(tmp);
            for(int i=0; i<3; i++) 
                t[i] += tmp[i];

            /* */

            /* */

            /* */

            /* */

            /* Print */
            if(timestamp == time2){
                printf("%%Cpu(s): %f %%, %f %%, %f %%\n");
            }

            usleep(atoi(argv[1])*1000);
        }
        

        float m[2];
        get_free_mem_amount_and_percentage(m);
        printf("%.0f KB %f\n", m[0], m[1]*100.0);

        double rw[2];
        get_rate_of_disk(rw);
        printf("%lf %lf\n", rw[0], rw[1]);

    }
    else {
    	printf("Tip: \nPlease run without parameter to call Version 1, or run with <read_rate> <print_out> to call Version 2.\n");
    }
    
    return 0;
}


