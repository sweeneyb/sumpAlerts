bool isNotificationPending = false;
SystemSleepConfiguration config;

void setup() {
  Particle.publish(getPumpStatusString());
  config.mode(SystemSleepMode::STOP)
      .duration(500ms);
  pinMode(D5, INPUT_PULLUP);
  attachInterrupt(D5, pumpStatusChanged, CHANGE);
}


void pumpStatusChanged() {
  isNotificationPending = true;
}

void loop() {
  SystemSleepResult result = System.sleep(config);

  if(isNotificationPending) {
    Particle.publish(getPumpStatusString());
    Particle.publish("pushbullet", getPumpStatusString());
    isNotificationPending = false;
  }
}

bool getPumpStatus() {
  return !digitalRead(D5);
}

String getPumpStatusString() {
  bool pin = getPumpStatus();
  String status = "pump changed.  Running: ";
  if(pin) {
    status += "true";
  } else {
    status += "false";
  }
  return status;
}