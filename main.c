#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>
#include "rtclock.h"
#include "mmm.h"

// just helps error check commands to be numbers
int helper(char cmnd[]){	
	for(int i =0; cmnd[i]!=0;i++){
		if(!isdigit(cmnd[i])){
			return 0;
		}
	}
	return 1;
}

int main(int argc, char *argv[])
{
	int check;
	int num;
	if(argc>4||argc<3){
		printf("Usage: ./mmm <mode> [num threads] <size>\n");
		return 0;
	}

	if(strcmp("S",argv[1])==0){
		int cor =helper(argv[2]);
		if(cor==0){
			printf("Usage: ./mmm <mode> [num threads] <size>\n");
			return 0;
		}
		check =0;
		size = atoi(argv[2]);
		printf("======\n");
		printf("mode: sequential\n");
		printf("thread count: %d\n",1);
		printf("size: %d\n",size);
		printf("======\n");

	}
	if(strcmp("P",argv[1])==0){
		int cor =helper(argv[2]);
		if(cor==0){
			printf("Usage: ./mmm <mode> [num threads] <size>\n");
			return 0;
		}
		cor =helper(argv[3]);
		if(cor==0){
			printf("Usage: ./mmm <mode> [num threads] <size>\n");
			return 0;
		}
		check =1;
		size = atoi(argv[2]);
		num = atoi(argv[3]);
		printf("======\n");
		printf("mode: parallel\n");
		printf("thread count: %d\n",num);
		printf("size: %d\n",size);
		printf("======\n");

	}
	if(check!=1&&check!=0){
		printf("Usage: ./mmm <mode> [num threads] <size>\n");
		return 0;
	}

	double ver,clockstart, clockend, seqT, parT;
	
	mmm_init();
	//printMatrix(A);
	//printf("\n");
	//printMatrix(B);
	//printf("\n");

	clockstart = rtclock(); // start clocking
	mmm_seq();
	clockend = rtclock(); // stop clocking
	seqT=(clockend - clockstart);

	if(check==1){
		for(int m=0;m<4;m++){// four runs throwing out the first
			clockstart = rtclock(); // start clocking
			int i;
			pthread_t threads[num];
			int div = (size/num)-1;
			struct parParams args[num];
			for(i =0;i<num-1;i++){
				args[i].s_row=i*div;
				args[i].f_row=((i+1)*div)-1;
				pthread_create(&threads[i],NULL,mmm_par,&args[i]);
			}
			args[i].s_row=i*div;
			args[i].f_row=size-1;
			pthread_create(&threads[i],NULL,mmm_par,&args[i]);
			for (i = 0; i < num; i++){
    			pthread_join(threads[i], NULL);
  			}
			clockend = rtclock(); // stop clocking
			if(m!=0){
				ver += mmm_verify();
				parT+=(clockend - clockstart);
			}
			mmm_reset(D);
		}
		parT=parT/3;
		printf("Sequential Time: %.6f sec\n", seqT);
		printf("Parallel Time: %.6f sec\n", parT);
		printf("Speedup: %.6f \n",(seqT/parT));
		printf("Verifying...largest error between parallel and sequential matrix: %f\n",ver);
		mmm_freeup();
		return 0;
	}
	printf("Sequential Time: %.6f sec\n", seqT);
	mmm_freeup();

	return 0;
}
