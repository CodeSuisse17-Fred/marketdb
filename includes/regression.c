#include<stdio.h>
#include<math.h>

void regression(char *arr[]){
	int n,I,i, j,k,l;
	float sumx, sumsq, sumy, sumxy, x, y, a0, a1, denom, sumx3, sumx4, sumxsqy, A[20][20], u=0.0, b[20];

	n = 0;//length of data (how many data sets)
	sumx = 0;
	sumsq = 0;
	sumy = 0;
	sumxy = 0;
	sumx3 = 0;
	sumx4 = 0;
	sumxsqy = 0;

	for(i = 0; i < n; i++){
		&x = 0;//time
		&y = 0;//data
		sumx += x; //linear regression & poly
		sumsq += pow(x, 2); //linear regression
		sumy += y;//linear regression & poly
		sumxy += x * y;//linear regression
		sumx3 += pow(x,3);
        sumx4 += pow(x,4);
        sumxsqy += pow(x,2) *y;
 	}

 	A[0][0] = n;
    A[0][1] = sumx;
    A[0][2] = sumsq;
    A[0][3] = sumy;
    A[1][0] = sumx;
    A[1][1] = sumsq;
    A[1][2] = sumx3;
    A[1][3] = sumxy;
    A[2][0] = sumsq;
    A[2][1] = sumx3;
    A[2][2] = sumx4;
    A[2][3] = sumxsqy;

	denom = n * sumsq – pow(sumx, 2);
	a0 = ((sumy * sumsq) – (sumx * sumxy)) / denom;
	a1 = ((n * sumxy) – (sumx * sumy)) / denom;


    for(k=0;  k<=2; k++){
        for(j=0;j<=2;j++){
			if(j!=k)
				u=A[j][k]/A[k][k];
			for(l = k; l<=3; l++)
			A[j][l]= A[j][l] – (u * A[k][l]);
		}
	}

	printf("y= %10.4fx^2 +%10.4fx +%10.4f",b[2],b[i],b[0]);

	printf("linear regression: y = %fx + %f\nPlease input time you would like to calculate",a1, a0);
}