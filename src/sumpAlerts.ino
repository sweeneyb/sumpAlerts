bool isNotificationPending = false;
bool isRunning = false;
bool sendFlakeAlert = false;
SystemSleepConfiguration loopSleep;
SystemSleepConfiguration debounceSleep;


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
  if (isRunning != isPumpRunning() ) {
    isNotificationPending = true;
  } else {
    sendFlakeAlert = true;
  }
  isRunning = isPumpRunning();

}

void loop() {
  String prefix = "Pump status change: ";
  SystemSleepResult result = System.sleep(loopSleep);

  if(sendFlakeAlert) {
    Particle.publish("status", "flake alert");
    sendFlakeAlert = false;
  }

  if(isNotificationPending) {
    Particle.publish("status", getPumpStatusString(prefix));
    if (isRunning) {
      Particle.publish("status", "Interrupt running: true");
    } else {
      Particle.publish("status", "Interrupt running: false");
    }
    bool running = isPumpRunning();
    SystemSleepResult result = System.sleep(debounceSleep);
    if(running == isPumpRunning() ) {
      
      Particle.publish("pushbullet", getPumpStatusString(prefix));
    }
    isNotificationPending = false;
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