bool isNotificationPending = false;
SystemSleepConfiguration config;

void setup() {
  pinMode(D5, INPUT_PULLUP);

  Particle.publish("status", getPumpStatusString("Startup: "));
  config.mode(SystemSleepMode::STOP)
      .duration(500ms);
  
  attachInterrupt(D5, pumpStatusChanged, CHANGE);
}


void pumpStatusChanged() {
  isNotificationPending = true;
}

void loop() {
  String prefix = "Pump status change: ";
  SystemSleepResult result = System.sleep(config);

  if(isNotificationPending) {
    Particle.publish("status", getPumpStatusString(prefix));
    Particle.publish("pushbullet", getPumpStatusString(prefix));
    isNotificationPending = false;
  }
}

bool getPumpStatus() {
  return !digitalRead(D5);
}

String getPumpStatusString(String prefix) {
  bool pin = getPumpStatus();
  String status = prefix + "  Running: ";
  if(pin) {
    status += "true";
  } else {
    status += "false";
  }
  return status;
}