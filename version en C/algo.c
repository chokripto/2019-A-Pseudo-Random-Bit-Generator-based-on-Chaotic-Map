//
//  algo.c
//  
//
//  Created by chokri on 17/01/2019.
//

#include "algo.h"
#include <stdio.h>
#include <stdint.h>
#include <conio.h>
#include <math.h>





main(){
    
    
    int i,j,k,q;
    double t,x1,x2,y1,y2,p=pow(2,32);
    unsigned long R,S;
    
    
    
    
    
    
    
    
    FILE *fichier=fopen("data.pi", "r+");
    
    j=0;
    while (j<1000){
        
        t=3.85;
        x1=0.03 + j*0.0001;
        y1=0.03 + j*0.0001;
        printf("le mot de pass est %f \n",x1);
        i=0;
        while (i<1001000){
            if (i<1000){
                x2 = 1 - y1 + fabs(x1);
                y2 = x1 + t*sin(y1);
                x1=x2;
                y1=y2;
                i++;
            }
            else {
                x2 = 1 - y1 + fabs(x1);
                y2 = x1 + t*sin(y1);
                x1=x2;
                y1=y2;
                S=floor(fmod(x1,1)*p);
                R=floor(fmod(y1,1)*p);
                S=S^R;
                //        fprintf( fichier, "%.9f  \n", x1*p  );
                k=0;
                while( k<32 ){
                    fprintf( fichier, "%d", (S>>(31-k))&1);
                    k++;
                    i++;
                    if(i>=1001000) break;
                }
            }
        }
        printf("le nombres des suites  = %d \n",j);
        j++;
    }
    fclose(fichier);
    getch();
    return 0;
}
