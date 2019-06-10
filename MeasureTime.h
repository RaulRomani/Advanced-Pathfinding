#ifndef MEASURETIME_H_INCLUDED
#define MEASURETIME_H_INCLUDED


#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <sys/time.h>

using namespace std;


class MeasureTime{
	struct timeval tvalBefore, tvalAfter;  // removed comma
public:

	void record(){
		gettimeofday(&tvalBefore, NULL);
	}

	void stopAndPrint(const char* msg){
    gettimeofday (&tvalAfter, NULL);
    printf("Elapsed Time  on %s: %ld microseconds\n",msg,
            ((tvalAfter.tv_sec - tvalBefore.tv_sec)*1000000L + tvalAfter.tv_usec)
            - tvalBefore.tv_usec);
	}

	void test()
  {

    MeasureTime tiempo1;
    tiempo1.record();
    int sum=0;
    for(int i=0; i<10000000; i++)
      sum++;
    tiempo1.stopAndPrint("test 1");

    tiempo1.record();
    sum=0;
    for(int i=0; i<10000000; i++)
      sum++;
    tiempo1.stopAndPrint("test 2");
  }
};






#endif // MEASURETIME_H_INCLUDED
