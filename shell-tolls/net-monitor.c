#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>



int main(int argc, char* argv[]){

    struct timespec ts;
    /*
    clock_gettime(CLOCK_REALTIME, &start_t);
    long int start_ts_ns=(long int)start_t.tv_sec*1e9 + start_t.tv_nsec;
    printf("程序启动， %ld  %ld\n", (long int)start_t.tv_sec, start_t.tv_nsec);
    printf("程序启动 ts  %ld\n", start_ts_ns);
    */
    
    

    if(argc<4){
        printf("Usage: ./out  ifname  min_interval  measure_range  clean \n  " );
                // ifname is network interface name
                // t1 是测网速的最小时间间隔，单位是ms； \n     \
                // t2 是总测量时间长度，单位是ms;  \n  \
                // 'clean' means whether make sure the first time is 0"

        return 0;
    }
    //printf("fak");
    //printf("%s  %s \n",argv[1], argv[2]);
    const char* ifname=argv[1]; //ms
    int measure_interval=atoi(argv[2]); //ms
    int measure_time_range=atoi(argv[3]); //ms
    int loop_num=(int)(measure_time_range/measure_interval);
    //printf("loop_num: %d \n",loop_num);
    long int TIME[loop_num];
    double TX_BW[loop_num];
    double RX_BW[loop_num];
    int i = 0;
    char bytes[201];
    
    //init time and bytes
    clock_gettime(CLOCK_REALTIME, &ts);
    long int last_ts_ns=(long int)ts.tv_sec*1e9 + ts.tv_nsec;
    long int last_tx_bytes=0;
    long int last_rx_bytes=0;

    for(i = 0 ; i < loop_num ; i++){

        usleep(measure_interval*1000);  //us

        //measure time
        clock_gettime(CLOCK_REALTIME, &ts);

        //get bytes
        char  file_path[200]= "/sys/class/net/";
        strcat(file_path, ifname);
        strcat(file_path, "/statistics/tx_bytes");
        //printf("%s  \n",file_path);

        int fp = open(file_path,O_RDONLY);
        read(fp,bytes,200);
        bytes[201] = '\0';
        long int tx_bytes = strtol(bytes,NULL,10);



        char file_path_2[200] = "/sys/class/net/";
        strcat(file_path_2, ifname);
        strcat(file_path_2, "/statistics/rx_bytes");
        fp = open(file_path_2,O_RDONLY);
        read(fp,bytes,200);
        bytes[201] = '\0';
        long int rx_bytes = strtol(bytes,NULL,10);



        //calculate bandwidth;
        long int ts_ns=(long int)ts.tv_sec*1e9 + ts.tv_nsec;
        long int delta_t_ns = ts_ns - last_ts_ns;
        last_ts_ns = ts_ns;

        //tx_bw
        long int delta_tx_bytes = tx_bytes - last_tx_bytes;
        last_tx_bytes=tx_bytes;
        double tx_bw=(double)delta_tx_bytes*8 / delta_t_ns;  //Gbps
        if(i==0) tx_bw=0;


        //rx_bw
        long int delta_rx_bytes = rx_bytes - last_rx_bytes;
        last_rx_bytes=rx_bytes;
        double rx_bw=(double)delta_rx_bytes*8 / delta_t_ns;  //Gbps
        if(i==0) rx_bw=0;


        //record
        TIME[i] = ts_ns;
        TX_BW[i] = tx_bw;
        RX_BW[i] = rx_bw;

      
    }

    //process TIME
    int clean = 0;
    if(argc>=4) clean=atoi(argv[4]);
    if(clean!=0) {
        long int tmp_ts = TIME[0];
        for (i = 0; i < sizeof(TIME) / sizeof(TIME[0]); i++) {
            TIME[i] = TIME[i] - tmp_ts;
        }
    }
    
    //store value
    FILE *fp = fopen("output.txt", "w+");
    int total_num =sizeof(TIME)/sizeof(TIME[0]);
    for(i = 0; i<total_num ;i++){
        fprintf(fp, "%ld", TIME[i]);   //ns
        fprintf(fp, "    ");
        fprintf(fp, "%f", TX_BW[i]);   //Gbps
        fprintf(fp, "    ");
        fprintf(fp, "%f", RX_BW[i]);   //Gbps
        fprintf(fp, "\n");
    }
    fclose(fp);
    
    
    
    
    /*
    clock_gettime(CLOCK_REALTIME, &end_t);
    long int end_ts_ns=(long int)end_t.tv_sec*1e9 + end_t.tv_nsec;
    printf("程序启动， %ld  %ld\n", (long int)end_t.tv_sec, end_t.tv_nsec);
    printf("程序启动 ts  %ld\n", end_ts_ns);

    printf("总时间： %ld\n", end_ts_ns-start_ts_ns); //ns
    
    */
    return 0;
}


