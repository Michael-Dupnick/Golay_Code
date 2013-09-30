/*********************golay.h*********************/

#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include <string>

using namespace std;

class matrix
{
    public:
        int row;
        int column;    
        vector < vector <int> > m;  

        matrix() {};
        matrix(int r, int c) 
        {
            row = r;
            column = c;

            for (int i = 0; i < r; i++) 
            {
                m.push_back(vector<int>()); 
            }

            for (int j = 0; j < c; j++) 
            {
                for (int i = 0; i < m.size(); i++) 
                {
                     m[i].push_back(0); 
                }
            }
        }
};

matrix multiply(matrix a, matrix b);
void print(matrix a);
matrix transpose(matrix a);
matrix golay_generator();
matrix generate_B();
matrix generate_I(int a);
matrix addition(matrix a, matrix b);
matrix generate_message();
matrix encode(matrix m);
matrix noisy_channel(matrix m);
int weight(matrix a);

//functions prototypes
 matrix multiply(matrix a, matrix b)
{
    int i,j,k,l;
    int tem = 0;
    int index = 0;
    matrix temp(a.row , b.column);

    if (a.column != b.row)
    {
        cout << "ERROR: can't multiply these two matrices";
    }
    else
    {
        for(i = 0 ; i < a.row ; i++)
        {
            for(j = 0; j < b.column ; j++)
            {
                for(k = 0 ; k < a.column ; k++)
                {   
                    tem = a.m[i][k] * b.m[k][j];
                    index = index + tem;
                    tem = 0;
                }

                temp.m[i][j] = index % 2;
                index = 0;
            }
        }
    }

    return temp;
}

matrix addition(matrix a, matrix b)
{
    matrix temp(a.row, a.column);
    int i,j;

    if(a.row != b.row || a.column != b.column)
    {
        cout << "Error: cannot add these matrices";
    }
    else
    {
        for(i = 0 ; i < a.row ; i++)
        {
            for(j = 0 ; j < a.column ; j++)
            {
                temp.m[i][j] = (a.m[i][j] + b.m[i][j]) % 2;
            }
        }
    }

    return temp;    
}

matrix transpose(matrix a)
{
    matrix temp(a.column , a.row);
    int i, j;

    for(i = 0; i < a.row ; i++)
    {
        for(j = 0 ; j < a.column ; j++)
        {
            temp.m[j][i] = a.m[i][j];
        }
    }

    return temp;
}

void print(matrix a)
{
    int i,j;

    for (i = 0 ; i < a.row ; i++)
    {
        for (j = 0 ; j < a.column ; j++)
        {

            cout << a.m[i][j] << "  ";
        }

        cout << endl;
    }
}

matrix golay_generator()
{
    matrix generator(12,24);
    int i, j, k;
    int check = 0;
    int count = 1;
    vector<int> squares(1);

    for(i = 0 ; i < 12 ; i++)
    {
        generator.m[i][i] = 1;
    }

    for(i = 0 ; i < 11 ; i++)
    {
        generator.m[i][12] = 1;
    }

    for(i = 13 ; i < 24 ; i++)
    {
        generator.m[11][i] = 1;
    }

    for(i = 0 ; i < 11 ; i++)
    {
        j = (i*i) % 11;

        for(k = 0; k < squares.size() ; k++)
        {
            if(j == squares[k])
            {
                check = 1;
            }
            
        }

        if(check == 0)
        {
            squares.push_back(j);
        }

        check = 0;
    }

    for(i = 0 ; i < squares.size() ; i++)
    {
        squares[i] = squares[i] + 13;
    }

    for(i = 13 ; i < 24 ; i++)
    {
        for(j = 0 ; j < squares.size() ; j++)
        {
            if(squares[j] == i)
            {
                generator.m[0][i] = 1;
            }
        }
    }

    while(count != 11)
    {
        for(i = 0 ; i < squares.size() ; i++)
        {
            squares[i] = ((squares[i] - 12) % 11) + 13;
        }

        for(i = 13 ; i < 24 ; i++)
        {
            for(j = 0 ; j < squares.size() ; j++)
            {
                if(squares[j] == i)
                {
                    generator.m[count][i] = 1;
                }
            }
        }

        count++;
    }

    return generator;
}

matrix generate_B()
{                      
    matrix generator(12,12);
    int i, j, k;
    int count = 1;
    int check = 0;
    vector<int> squares;

    for(i = 0 ; i < 11 ; i++)
    {
        generator.m[i][0] = 1;
    }

    for(i = 1 ; i < 12 ; i++)
    {
        generator.m[11][i] = 1;
    }

    for(i = 0 ; i < 11 ; i++)
    {
        j = (i*i) % 11;

        for(k = 0; k < squares.size() ; k++)
        {
            if(j == squares[k])
            {
                check = 1;
            }
            
        }

        if(check == 0)
        {
            squares.push_back(j);
        }

        check = 0;
    }

    for(i = 0 ; i < squares.size() ; i++)
    {
        squares[i] = squares[i] + 1;
    }

    for(i = 0 ; i < 12 ; i++)
    {
        for(j = 0 ; j < squares.size() ; j++)
        {
            if(squares[j] == i)
            {
                generator.m[0][i] = 1;
            }
        }
    }

    while(count != 11)
    {
        for(i = 0 ; i < squares.size() ; i++)
        {
            squares[i] = (squares[i] % 11) + 1;
          

        }

        for(i = 1 ; i < 12 ; i++)
        {
            for(j = 0 ; j < squares.size() ; j++)
            {
                if(squares[j] == i)
                {
                    generator.m[count][i] = 1;
                }
            }
        }

        count++;
    }

    return generator;
}

matrix generate_I(int a)
{
    matrix temp(a,a);
    int i;

    for(i = 0 ; i < a ; i++)
    {
        temp.m[i][i] = 1;
    }

    return temp;
}

matrix generate_message()
{
    matrix message(1,12);
    int i,temp;

    srand (time(NULL));
    
    for(i = 0 ; i < 12 ; i++)
    {
        temp = rand()%2;
        message.m[0][i] = temp;   
    }

    return message;
}  

matrix encode(matrix m, matrix G)
{
    matrix temp = multiply(m,G);
    
    return temp;
}  

matrix noisy_channel(matrix m)
{
    int i = 0;
    int r;
    int errors = 0;
    matrix temp = m;

    srand (time(NULL));

    while(errors != 3 && i != temp.m[0].size())
    {

        r = rand()%100;

        if(r >= 25 && r <= 35)
        {
            temp.m[0][i] = (temp.m[0][i] + 1) % 2;

            errors++;
        }

        i++;
    }

    cout << endl << "number of errors during transmission were: " << errors << endl;

    return temp;
}
                                     
int weight(matrix a)
{
    int temp = 0;
    int i;

    for(i = 0 ; i < a.m[0].size() ; i++)
    {
        if(a.m[0][i] == 1)
        {
            temp++;
        }
    }

    return temp;
}

    






















            
