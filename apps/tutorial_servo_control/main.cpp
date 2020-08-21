
AnalogIn potVoltageIn(PA_0);
PwmOut servoPwmOut(PA_1);

int main(){
  servoPwmOut.period(0.020);
  while(1){
    float potVoltage = potVoltageIn.read();
    servoPwmOut.pulsewidth(0.001 + 0.001*potVoltage/3.3);

  }
  return 0;
}