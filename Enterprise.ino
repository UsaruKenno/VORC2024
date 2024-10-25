#include <AFMotor.h>
#include <Servo.h>
#include <PS2X_lib.h>
#include <Adafruit_TCS34725.h>

#define PS2_DAT        A5      
#define PS2_CMD        A4  
#define PS2_SEL        A3  
#define PS2_CLK        A2

#define pressures   false
#define rumble      false

// Khai báo các chân cho động cơ và servo
AF_DCMotor dbl(1);  // Động cơ DC 12V 1 (Driver Base Trái)
AF_DCMotor dbr(2);  // Động cơ DC 12V 2 (Driver Base Phải)
AF_DCMotor IntakeOutake(3);  // Động cơ DC 12V 3 (cơ chế thu hoạch)
AF_DCMotor ExpansionReduction(4);  // Động cơ DC 12V 4 (Shooter)

// Khai báo PS2 controller
PS2X ps2x; 

int error = 0;
byte type = 0;
byte vibrate = 0;

// Hàm setup
void setup() {
  Serial.begin(9600);

  // Khởi tạo PS2 controller
  ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  ps2x.read_gamepad();

  // Khởi tạo servo
  angle.attach(9);
  doorw.attach(10);
  sorter.attach(11);
  doorb.attach(12);

  // Đặt các giá trị mặc định cho servo
  angle.write(0);
  doorw.write(0);
  sorter.write(0);
  doorb.write(0);
}

// Chạy chương trình
void loop() {
  Ingame();
}

// Tiến
void Forward(long speed) {
  dbl.setSpeed(speed);
  dbr.setSpeed(speed);
  dbl.run(FORWARD);
  dbr.run(FORWARD);
}

// Lùi
void Backward(long speed) {
  dbl.setSpeed(speed);
  dbr.setSpeed(speed);
  dbl.run(BACKWARD);
  dbr.run(BACKWARD);
}

// Xoay trái
void Turn_left(long speed) {
  dbl.setSpeed(speed);
  dbr.setSpeed(speed);
  dbl.run(BACKWARD);
  dbr.run(FORWARD);
}

// Xoay phải
void Turn_right(long speed) {
  dbl.setSpeed(speed);
  dbr.setSpeed(speed);
  dbl.run(FORWARD);
  dbr.run(BACKWARD);
}

// Tiến bên trái
void One_left_up(long speed) {
  dbr.setSpeed(speed);
  dbr.run(FORWARD);
}

// Tiến bên phải
void One_right_up(long speed) {
  dbl.setSpeed(speed);
  dbl.run(FORWARD);
}

// Không di chuyển
void Stop() {
  dbl.setSpeed(0);
  dbr.setSpeed(0);
  dbl.run(RELEASE);
  dbr.run(RELEASE);
}

void Ingame() {
  ps2x.read_gamepad(false, vibrate); // Đọc trạng thái nút

  // Điều khiển Intake và Outake
  if (ps2x.Button(PSB_SQUARE)) {
    IntakeOutake.setSpeed(255);
    IntakeOutake.run(FORWARD); // IntakeOutake hoạt động
  } 
  if (ps2x.Button(PSB_CIRCLE)) {
    IntakeOutake.setSpeed(255);
    IntakeOutake.run(BACKWARD); // Outake hoạt động
  } 

  // Điều khiển các motor mở cửa thùng chứa
  if (ps2x.Button(PSB_L1)) {
    ExpansionReduction.write(90);  // Mở cửa thùng chứa
  } else if (ps2x.Button(PSB_L2)) {
    ExpansionReduction.write(0); // Đóng cửa thùng chứa
  }
}
