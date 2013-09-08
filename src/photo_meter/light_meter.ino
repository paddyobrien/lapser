volatile unsigned long cnt = 0;

int calculateExpTime()
{
  run_status |= FLG_LS_RUN;
  MsTimer2::set(sample_time, stopSampling);
  MsTimer2::start();
  LMAttachInterrupt();
}

void stopSampling()
{
  MsTimer2::stop();
  LMDetachInterrupt();
  //double hz = cnt * (1000 / sample_time);
  //double irradiance = (hz+50)/100;
  //double ev = log(irradiance) - log(2.5)/log(2);
  //exposure_time = pow(F_stop, 2) / pow(2, ev);
  exposure_time = (pow(F_stop, 2) / pow(2, (log(((cnt * (1000 / sample_time))+50)/100) - log(2.5))/log(2))) * 1000;
  run_status -= FLG_LS_RUN;
  run_status |= FLG_LS_FIN;
}

void LMAttachInterrupt()
{
  cnt = 0;
  attachInterrupt(INT_LIGHT_SENSOR, interruptHandler, RISING);
}

void interruptHandler()
{
  cnt ++;
}

void LMDetachInterrupt()
{
  detachInterrupt(INT_LIGHT_SENSOR);
}
