#include <Bluepad32.h>
#include <SCServo.h>

// ---------- CONFIG: edit this section for your arm ----------

// UART pins used to talk to the servo bus.
#define SERVO_RX_PIN 18
#define SERVO_TX_PIN 19

// Servo IDs on the bus, one entry per joint.
enum JointIndex { BASE = 0,
                  JOINT_ONE,
                  JOINT_TWO,
                  JOINT_THREE,
                  JOINT_FOUR,
                  NUM_JOINTS };
uint8_t servoId[NUM_JOINTS] = { 1, 2, 3, 4, 5 };

// Soft limits (servo position units, 0-4095 on most STS3215 setups).
// Tune these per joint so the arm can't drive itself into its own frame.
int minPos[NUM_JOINTS] = { 1024, 0, 1031, 475, 1275 };
int maxPos[NUM_JOINTS] = { 3072, 2658, 4000, 2278, 2441 };

// Starting/home position for each joint - set to wherever is safe to power on into.
int homePos[NUM_JOINTS] = { 3000, 3000, 3000, 3000, 3000 };

// How fast a full stick deflection moves a joint, in position-units per update.
int stepSize = 30;

// Servo motion speed/acceleration passed to WritePosEx (tune to taste).
uint16_t servoSpeed = 2000;
uint8_t servoAcc = 80;

// --------------------------------------------------------------

SMS_STS st;
ControllerPtr myController = nullptr;
int currentPos[NUM_JOINTS];
bool armEnabled = false;  // start disarmed as a safety default

void onConnectedController(ControllerPtr ctl) {
  Serial.println("Controller connected");
  j
    myController = ctl;
}

void onDisconnectedController(ControllerPtr ctl) {
  Serial.println("Controller disconnected");
  if (myController == ctl) myController = nullptr;
}

void setup() {
  Serial.begin(115200);

  // Bring up the servo bus
  Serial1.begin(1000000, SERIAL_8N1, SERVO_RX_PIN, SERVO_TX_PIN);
  st.pSerial = &Serial1;

  // Move every joint to its home position on boot
  for (int i = 0; i < NUM_JOINTS; i++) {
    currentPos[i] = homePos[i];
    st.WritePosEx(servoId[i], homePos[i], servoSpeed, servoAcc);
  }

  // Bring up Bluepad32 and start scanning for a controller
  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys();  // remove if you want it to auto-reconnect to the last controller
  Serial.println("Ready - put your Xbox controller into pairing mode.");
}

// Clamp a value between lo and hi
int clampInt(int v, int lo, int hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

void processController(ControllerPtr ctl) {
  // Bluepad32 stick axes are roughly -512..512, triggers 0..1023.
  int lx = ctl->axisX();     // left stick X  -> base rotation
  int ly = ctl->axisY();     // left stick Y  -> joint one
  int rx = ctl->axisRX();    // right stick X -> joint four
  int ry = ctl->axisRY();    // right stick Y -> joint two
  int lt = ctl->brake();     // left trigger  -> joint three (down)
  int rt = ctl->throttle();  // right trigger -> joint three (up)

  // Right bumper = arm/disarm toggle (edge-triggered)
  static bool lastR1 = false;
  bool r1 = ctl->r1();
  if (r1 && !lastR1) {
    armEnabled = !armEnabled;
    Serial.println(armEnabled ? "ARMED" : "DISARMED");
  }
  lastR1 = r1;

  if (!armEnabled) return;  // ignore movement input while disarmed

  // // Deadzone so idle sticks don't cause drift
  // const int deadzone = 60;
  // auto applyDeadzone = [&](int v) {
  //   return (abs(v) < deadzone) ? 0 : v;
  // };
  // lx = applyDeadzone(lx);
  // ly = applyDeadzone(ly);
  // rx = applyDeadzone(rx);
  // ry = applyDeadzone(ry);

  // Map stick deflection to an incremental step per loop (feels like velocity control)
  currentPos[BASE] += (lx * stepSize) / 512;
  currentPos[JOINT_ONE] += (ly * stepSize) / 512;
  currentPos[JOINT_TWO] += (ry * stepSize) / 512;
  currentPos[JOINT_FOUR] += (rx * stepSize) / 512;
  currentPos[JOINT_THREE] += ((rt - lt) * stepSize) / 1023;

  // Clamp everything to soft limits, then send to servos
  for (int i = 0; i < NUM_JOINTS; i++) {
    currentPos[i] = clampInt(currentPos[i], minPos[i], maxPos[i]);
    st.WritePosEx(servoId[i], currentPos[i], servoSpeed, servoAcc);
  }
}

void loop() {
  bool dataUpdated = BP32.update();
  if (dataUpdated && myController && myController->isConnected()) {
    processController(myController);
  }
  delay(20);  // ~50Hz update rate
}
