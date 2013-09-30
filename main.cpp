/******************* main.cpp *******************/

#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include <string>
#include "golay.h"
#include <ctime>

using namespace std;

int main()
{   
    matrix gen,gen_t,par,par_t,encoded_message,syndrome,
           sB,s_add_colB,sB_add_colB_T,original_message;
    matrix message(1,12);
    matrix temp1(1,12);
    matrix noise(1,24);
    matrix error(1,24);
    matrix s_add_colB_full(12,13);
    matrix sB_add_colB_T_full(12,13);
    matrix temp(12,1);
   
    int i,j,k;
    vector<int> zeros(24);

    clock_t start = clock();

    gen = golay_generator();   //generating matrix for golay 24 code
    par = generate_B();        //parity matrix for golay 24 code

    cout << "generating matrix for golay 24 code (G): " << endl;
    print(gen);
   
    gen_t = transpose(gen); //generating matrix, G, transposed

    cout << endl << "generating matrix transposed (G^T): " << endl;
    print(gen_t);

    cout << endl << "parity matrix for golay 24 code (B): " << endl;
    print(par);
    
    par_t = transpose(par); //parity matrix, B, transposed

    cout << endl << "parity matrix transposed (B^T): " << endl;
    print(par_t);

    message = generate_message();   //generates random message

    cout << endl << "message (m):   ";
    print(message);

    encoded_message = encode(message,gen);  //encodes message

    cout << endl << "encoded message (mG):  ";
    print(encoded_message);

    noise = noisy_channel(encoded_message); //sends message over "noisy channel

    cout << endl << "recieved message (r = c + e):  ";
    print(noise);

    syndrome = multiply(noise , gen_t); //calculates s = rG^T

    cout << endl << "syndrome (s = rG^T):   "; 
    print(syndrome);
    cout << "weight of syndrome is " << weight(syndrome) << endl; 

    sB = multiply(syndrome,par);    //calculates sB 

    cout << endl << "sB:    "; 
    print(sB);
    cout  << "weight of sB is " << weight(sB); 

    /* computes the row vectors s + (c_j)^T and calculates the weight of the row vectors */
    for(i = 0 ; i < 12 ; i++)
    {
        for(j = 0 ; j < 12 ; j++)
        {
            temp.m[j][0] = par.m[j][i];
        }
        
        temp1 = transpose(temp);
        s_add_colB = addition(syndrome,temp1);

        for(k = 0 ; k < 13 ; k++)
        {
            if(k < 12)
            {
                s_add_colB_full.m[i][k] = s_add_colB.m[0][k];   
            }
            else
            {
                s_add_colB_full.m[i][k] = weight(s_add_colB);
            }
        }  
    }

    cout << "\n\n" << "full s + (c_j)^T (the values in the last "
         << "colums are the weights of the rows): " << endl;
    print(s_add_colB_full);

    /* computes the row vectors sB + (b_j)^T and calculates the weight of the row vectors */
    for(i = 0 ; i < 12 ; i++)
    {
        for(j = 0 ; j < 12 ; j++)
        {
            temp.m[j][0] = par_t.m[j][i];
        }

        temp1 = transpose(temp);
        sB_add_colB_T = addition(sB,temp1);

        for(k = 0 ; k < 13 ; k++)
        {
            if(k < 12)
            {
                sB_add_colB_T_full.m[i][k] = sB_add_colB_T.m[0][k];
            }
            else
            {
                sB_add_colB_T_full.m[i][k] = weight(sB_add_colB_T);
            }
        }
    }

    cout << "\n\n" << "full sB + (b_j)^T (the values in the last "
         << "colums are the weights of the rows): " << endl;
    print(sB_add_colB_T_full);

    /***** calculates the error vector e *****/
    if(weight(syndrome) <= 3)
    {
        for(i = 0; i < syndrome.m[0].size() ; i++)
        {
            if(syndrome.m[0][i] == 1)
            {
                error.m[0][i] = 1;
            }
        }
    }
    else if(weight(sB) <= 3)
    {        
        for(i = 0; i < sB.m[0].size() ; i++)
        {
            if(sB.m[0][i] == 1)
            {
                error.m[0][i+12] = 1;
            }
        }
    }

    if(error.m[0] == zeros)
    {
        for(i = 0; i < 12 ;i++)
        {
            if(s_add_colB_full.m[i][12] <= 2)
            {
                error.m[0][i+12] = 1;

                for(j = 0 ; j< 12 ; j++)
                {
                    if(s_add_colB_full.m[i][j] ==1)
                    {
                        error.m[0][j] = 1;
                    }
                }
            }
        }
    }

    if(error.m[0] == zeros)
    {
        for(i = 0; i < 12 ;i++)
        {
            if(sB_add_colB_T_full.m[i][12] <= 2)
            {
                error.m[0][i] = 1;

                for(j = 0 ; j< 12 ; j++)
                {
                    if(sB_add_colB_T_full.m[i][j] ==1)
                    {
                        error.m[0][j+12] = 1;
                    }
                }
            }
        }
    }

    cout << endl <<"the error vector is:    "; 
    print(error);

    original_message =addition(noise,error);    //computes the original message c = r + e
    
    /***** checks that the decoding was correcr *****/
    if(original_message.m == encoded_message.m)
    {
        cout << endl << "Transmitance and error correcting successful." << endl;
    }
    else 
    {
        cout << "error in transmittance or error correcting." << endl;
    }
    
    cout << endl << "computed original sent message:   ";
    print(original_message);
    cout << "         original sent message:   ";
    print(encoded_message); 


    cout << endl << "computed original message m :   ";
    for(i = 0 ; i < 12 ; i++)
    {
        cout << original_message.m[0][i] << "  ";
    }
    cout << endl << "         original message m :   ";
    for(i = 0 ; i < 12 ; i++)
    {
        cout << message.m[0][i] << "  ";
    }
    
    clock_t finish = clock();

    cout << "\n\n" << "Running time: " << (double)(finish-start)*1000/CLOCKS_PER_SEC<< " ms" << endl;
  
    return 0;
}
  
