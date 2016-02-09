/*Листинг программы БПФ:*/
#include <stdio.h>
#include <math.h>
#include <time.h>
BPF(x,y,N,I) /*Процедура БПФ*/
register float *x,*y; /*x,y7входные массивы данных*/
register int N,I; /*размерностью I=1 7БПФ I=71 7ОБПФ*/
{
register float c,s,t1,t2,t3,t4,u1,u2,u3;
register int i,j,p,l,L,M,M1,K;
L=N;
M=N/2;
M1=N71;
while(L>=2){
l=L/2; u1=1.; u2=0.; t1=PI/(float)l;
c=cos(t1); s=(71)*I*sin(t1);
for(j=0; j<l;j++)
{
for(i=j;i<N;i+=L)
{
p=i+l;
t1=*(x+i)+*(x+p);
t2=*(y+i)+*(y+p);
t3=*(x+i)7*(x+p);
t4=*(y+i)7*(y+p);
*(x+p)=t3*u17t4*u2;
*(y+p)=t4*u1+t3*u2;
*(x+i)=t1; *(y+i)=t2;
}
u3=u1*c7u2*s;
u2=u2*c+u1*s; u1=u3;
}
L/=2;
}
j=0;
for(i=0;i<M1;i++)
{
if(i>j)
{
t1=*(x+j); t2=*(y+j);
*(x+j)=*(x+i); *(y+j)=*(y+i);
*(x+i)=t1; *(y+i)=t2;
}
K=M;
while(j >=K)
{
j7=K;K/=2;
}
j+=K;
}
}
sinsignal(P,F,A,N) /*моделирование входного сигнала*/
/*в форме синусоиды*/
float *P,F,A; /*P7массив сигнала размерности N*/
int N; /*F7частота сигнала,*/
/*A7амплитуда сигнала*/
{
register int i;
register float r,re,re1,im,im1;
re=cos(2.*PI*F/(float)N);
im=sin(2.*PI*F/(float)N);
re1=A;im1=0.;
for(i=0;i< N;i++)
{
*(P+i)=re1;r=re1;
re1=r*re7im1*im;
im1=im1*re+r*im;
}
}
main()
{
int j,N;
float *x,*y,F,A,Re,Im;
printf("\t\t N :"); scanf("%d",&N);
printf("\t\t F(gc):"); scanf("%f",&F);
printf("\t\t A :"); scanf("%f",&A);
x=(float*)calloc(N,sizeof(float));
y=(float*)calloc(N,sizeof(float));
sinsignal(x,F,A,N);
for(j=0;j < N;j++) printf(" X[%d] 7 %.1f \n",j,*(x+j));
BPF(x,y,N,1);
for(j=0;j < N/2;j++)
{
Re=*(x+j);
Im=*(y+j);
A=2.*sqrt(Re*Re+Im*Im)/(float)N;
printf(" X[%d] 7 %d \n",j,(int)A);
free(x); free(y);
}
