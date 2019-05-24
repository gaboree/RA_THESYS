/* 	Algorithm to process Signal Aspect Sequencing, based on a 8 block layout, with one or two train modules present
	The input is two char arrays
	signal_network_states = FOFFFOFF
	vacancy_network_states = RYGDRYGD 
 */
#include <bits/stdc++.h> 
using namespace std; 
  
// function to print circular list starting 
// from given index ind. 
void sias(char vac[], char sig[],int a_size, int t){
    int end = 0;
    for ( int i = 0; i < a_size; i++){
        
        if ( i-1 < 0)
            end = (i-1)%a_size + n;
        
        if(	(vac[i%a_size] == 'O' && vac[(i+1)%a_size] =='F' && vac[(i+2)%a_size] == 'F' && vac[(i+3)%a_size] == 'O'  )||
			(vac[i%a_size] == 'O' && vac[(i+1)%a_size] =='F' && vac[(i+2)%a_size] == 'F' && vac[(i+3)%a_size] == 'F' && vac[(i+3)%a_size] == 'O')){
				sig[i%a_size] = 'R';
				sig[(i+1)%a_size] = 'G';
				if ( i-1 < 0){
					sig[end]='Y';
				}else{
					sig[(i-1)%a_size]='Y';
				}
				//issue SPEED_MAX command
        }
        
        if(vac[i%a_size] == 'O' && vac[(i+1)%a_size] =='F' && vac[(i+2)%a_size] == 'O' ){
            sigb[i%a_size] = 'R';
            sig[(i+1)%a_size] = 'Y';
            sig[(i+2)%a_size] = 'R';
            sig[(i+3)%a_size] = 'G';
            if ( i-1 < 0){
                sig[end]='Y';
            }else{
                sig[(i-1)%a_size]='Y';
            }
            //issue SPEED_MIN command
        }
        
        if(vac[i%a_size] == 'O' && vac[(i+1)%a_size] =='O' ){
            sig[i%a_size] = 'R';
            sig[(i+1)%a_size] = 'R';
            sig[(i+2)%a_size] = 'G';
            if ( i-1 < 0){
                sig[end]='Y';
            }else{
                sig[(i-1)%a_size]='Y';
            }
            //issue STOP command
        }
        
        if(t == 1 && vac[i%a_size] == 'O'){
            sig[i%a_size] = 'R';
            sig[(i+1)%a_size] = 'G';
            if ( i-1 < 0){
                sig[end]='Y';
            }else{
                sig[(i-1)%a_size]='Y';
            }
			// issue SPEED_MAX command
        }

    }    
    
    for (int i = 0; i < n; i++){
        cout << sig[i] << " ";
    }
}

int count_train(char vac[], int vac_size){
    int val = 0;
    for( int i = 0; i < vac_size; i++){
        if(vac[i] == 'O')
            val++;
    }
    return val;
}

// exmple to demonstrate the algorithm
int main() { 
    char vacancy[] = { 'F', 'F', 'F', 'F', 'F', 'F', 'F', 'F' };
    char signal[] = { 'D', 'D', 'D', 'D', 'D', 'D', 'D' , 'D' };
    int array_size = sizeof(signal) / sizeof(signal[0]);
    int train = count_train(vacancy,array_size);
    cout<< train << "\n";
    cout << n << "\n";
    for (int i = 0; i < n; i++){
        cout << signal[i] << " ";
    }
    cout << train << "\n";
    sias(vacancy, signal, array_size, train);
    return 0;
}
