bool isRunning = false;
SystemSleepConfiguration loopSleep;
SystemSleepConfiguration debounceSleep;

bool interruptFired = false;


void setup() {
  pinMode(D5, INPUT_PULLUP);

  Particle.publish("status", getPumpStatusString("Startup: "));
  isRunning = isPumpRunning();
  loopSleep.mode(SystemSleepMode::STOP)
      .duration(500ms);
  debounceSleep.mode(SystemSleepMode::STOP)
      .duration(100ms);      
  
  attachInterrupt(D5, pumpStatusChanged, CHANGE);
}


void pumpStatusChanged() {
  interruptFired = true;
}

void loop() {
  String prefix = "Pump status change: ";
  SystemSleepResult result = System.sleep(loopSleep);

  if (interruptFired){
    bool tmpRead = isPumpRunning();
    SystemSleepResult result = System.sleep(debounceSleep);
    if ( tmpRead == isPumpRunning() ) {
      if( tmpRead != isRunning ) {
        isRunning = tmpRead;
        Particle.publish("status", getPumpStatusString(prefix));
        Particle.publish("pushbullet", getPumpStatusString(prefix));
      } else {
        // nothing. pump status didn't change.
      }
    } else {
      Particle.publish("status", "flake alert");
    }

  }
}

bool isPumpRunning() {
  return !digitalRead(D5);
}

String getPumpStatusString(String prefix) {
  bool pin = isPumpRunning();
  String status = prefix + "  Running: ";
  if(pin) {
    status += "true";
  } else {
    status += "false";
  }
  return status;
}