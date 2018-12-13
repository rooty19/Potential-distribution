#include <stdio.h>
#include <stdlib.h>
void debug_attry_d(double **matrix,int count_x,int count_y);
void debug_attry_i(int **matrix,int count_x,int count_y);
void setup_pf(double **d_matrix,int **i_matrix,int count_x,int count_y);
void set_pf_mask_square(double **d_matrix,int **i_matrix,int count_x,int count_y,int start_x,int start_y,int end_x,int end_y,double value,int mode);
void calculation(double **n_matrix,double **o_matrix,int **i_matrix,int count_x,int count_y);
void new2old(double **n_matrix,double **o_matrix,int count_x,int count_y);
void calcfc(long long int max_count,double **n_matrix,double **o_matrix,int **i_matrix,int count_x,int count_y,int x1,int y1,int x2,int y2,int multiple);

int main() {
    FILE *data, *gp;
    char *data_file;

    static int count_x=201;
    static int count_y=301;
    static double delta_x = 0.01;
    static double delta_y = 0.01;
    static long long int calc_count_max = 1000000;

    int cox=count_x -1;
    int coy=count_y -1;

    double **new_voltage = (double**)malloc(count_y*sizeof(double*));
    new_voltage[0]=(double*)malloc(count_y*count_x*sizeof(double));
    for(int i=0; i<count_y; i++){
        new_voltage[i]=new_voltage[0] + i*count_x;
    }

    double **old_voltage = (double**)malloc(count_y*sizeof(double*));
    old_voltage[0]=(double*)malloc(count_y*count_x*sizeof(double));
    for(int i=0; i<count_y; i++){
        old_voltage[i]=old_voltage[0] + i*count_x;
    }

    int **mask = (int**)malloc(count_y*sizeof(int*));
    mask[0]=(int*)malloc(count_y*count_x*sizeof(mask));
    for(int i=0; i<count_y; i++){
        mask[i]=mask[0] + i*count_x;
    }

    
    setup_pf(old_voltage,mask,count_x,count_y);
    //set_pf_mask_square(old_voltage,mask,count_x,count_y,600,600,1000,1000,5.0,1);
    set_pf_mask_square(old_voltage,mask,count_x,count_y,60,60,100,100,5,1);
    set_pf_mask_square(old_voltage,mask,count_x,count_y,0,0,cox,0,0,1);
    set_pf_mask_square(old_voltage,mask,count_x,count_y,0,coy,cox,coy,0,1);
    set_pf_mask_square(old_voltage,mask,count_x,count_y,0,0,0,coy,0,1);
    set_pf_mask_square(old_voltage,mask,count_x,count_y,cox,0,cox,coy,0,1);
    //debug_attry_i(mask,count_x,count_y);
    /*
    for(long int i=0;i<calc_count_max;i++){
        calculation(new_voltage,old_voltage,mask,count_x,count_y);
        new2old(new_voltage,old_voltage,count_x,count_y);
        if(i%100==0)putchar('.');
    }
    */
//    calcfc(100000000,new_voltage,old_voltage,mask,count_x,count_y,600,600,1000,1000,0);
    calcfc(calc_count_max,new_voltage,old_voltage,mask,count_x,count_y,60,60,100,100,0);
    
    putchar('\n');

    
    //debug_attry_d(new_voltage,count_x,count_y);
    //putchar('\n');
    //debug_attry_i(mask,count_x,count_y);

    double long_x,long_y;
    data_file="pd-plot.dat";
    data = fopen(data_file,"w");
    for(int i=0; i<count_y; i++){
        for(int j=0; j<count_x; j++){
            //if(j%10==0 && i%10==0)
            fprintf(data,"%lf\t%lf\t%lf\n",j*delta_x,i*delta_y,new_voltage[i][j]);
        }
        putchar('\n');
    }
    fclose(data);
    gp = popen("gnuplot -persist","w");
    fprintf(gp, "set xrange [0:%.3lf]\n",cox*delta_x);
    fprintf(gp, "set yrange [0:%.3lf]\n",coy*delta_y);
    fprintf(gp, "splot \"%s\" with lines linetype 1 title \"hoge\"\n",data_file);
    pclose(gp);

    free(old_voltage);
    free(new_voltage);
    free(mask);
    
}

void debug_attry_d(double **matrix,int count_x,int count_y){
    for(int i=0; i<count_y; i++){
        for(int j=0; j<count_x; j++){
            //matrix[i][j]=1;
            //printf("matrix[%d][%d]=%lf ", i, j, matrix[i][j]);
            printf("%.1lf ",matrix[i][j]);
        }
        putchar('\n');
    }

}

void debug_attry_i(int **matrix,int count_x,int count_y){
    for(int i=0; i<count_y; i++){
        for(int j=0; j<count_x; j++){
            //matrix[i][j]=1;
            //printf("matrix[%d][%d]=%lf ", i, j, matrix[i][j]);
            printf("%d ",matrix[i][j]);
        }
        putchar('\n');
    }
}

void setup_pf(double **d_matrix,int **i_matrix,int count_x,int count_y){
    for(int i=0; i<count_y; i++){
        for(int j=0; j<count_x; j++){
            i_matrix[i][j]=0;
            d_matrix[i][j]=0.0;
            //printf("matrix[%d][%d]=%lf ", i, j, matrix[i][j]);
            //printf("%d ",matrix[i][j]);
        }
        //putchar('\n');
    }

}

void set_pf_mask_square(double **d_matrix,int **i_matrix,int count_x,int count_y,int start_x,int start_y,int end_x,int end_y,double value,int mode){
    int *sxp,*syp,*gxp,*gyp;

    if(start_x < end_x){
        sxp = &start_x;
        gxp = &end_x;
    }else{
        sxp = &end_x;
        gxp = &start_x;
    }
    if(start_y < end_y){
        syp = &start_y;
        gyp = &end_y;
    }else{
        syp = &end_y;
        gyp = &start_y;
    }

    for(int i=*syp; i<=*gyp; i++){
        for(int j=*sxp; j<=*gxp; j++){
            if(mode == 1){
            i_matrix[i][j]=1;
            d_matrix[i][j]=value;
            //printf("matrix[%d][%d]=%lf ", i, j, matrix[i][j]);
            //printf("%d ",matrix[i][j]);
            }else if(mode == 0){
            i_matrix[i][j]=0;
            d_matrix[i][j]=0;
            }
        }
        //putchar('\n');
    }
    if(mode !=0 && mode !=1)printf("set_pf_mask_square(): ERROR\n");

}

void calculation(double **n_matrix,double **o_matrix,int **i_matrix,int count_x,int count_y){
    for(int i=0; i<count_y; i++){
        for(int j=0; j<count_x; j++){
            if(i_matrix[i][j]==1){
                n_matrix[i][j] = o_matrix[i][j];
            }else{
                n_matrix[i][j] = (o_matrix[i-1][j]+o_matrix[i+1][j]+o_matrix[i][j-1]+o_matrix[i][j+1])/4;
            }       
        }
    }
}

void new2old(double **n_matrix,double **o_matrix,int count_x,int count_y){
    for(int i=0; i<count_y; i++){
        for(int j=0; j<count_x; j++){
            o_matrix[i][j] = n_matrix[i][j];
        }
    }
}

void calcfc(long long int calc_count_max,double **n_matrix,double **o_matrix,int **i_matrix,int count_x,int count_y,int x1,int y1,int x2,int y2,int multiple){
    long long t_count = 0,hoge=0;
    int *sxp,*syp,*gxp,*gyp;
    int delta = 1;
    int minus_x,plus_x;
    int minus_y,plus_y;
    
    if(x1 < x2){
        sxp = &x1;
        gxp = &x2;
    }else{
        sxp = &x2;
        gxp = &x1;
    }
    if(y1 < y2){
        syp = &y1;
        gyp = &y2;
    }else{
        syp = &y2;
        gyp = &y1;
    }
    minus_x = *sxp -1;
    plus_x = *gxp + 1;
    minus_y = *syp -1;
    plus_y = *gyp + 1;

    while(t_count <= calc_count_max){
        for(int i=minus_y; i<=plus_y; i++){
            for(int j=minus_x; j<plus_x; j++){
                if(i_matrix[i][j]==1){
                    n_matrix[i][j] = o_matrix[i][j];
                }else{
                    n_matrix[i][j] = (o_matrix[i-1][j]+o_matrix[i+1][j]+o_matrix[i][j-1]+o_matrix[i][j+1])/4;
                }       
            }
        }
        minus_x--;
        minus_y--;
        plus_x++;
        plus_y++;
        if(minus_x <= 0)minus_x=0;
        if(minus_y <= 0)minus_y=0;
        if(plus_x >= count_x-1)plus_x=count_x-1;
        if(plus_y >= count_y-1)plus_y=count_y-1;
        t_count++;
        printf("%lld\n",hoge);
        hoge++;
        if(t_count%1000==0)printf("t_count");
        new2old(n_matrix,o_matrix,count_x,count_y);
    }

}