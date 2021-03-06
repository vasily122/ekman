//=========================================================================
//Решение нестационарной задачи о слое Экмана
//v0.4.2   06.08.2008
//
//
//=========================================================================
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int N=100,        // количество расчетных точек по z
    i=0,count=0;  //счетчик
double H=2000.0;  //высота слоя в метрах
double VX=10.0,   //начальная скорость на верхней границе по х
       VY=10.0,   //начальная скорость на верхней границе по у
       VZ=10.0;   //начальная скорость на верхней границе по z
double dVx0[100], //нижний уже рассчитанный слой
       dVx[100],  //начальные значения 
       dVx1[100]; //текущий рассчитываемый слой
double dVy0[100], //нижний уже рассчитанный слой
       dVy[100],  //начальные значения 
       dVy1[100]; //текущий рассчитываемый слой
double dVz0[100], //нижний уже рассчитанный слой
       dVz[100],  //начальные значения
       dVz1[100]; //текущий рассчитываемый слой
double kappa=1.5e-5;//кинематический коэффициент вязкости воздуха при 20 градусах Цельсия
double dhx,       // шаг по пространству
       dhy,
       dhz,
       dht;       // шаг по времени
double g=9.81;    // ускорение свободного падения
double eps=0.001; // критерий выхода - максимальная разница между значениями разицы
double delta=1.0; // максимальная разность между значениями скорости на разных временных
                  //слоях
 //double f=2.31481e-5;//множитель Корриолиса для воздуха
//double f=1.0;//множитель Корриолиса для воды
double f=2.31481e-2;//множитель Корриолиса для воздуха

int main(int argc, char *argv[])
{
dhx=dhy=dhz=H/(double)N;
dht=(2*kappa)/(dhx*dhx);
//dht=.01;//-можно варьировать для исследования поведения программы
//начальные условия как линейные функции от расстояния

for(i=0;i<=N-1;i++){
   dVx[i]=dVx1[i]=dVx0[i]=(double)i*VX/(double)N;     //
   dVy[i]=dVy1[i]=dVy0[i]=(double)i*VY/(double)N;     //
   dVz[i]=dVz1[i]=dVz0[i]=(double)i*VZ/(double)N;     //=0
   }

//цикл итераций фактически означает затраченное время.
for(count=0;count<=50000000;count++){
//задаем граничные условия
   dVx1[0]=dVx0[0]=0.0;
   dVy1[0]=dVy0[0]=0.0;
   dVz1[0]=dVz0[0]=0.0;
// верхниe граничныe услови
 //dVx1[N-1]=dVx0[N-1]=VX;
 //dVy1[N-1]=dVy0[N-1]=VY;
 //dVz1[N-1]=dVz0[N-1]=VZ;// =0

dVx1[N-1]=dVx0[N-1];
dVy1[N-1]=dVy0[N-1];
dVz1[N-1]=dVz0[N-1];// =0


//основное рассчетное уравнение
   for(i=1;i<=N-2;i++){
//------x------
      dVx1[i]=dVx0[i]-f*dht*dVy0[i];
      dVx1[i]=dVx1[i]+kappa*dht/(dhx*dhx)*(dVx0[i+1]-2.0*dVx0[i]+dVx0[i-1]);
      
//------y------
      dVy1[i]=dVy0[i]+f*dht*dVx0[i];
      dVy1[i]=dVy1[i]+kappa*dht/(dhy*dhy)*(dVy0[i+1]-2.0*dVy0[i]+dVy0[i-1]);
      
//------z------

      dVz1[i]=dVz0[i]-g*dht;
      dVz1[i]=dVz1[i]+kappa*dht/(dhz*dhz)*(dVz0[i+1]-2.0*dVz0[i]+dVz0[i-1]);
      
      }
//---- примнение верхнего граничногоусловия из уравнения неразрывности dV/dz=0 -> V=const
      dVx1[N-1]=dVx1[N-2];
      dVy1[N-1]=dVy1[N-2];
      dVz1[N-1]=dVz1[N-2];



//переприсвоение временных слоев и определение условий остановки
   for(i=0;i<=N-1;i++){
      delta=(delta>(abs(dVx1[i]-dVx0[i]))?delta:(abs(dVx1[i]-dVx0[i])) ); 
      delta=(delta>(abs(dVy1[i]-dVy0[i]))?delta:(abs(dVy1[i]-dVy0[i])) );
      delta=(delta>(abs(dVz1[i]-dVz0[i]))?delta:(abs(dVz1[i]-dVz0[i])) );
      

      dVx0[i]=dVx1[i];
      dVy0[i]=dVy1[i];
      dVz0[i]=dVz1[i];
      }
 //---if(delta<eps)break;// условие остановки счета БЕСКОНЕЧНОГО цикла: delta < eps
 }  //--- for(;;) ---
   for(i=0;i<=N-1;i++){
//--- вывод значений скоростей 
//      printf("%i %11.4e %11.4e   %11.4e \n",i,dVx0[i],dVy0[i],dVz0[i]);
//--- вывод отклонений значений скоростей от начальных условий
//      printf("%i %11.4e   %11.4e   %11.4e \n",i,dVx0[i]-dVx[i],dVy0[i]-dVy[i],dVz0[i]-dVz[i]);
// ----значений скоростей и отклонений значений скоростей от начальных условий
printf("%i %11.4e   %11.4e   %11.4e  %11.4e %11.4e %11.4e\n" \
,i,dVx0[i],dVy0[i],dVz0[i],dVx0[i]-dVx[i],dVy0[i]-dVy[i],dVz0[i]-dVz[i]);
}
  return EXIT_SUCCESS;
}  //---main---
