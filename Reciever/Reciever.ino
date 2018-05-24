#define SAMPLES 32    // number of samples
#define SAMPLE_RATE 1000   // microsecond delay between samples

#define TABLELENGTH 128   //1/2 times max samples

#include <avr/pgmspace.h>

double wave[SAMPLES];     //input goes here
double freqmag[SAMPLES];      // output is placed here

const double costable[] PROGMEM={1.0, 0.9997, 0.9988, 0.9973, 0.9952, 0.9925, 0.9892, 0.9853, 0.9808, 0.9757, 0.97, 0.9638, 0.9569, 0.9495, 0.9415, 0.933, 0.9239, 0.9142, 0.904, 0.8932, 0.8819, 0.8701, 0.8577, 0.8449, 0.8315, 0.8176, 0.8032, 0.7883, 0.773, 0.7572, 0.741, 0.7242, 0.7071, 0.6895, 0.6716, 0.6532, 0.6344, 0.6152, 0.5957, 0.5758, 0.5556, 0.535, 0.5141, 0.4929, 0.4714, 0.4496, 0.4276, 0.4052, 0.3827, 0.3599, 0.3369, 0.3137, 0.2903, 0.2667, 0.243, 0.2191, 0.1951, 0.171, 0.1467, 0.1224, 0.098, 0.0736, 0.0491, 0.0245, 0.0, -0.0245, -0.0491, -0.0736, -0.098, -0.1224, -0.1467, -0.171, -0.1951, -0.2191, -0.243, -0.2667, -0.2903, -0.3137, -0.3369, -0.3599, -0.3827, -0.4052, -0.4276, -0.4496, -0.4714, -0.4929, -0.5141, -0.535, -0.5556, -0.5758, -0.5957, -0.6152, -0.6344, -0.6532, -0.6716, -0.6895, -0.7071, -0.7242, -0.741, -0.7572, -0.773, -0.7883, -0.8032, -0.8176, -0.8315, -0.8449, -0.8577, -0.8701, -0.8819, -0.8932, -0.904, -0.9142, -0.9239, -0.933, -0.9415, -0.9495, -0.9569, -0.9638, -0.97, -0.9757, -0.9808, -0.9853, -0.9892, -0.9925, -0.9952, -0.9973, -0.9988, -0.9997};
const double sintable[] PROGMEM={-0.0, -0.0245, -0.0491, -0.0736, -0.098, -0.1224, -0.1467, -0.171, -0.1951, -0.2191, -0.243, -0.2667, -0.2903, -0.3137, -0.3369, -0.3599, -0.3827, -0.4052, -0.4276, -0.4496, -0.4714, -0.4929, -0.5141, -0.535, -0.5556, -0.5758, -0.5957, -0.6152, -0.6344, -0.6532, -0.6716, -0.6895, -0.7071, -0.7242, -0.741, -0.7572, -0.773, -0.7883, -0.8032, -0.8176, -0.8315, -0.8449, -0.8577, -0.8701, -0.8819, -0.8932, -0.904, -0.9142, -0.9239, -0.933, -0.9415, -0.9495, -0.9569, -0.9638, -0.97, -0.9757, -0.9808, -0.9853, -0.9892, -0.9925, -0.9952, -0.9973, -0.9988, -0.9997, -1.0, -0.9997, -0.9988, -0.9973, -0.9952, -0.9925, -0.9892, -0.9853, -0.9808, -0.9757, -0.97, -0.9638, -0.9569, -0.9495, -0.9415, -0.933, -0.9239, -0.9142, -0.904, -0.8932, -0.8819, -0.8701, -0.8577, -0.8449, -0.8315, -0.8176, -0.8032, -0.7883, -0.773, -0.7572, -0.741, -0.7242, -0.7071, -0.6895, -0.6716, -0.6532, -0.6344, -0.6152, -0.5957, -0.5758, -0.5556, -0.535, -0.5141, -0.4929, -0.4714, -0.4496, -0.4276, -0.4052, -0.3827, -0.3599, -0.3369, -0.3137, -0.2903, -0.2667, -0.243, -0.2191, -0.1951, -0.171, -0.1467, -0.1224, -0.098, -0.0736, -0.0491, -0.0245};


void _myfft(double* x, double* rv, int N)
{
  int k,i;
  double wr,wi;

  if (N<=1)
  {
    rv[0]=x[0];
    rv[1]=0;
    return;
  }

  double xx[N/2];
  for (i=0; i<N/2; i++)
    xx[i]=x[i*2];
  double evens[N];
  _myfft(xx,evens,N/2);

  for (i=0; i<N/2; i++)
    xx[i]=x[i*2+1];
  double odds[N];
  _myfft(xx,odds,N/2);

  double Tr[N/2];
  double Ti[N/2];

  for(k=0; k<N/2; k++)
  {
    int index=2*k*TABLELENGTH/N;
    if(index<TABLELENGTH)
    {
        wr=pgm_read_float_near(costable+index);
        wi=pgm_read_float_near(sintable+index);
    }
    else
    {
      wr=-costable[index-TABLELENGTH];
      wi=-sintable[index-TABLELENGTH];
    }
    Tr[k]=odds[k]*wr - odds[k+N/2]*wi;
    Ti[k]=odds[k+N/2]*wr + odds[k]*wi;
  }
  for(k=0; k<N/2; k++)
  {
    rv[k]=evens[k]+Tr[k];
    rv[k+N]=evens[k+N/2]+Ti[k];
    rv[k+N/2]=evens[k]-Tr[k];
    rv[k+N+N/2]=evens[k+N/2]-Ti[k];
  }
}


void myfft()
{
  double F[SAMPLES*2];
  _myfft(wave,F,SAMPLES);

  for(int n=0; n<SAMPLES; n++)
  {
    freqmag[n]=F[n]*F[n] + F[n+SAMPLES]*F[n+SAMPLES];
  }
}
double getpitch() {
  for(int i=0; i<SAMPLES; i++)
  {
    int t = 10;
    float p = 1024;
    wave[i]= (analogRead(A1)/p);
    while(micros()-t<SAMPLE_RATE);
    t=micros();

  }

  double highestPitch = 0;
  myfft();
  for(int i=1; i<SAMPLES/2; i++)
  {
    //Serial.print(i);
    //Serial.print(" ");
    //Serial.println(freqmag[i]);
    if(freqmag[i]>highestPitch && freqmag[i]>0.1){
      highestPitch = freqmag[i];
    }
  }
  if(highestPitch==0){
    return -1;
  }
  return highestPitch;
}

int getbit(double pitch){

if(pitch >= .1 && pitch < 1.0){
  return 1;
}
if(pitch >=1.0 && pitch <= 2.5){
  return 0;
}
else return -1;
  
}
void setup() {

  Serial.begin(9600);
  pinMode(A1,INPUT);
  // double test[SAMPLES]={1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0};


  
}
void loop(){

  unsigned char c=0;
  int bit = getbit(getpitch());
  if(bit == 0){
    delay(1000);
    for(int i = 0; i < 8; i++){
      int pit = getbit(getpitch());
        Serial.println(pit);
          c = c >> 1;
          if (pit == 1)
              c = c | 0x80;
         delay(1000);
    }
  }
  Serial.println((char)c);
  delay(1000);
  
}



