/* 	Algorithm to convert aspect array in to three activation arrays for the shift registers
	The input is a char array
	local_signal = GYRGGYRG
	the outputs are three arrays that activate the corresponding LED pin, on the shift register, for the aspect
	register_1_data = 00101010
	register_2_data = 00010010
	register_3_data = 10100001
	tested on https://rextester.com/l/cpp_online_compiler_gcc
 */
#include <bits/stdc++.h> 
using namespace std; 
int data_size, long_data_size;
char local_signal[] = "GYRGGYRG";
char full_state_list[] = "000000000000000000000000";
char register_1_data[] = "00000000";
char register_2_data[] = "00000000";
char register_3_data[] = "00000000";

// function to print array content
void print_array(char data[], int n){
  for (int i = 0; i < n; i++){
    cout<<data[i];
  }
}

void decode_aspects_to_register(int n) {
  int j = 0;
  for (int i = 0; i < n; i++) {
    switch (local_signal[i]) {
      case 'R':
        full_state_list[j] = 49;
        full_state_list[j + 1] = 48;
        full_state_list[j + 2] = 48;
        break;
      case 'Y':
        full_state_list[j] = 48;
        full_state_list[j + 1] = 49;
        full_state_list[j + 2] = 48;
        break;
      case 'G':
        full_state_list[j] = 48;
        full_state_list[j + 1] = 48;
        full_state_list[j + 2] = 49;
        break;
    }
    j = j + 3;
  }
}

void load_registers_simple(int n) {
    int j=0,k=0;
    for (int i = 0; i < n; i++) {
      if ( i < 8){
          register_1_data[i] = full_state_list[i];
      }

      if ( i > 7 && i < 16){
          register_2_data[j] = full_state_list[i];
          j++;
      }
      if (i > 15 && i < 24){
          register_3_data[k] = full_state_list[i];
          k++;
      }
  }
}


// exmple to demonstrate the algorithm
int main()
{
    data_size = sizeof(local_signal)-1;
    long_data_size = sizeof(full_state_list) -1;
    print_array(local_signal, data_size);
    cout<<"\n";
    print_array(full_state_list, long_data_size);
    cout<<"\n";
    decode_aspects_to_register( data_size);
    cout<<"\n";
    print_array(full_state_list, long_data_size);
    cout<<"\n";
    load_registers_simple(long_data_size);
    cout<<"\n";
    print_array(register_1_data, data_size);
    print_array(register_2_data, data_size);
    print_array(register_3_data, data_size);
    cout<<"\n";
    return 0;
}