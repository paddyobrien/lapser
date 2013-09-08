
void fireCamera(unsigned long exp_tm) {
  run_status |= FLG_CM_RUN;
  // Pull focus and shutter to bulb expose
  digitalWrite(PIN_FOCUS, HIGH);
  digitalWrite(PIN_SHUTTER, HIGH);
  // start timer to stop camera exposure
  MsTimer2::set(exp_tm, stopCamera);
  MsTimer2::start();
  return;
}

void stopCamera() {
  // Focus and shutter to low to stop exposing
  digitalWrite(PIN_SHUTTER, LOW);
  digitalWrite(PIN_FOCUS, LOW);
  MsTimer2::stop();
  // set the correct status flags
  run_status -= FLG_CM_RUN;
  run_status |= FLG_CM_FIN;
}
