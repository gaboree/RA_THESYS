/* 	Algorithm to process Signal Aspect Sequencing, based on a 8 block layout, with one or two train modules present
	The inputs are two char arrays
	vacancy_network_states = FOFFFOFF (incomming vacancy state)
	signal_network_states = RYGRRYGR ( previous signal state, it doesn't matter it is overwritten)
	Gives output new signal aspects
	signal_network_states = YRGRYRGR
 */
#include <bits/stdc++.h> 
using namespace std; 
  
// function to print circular list starting 
// from given index ind. 
void sias(char vac[], char sig[],int a_size, int t){
    int end = 0;
    for ( int i = 0; i < a_size; i++){
        if ( i-1 < 0)
            end = (i-1)%a_size + a_size;
        if(	(vac[i%a_size] == 'O' && vac[(i+1)%a_size] =='F' && vac[(i+2)%a_size] == 'F' && vac[(i+3)%a_size] == 'O'  )||
			(vac[i%a_size] == 'O' && vac[(i+1)%a_size] =='F' && vac[(i+2)%a_size] == 'F' && vac[(i+3)%a_size] == 'F' && vac[(i+4)%a_size] == 'O')){
            sig[i%a_size] = 'R';
            sig[(i+1)%a_size] = 'G';
            if ( i-1 < 0){
                sig[end]='Y';
            }else{
                sig[(i-1)%a_size]='Y';
            }
            //cout<<"OFFO OFFFO"<<"\n";
            //issue SPEED_MAX command
        }
        if(vac[i%a_size] == 'O' && vac[(i+1)%a_size] =='F' && vac[(i+2)%a_size] == 'O' ){
            sig[i%a_size] = 'R';
            sig[(i+1)%a_size] = 'Y';
            sig[(i+2)%a_size] = 'R';
            sig[(i+3)%a_size] = 'G';
            if ( i-1 < 0){
                sig[end]='Y';
            }else{
                sig[(i-1)%a_size]='Y';
            }
            //issue SPEED_MIN command
            //cout<<"OFO"<<"\n";
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
            //cout<<"OO"<<"\n";
        }
        if((t ==1 && vac[i%a_size] == 'O')||
        (vac[i%a_size] == 'O' && vac[(i+1)%a_size] =='F' && vac[(i+2)%a_size] == 'F' && vac[(i+3)%a_size] == 'F' && vac[(i+4)%a_size] == 'F' && vac[(i+5)%a_size] == 'O')){
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
    for (int i = 0; i < a_size; i++){
        cout << sig[i] << " ";
    }
}

void sias_remove_d(char sig[], , int n){
	for (int i = 0; i < n; i++){
		if(sig[i] == 'D')
			sig[i] = 'R';
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
    char vacancy[] = "FFOOFFFF";
    char signal[] =  "DDDDDDDD";
    int array_size = sizeof(signal)-1;
    int train = count_train(vacancy,array_size);
    for (int i = 0; i < array_size; i++){
        cout << vacancy[i] << " ";
    }
    cout<<"\n";
    sias(vacancy, signal, array_size, train);
    sias_remove_d(signal, array_size);
	return 0;
}
