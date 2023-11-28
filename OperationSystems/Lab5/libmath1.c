#include "stdio.h"
#include "math.h"

float trapez_integral(float a, float b, float n){
    int i;
    double h,x,sum=0,integral;
    /*Begin Trapezoidal Method: */
    h=fabs(b-a)/n;
    for(i=1;i<n;i++){
        x=a+i*h;
        sum=sum+sin(x);
    }
    integral=(h/2)*(sin(a)+sin(b)+2*sum);
    return integral;
}

float rectangle_integral(float a, float b, float n){
    double h,S=0,x;
    int i;
    /*Begin Rectangle Method: */
    h=fabs(b-a)/n;
    for(i=0;i<n-1;i++)
    {
        x=a+i*h;
        S=S+sin(x);
    }
    S=h*S;
    return S;
}
