#include <stdio.h>
#include <stdlib.h>


void print_matrix(double** array, int rows, int cols)
{
        //print a matrix
        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                printf("%.0f",array[i][j]);
                }
            printf("\n");
        }
}

void fetch_x(double **matrix, int rows, int  cols, double **X){
    for(int i=0;i<rows;i++){
        X[i][0]=1;
    }
    for(int i=0;i<rows;i++){
        for(int j=1;j<cols;j++){
            X[i][j]=matrix[i][j-1];      
        }
    }
 //   print_matrix(X,rows,cols);
}

void fetch_y(double **matrix, int rows, int  cols, double **Y){
    for(int i=0;i<rows;i++){
        Y[i][0]=matrix[i][cols-1];
    }
   // print_matrix(Y,rows,1);
}

void transpose(double **A, double **B,int M, int N)
{
    int i, j;
    for (i = 0; i < N; i++)
        for (j = 0; j < M; j++)
            B[i][j] = A[j][i];
}
                //Result matrix   //matrix X_t    //matrix X
void multiply(double **arr3, double **arr1, double **arr2, int ROW1, int COL1, int COL2){
      // multiply two matrices
    for(int i = 0; i < ROW1; i++)
    {
        for(int j = 0; j < COL2; j++)
        {
            arr3[i][j] = 0;
            for(int k = 0; k < COL1; k++)
            {
                arr3[i][j] += arr1[i][k] * arr2[k][j];            
            }                    // printf("%d\t", arr3[i][j]);                
        }                
      //  printf("\n");
    }   
}

void divide(double *N, double f, int n){
    for (int i=0; i<n; i++){
   // printf("%lf / %lf",N[i],f);
     N[i]=N[i]/f;
   //  printf(" : %lf \n",N[i]);
    }
}

void invert(double **M,int n,double**N){

//Gauss Jordan Elimination
    for(int p=0;p<n;p++){ 
    double f;   
        f =M[p][p];
        divide(M[p],f,n);
        divide(N[p],f,n);
        for(int i=p+1;i<n;i++){
            f=M[i][p];
            for(int k=0;k<n;k++){
            M[i][k]=M[i][k]-(M[p][k]*f);}
            for(int k=0;k<n;k++)
            N[i][k]=N[i][k]-(N[p][k]*f);
        }
    }
    for(int p=n-1;p>=0;p--){
        for(int i=p-1;i>=0;i--){
        double f=M[i][p];
          for(int k=0;k<n;k++){
            M[i][k]=M[i][k]-(M[p][k]*f);}
            for(int k=0;k<n;k++){
            N[i][k]=N[i][k]-(N[p][k]*f);}
        }
    }
}

void free_matrix(double **matrix_name,int rows){

     for(int i=0;i<rows;i++)
        free(matrix_name[i]);
    free(matrix_name);
}

double** initialize_matrix(int rows,int cols){
    
double ** matrix = calloc(sizeof(double*),rows);
    for(int i=0;i<rows;i++){
        matrix[i]=calloc(sizeof(double),cols);
      
    }
    for(int i=0;i<rows;i++){
    for(int p=0;p<cols;p++)
        matrix[i][p]=0;
    }
return matrix;
}

int main(int argc, char **argv){
    
    FILE * fPointer;
    FILE * Data;
    fPointer = fopen(argv[1],"r");
    Data = fopen(argv[2],"r");
    
    int k;
    int input_rows;    
    char c,d,e,f,g;
         
        fscanf(fPointer, "%c%c%c%c%c",&c,&d,&e,&f,&g);
        fscanf(fPointer, " %d",&k);
        fscanf(fPointer," %d",&input_rows);

    int  input_cols=k+1;
    int x_rows=input_rows;
    int x_cols=input_cols;
    int y_rows=input_rows;
    int y_cols=1;


//Initializing Matrix Input Matrix
double ** input_matrix = initialize_matrix(input_rows,input_cols);
   
    //Scanning Input Matrix from file
    for(int i=0;i<input_rows;i++){
    for(int j=0;j<input_cols;j++){       
       fscanf(fPointer,"%lf ",&input_matrix[i][j]);
    }   
    }
   
//Initializing Matrix X
double ** X = initialize_matrix(x_rows,x_cols);

//Initializing Matrix Y 
double ** Y = initialize_matrix(y_rows,y_cols);
double **x_t = initialize_matrix(x_cols,x_rows);

fetch_x(input_matrix,x_rows,x_cols,X);    
fetch_y(input_matrix,y_rows,input_cols,Y);    
free_matrix(input_matrix,input_rows);  
transpose(X,x_t,x_rows,x_cols);

    int x_t_rows=x_cols;
    int x_t_cols=x_rows; 
     
//initializing  New Matrix 

double **result_matrix = initialize_matrix(x_t_rows,x_cols);
multiply(result_matrix,x_t,X,x_t_rows,x_t_cols,x_cols);
double **Invert_matrix = initialize_matrix(x_cols,x_cols);

//Initializing Identity Matrix in N
    for(int i=0;i<x_cols;i++){
        for(int p=0;p<x_cols;p++){
            if(i==p)
                Invert_matrix[i][p]=1;
            else Invert_matrix[i][p]=0;
        }
    }

    invert(result_matrix, x_t_rows,Invert_matrix);
    double **ixt = initialize_matrix(x_cols,x_rows);
    multiply(ixt,Invert_matrix,x_t,x_cols,x_cols,x_rows);
    double **W = initialize_matrix(x_t_rows,y_cols);
    multiply(W,ixt,Y, x_t_rows,x_rows,y_cols);

    int data_cols;
    int data_rows;
    char a,b,h,o,q;
     fscanf(Data, " %c%c%c%c%c",&a,&b,&h,&o,&q);
     fscanf(Data, " %d",&data_cols);
     fscanf(Data," %d",&data_rows);

    double **data_X = initialize_matrix(data_rows,data_cols+1);

    for(int j=0;j<data_rows;j++){       
       data_X[j][0]=1;
    }
   
   //Scanning Input Matrix from file
    for(int i=0;i<data_rows;i++){
    for(int j=0;j<data_cols;j++){       
       fscanf(Data,"%lf ",&data_X[i][j+1]);
    }   
    }

double **final_Y = initialize_matrix(data_rows,y_cols);    
multiply(final_Y,data_X,W,data_rows,data_cols+1,y_cols);
print_matrix(final_Y,data_rows,y_cols);
free_matrix(ixt,x_cols);
free_matrix(data_X,data_rows);  
free_matrix(W,x_t_rows);
free_matrix(final_Y,data_rows);
free_matrix(Invert_matrix,x_cols);
free_matrix(X,x_rows);
free_matrix(Y,y_rows);
free_matrix(x_t,x_t_rows);
free_matrix(result_matrix,x_cols);
    return 0;
}
