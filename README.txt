Name: Ng Zheng Yao (Augustus)


Problem Description (Longest Strand)

The longest strand problem is to find the longest strand of bytes that is identical between 2 or more file. When given a large number of binary files.

The program LCB will display:
- The length of the strand
- The filenames where the strand appear and their offset

To solve this problem i have use dynamic programming to find the longest common bytes and the offset of all the common bytes among 2 files.
To save memory I have only collect the offset of those bytes so it does affect the performace of the program.
To further minimize the use of memory and increase the performance i have created 2 by (size of the sample) array when it is looking for common bytes using dynamic programming.
After getting the result i would use the result to compare with the next sample. This process will repeat itself until it reaches the last sample being provided by the user.
As the number of files increases the time taken for the program to produce the result will be longer.
Please do expect atleast 90second when there are more than 5 files.

Instruction:
Makefile have been provided. The program can be compile by just typing make in the terminal
If it is not working please do enter (g++ -std=c++11 -o LCB LCBinary.cpp)
After compiling (./LCB (No of sample) (sample.1) (sample.2)....)
Example: (./LCB 3 sample.1 sample.2 sample.3)

Sample 1    fileBuf[0][0] = 'A';        Report of sample.1 
            fileBuf[0][1] = 'A';        Length of longest strand of bytes: 2
            fileBuf[0][2] = 'C';        Offset of sample.1: 0
            fileBuf[0][3] = 'B';

Sample.2    fileBuf[1][0] = 'B';        Report of sample.2
            fileBuf[1][1] = 'B';        Length of longest strand of bytes: 2
            fileBuf[1][2] = 'A';        Offset of sample.2: 2
            fileBuf[1][3] = 'A';

Sample.3    fileBuf[2][0] = 'C';        Report of sample.3
            fileBuf[2][1] = 'B';        Length of longest strand of bytes: 2
            fileBuf[2][2] = 'B';        Offset of sample.3: 4
            fileBuf[2][3] = 'B';
            fileBuf[2][4] = 'A';
            fileBuf[2][5] = 'A';