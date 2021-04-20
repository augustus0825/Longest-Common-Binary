#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
#include <chrono>

using namespace std;

 
// An unsigned char can store 1 Bytes (8bits) of data (0-255)
typedef unsigned char BYTE;
struct Record *longestBYTE(BYTE *a, BYTE *b, size_t A, size_t B);

void addRecord(struct Record *, struct Record *);

struct Record {
    int offset1;
    int offset2;
    int corr_offset1_start[60000] = {0};
    int corr_offset1_end[60000] = {0};
    int corr_offset2_start[60000] = {0};
    int corr_offset2_end[60000] = {0};
    int corr_length[60000] = {0};
    int length;
    int counter = 0;
};


int main(int argc, char** argv)
{
    int no_file;

    no_file = stoi(argv[1]);

    string file_name[no_file];

    if ( argc != (no_file + 2)){
        cout << "Error: Invalid Input" << endl;
        cout << " Example format of input: [2] [sample.1] [sample.2]" << endl;
        return 0;
    }
    for( int i = 2; i < argc; i++){
    string current_exec_name = argv[i];
     file_name[i-2] = current_exec_name;
    }
    
    BYTE *fileBuf[no_file];  // Pointer to our buffered data
    FILE *file = NULL;      // File pointer
    BYTE *newBuf[60000];
    ifstream fin[no_file];
    size_t size[no_file];
    struct Record *records[no_file][no_file];
    
    int length = 0;
    int longest = 0;
    int result[10];
    int temp_offs;
    int temp_offs2;

    for (int i = 1; i < no_file; ++i) {
        records[0][i] = (Record *) malloc(sizeof(Record));
        memset(records[0][i], 0, sizeof(Record));
    }

    
    // Open the file in binary mode
    // This also checks if the file exists and/or can be opened for reading correctly
    auto start = chrono::steady_clock::now(); //Start timer
    for (int i = 0; i < no_file; i++) {
        fin[i].open(file_name[i].c_str(), ios::binary | ios::in);

    if (!fin[i].is_open())
        cout << "Could not open specified file" << endl;
    else
        cout << file_name[i] << " opened successfully" << endl;       
    }
    cout << "There is " << no_file << " number of sample" << endl;

    // Get the size of the file in bytes
    for (int x = 0; x < no_file; x++){
    fin[x].seekg(0, ios::end);
    size[x] = fin[x].tellg();
    cout << "Size of the " << file_name[x] << " = " << size[x] << endl;
    fin[x].seekg(0, ios::beg);
    }

    
    // Allocate space in the buffer for each of the file
    for (int i = 0; i < no_file; i++){
    fileBuf[i] = new BYTE[size[i]];
    fin[i].read((char*) fileBuf[i], size[i]);   
    }


    auto end = chrono::steady_clock::now(); //end timer
    chrono::duration<double> elapsed_seconds = end-start;
    cout << "Elapsed time for reading into buffer: " << elapsed_seconds.count() << "s" << endl;
 

    start = chrono::steady_clock::now(); //start timer

    //Testing buffer
    // fileBuf[0][0] = 'A';
    // fileBuf[0][1] = 'A';
    // fileBuf[0][2] = 'C';
    // fileBuf[0][3] = 'B';

    // fileBuf[2][0] = 'B';
    // fileBuf[2][1] = 'B';
    // fileBuf[2][2] = 'A';
    // fileBuf[2][3] = 'A';

    // fileBuf[1][0] = 'C';
    // fileBuf[1][1] = 'B';
    // fileBuf[1][2] = 'B';
    // fileBuf[1][3] = 'C';
    // fileBuf[1][4] = 'A';
    // fileBuf[1][5] = 'A';

    // fileBuf[3][0] = 'B';
    // fileBuf[3][1] = 'B';
    // fileBuf[3][2] = 'A';
    // fileBuf[3][3] = 'A';

    // size[0] = 4;
    // size[1] = 6;
    // size[2] = 4;
    // size[3] = 4;

if (no_file == 2) {
    records[0][1] = longestBYTE(fileBuf[0], fileBuf[1], size[0], size[1]);
    cout  << endl;
    cout << "****** Results ******" << endl;
    for (int i = 0; i< no_file; i++) {
        cout << " " << endl;
        cout << "Report of " << file_name[i] << " " << endl;
        cout << "Length of longest identical strand = " << records[0][1]->length << endl;
        if(file_name[i] == "sample.1"){
            cout << "Offset of " << file_name[i] << " = " << records[0][1]->offset1 << endl;
        }
        else {
            cout << "Offset of " << file_name[i] << " = " << records[0][1]->offset2 << endl;
        }
    }
}
else if (no_file==1) {
    cout << "Please select one extra file for comparision" << endl;
}
else {
   
    records[0][1] = longestBYTE(fileBuf[0], fileBuf[1], size[0], size[1]); // Call function to look for longest byte
    
    for ( int i = 2; i < no_file; i++){
        length = 0;
        cout << "Comparing " << file_name[i] << " please wait......" << endl;
        for (int j = records[0][i-1]->counter-1; j >= 0; j--) {     
            Record *tmp_record= longestBYTE(fileBuf[0] + records[0][i-1]->corr_offset1_start[j], fileBuf[i], records[0][i-1]->corr_length[j], size[i]);       
            addRecord(records[0][i], tmp_record);
        }   
    }
    for (int i  = 0; i < no_file; i++) {
        delete[] fileBuf[i];
    }
    for ( int i = 0; i < records[0][no_file-1]->counter; i++) {       
        if ( records[0][no_file-1]->corr_length[i] > longest) {
            longest = records[0][no_file-1]->corr_length[i];
            temp_offs = records[0][no_file-1]->corr_offset2_start[i];
        }
    }
   for (int y = 2; y < no_file+2; y++) {
        for ( int i = 0; i < records[0][y-1]->counter; i++) {       
            if ( records[0][y-1]->corr_length[i] == longest) {
                longest = records[0][y-1]->corr_length[i];
                temp_offs = records[0][y-1]->corr_offset1_start[i];
                temp_offs2 = records[0][y-1]->corr_offset2_start[i];
            }
            records[0][y-1]->length = longest;
            records[0][y-1]->offset1 = temp_offs;
            records[0][y-1]->offset2 = temp_offs2;
        }
    }

    for (int i = 0; i < no_file; i++) {
    fin[i].close();
    }

    cout  << endl;
    cout << "****** Results ******" << endl;
    
    for (int i = 2; i < no_file+2; i++){
        cout << " " << endl;
        cout << "Report of " << file_name[i-2] << " " << endl;
    
        if(file_name[i-2] == file_name[0]){
            cout << "Length of longest identical strand = " << records[0][1]->length << endl;
            cout << "Offset of " << file_name[i-2] << " = " << records[0][1]->offset1 << endl;
        }
        else if(file_name[i-2] == file_name[1]) {
            cout << "Length of longest identical strand = " << records[0][1]->length << endl;
            cout << "Offset of " << file_name[i-2] << " = " << records[0][1]->offset2 << endl;
            
        }
        else {
            cout << "Length of longest identical strand = " << records[0][i-2]->length << endl;
            cout << "Offset of " << file_name[i-2] << " = " << records[0][i-2]->offset2 << endl;             
        }
    }
    
    
    end = chrono::steady_clock::now(); // end timer
    chrono::duration<double> seconds = end-start;
    cout << "Elapsed time for finding the longest strand among " << no_file << " sample is: " << seconds.count() << "s" << endl;
}
    return 0;
}

//function to look for longest strand of byte between 2 file
Record *longestBYTE(BYTE *a, BYTE *b, size_t A, size_t B){
    struct Record *offset = new Record();

    offset->length = 0;
    int temp_longest = 5;
    if ( A == 0 || B == 0){
        return offset;
    }
       
    int cache[2][B];
    int currRow = 0;

    for (int i = 0; i < A; i++) {    
        for (int j = 0; j < B; j++) {
            
            
            if (a[i] == b[j] ){
                if ( i == 0 || j == 0) {
                cache[currRow][j] = 1;
                }
                else {                   
                cache[currRow][j] = cache[1 - currRow][j-1] +1;      
                }
            }
            else {
                cache[currRow][j] = 0;                
            }
            if ( j == B - 1 || i == A - 1){
                if (cache[currRow][j] > 1) {
                    int start_offset_1 = i - cache[currRow][j] + 1;
                    int start_offset_2 = j - cache[currRow][j] + 1;
                         
                    offset->corr_offset1_start[offset->counter] = start_offset_1; //copy sample 1 start
                    offset->corr_offset1_end[offset->counter] = i;                //^^ end
                    offset->corr_offset2_start[offset->counter] = start_offset_2; //copy sample 2 start
                    offset->corr_offset2_end[offset->counter] = j;                //^^ end
                    offset->corr_length[offset->counter] = cache[currRow][j];
                    offset->counter++;    
                }
            }
            else {

            if(cache[currRow][j] == 0 && j!= 0 && i != 0 ){
                if(cache[1-currRow][j-1] > 1){
                    int start_offset_1 = i-1 - cache[1-currRow][j-1] + 1;
                    int start_offset_2 = j-1 - cache[1-currRow][j-1] + 1;
                         
                    offset->corr_offset1_start[offset->counter] = start_offset_1; //copy sample 1 start
                    offset->corr_offset1_end[offset->counter] = i-1;                //^^ end
                    offset->corr_offset2_start[offset->counter] = start_offset_2; //copy sample 2 start
                    offset->corr_offset2_end[offset->counter] = j-1;                //^^ end
                    offset->corr_length[offset->counter] = cache[1-currRow][j-1];
                    offset->counter++;    
                }
            }
            }
            
            if (cache[currRow][j] > offset->length ) {             
                offset->length = cache[currRow][j];
                offset->offset1 = i - offset->length + 1;
                offset->offset2 = j - offset->length + 1;
            }
        }
        currRow = 1 - currRow;      
    } 
    return offset;
}

void addRecord(struct Record *dest, struct Record *source){
    for (int i = 0; i  < source->counter; i++ ){
        dest->corr_offset2_start[(dest->counter)+i] = source->corr_offset2_start[i];
        dest->corr_length[(dest->counter)+i] = source->corr_length[i];
    }
    dest->counter += source->counter;
}



              