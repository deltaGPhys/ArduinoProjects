
// Includes sensor code from:
// I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class using DMP (MotionApps v2.0)
// 6/21/2012 by Jeff Rowberg <jeff@rowberg.net>

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

MPU6050 mpu;

// uncomment "OUTPUT_READABLE_YAWPITCHROLL" if you want to see the yaw/
// pitch/roll angles (in degrees) calculated from the quaternions coming
// from the FIFO. Note this also requires gravity vector calculations.
// Also note that yaw/pitch/roll angles suffer from gimbal lock (for
// more info, see: http://en.wikipedia.org/wiki/Gimbal_lock)
#define OUTPUT_READABLE_YAWPITCHROLL

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

float angle; // store the current tilt angle
float calibdata; // store sum of calibration values 
float output; // motor output
int calibnum = 0; // calibration counter
float zeropoint = 0; //offset for gyro angle
boolean calibration = false; // calibration flag
int calibPin = 8; 
int calibDonePin = 9;

float amp; // motor amplitude
int leftcal = 1509; // zero for the left motor
int rightcal = 1504; // zero for the right motor

// Store the integral and derivative values, store the last error value to
// help calculate the derivative
float integral = 0;
float derivative = 0;
float lasterror = 0;
float error = 0;
// PID constants
float Kp = .2;
float Ki = 0;
float Kd = .05;

// Potentiometer settings
int KpPin = A0;
int KiPin = A1;
int KdPin = A2;
// maximum values for the coefficients
float KpMax = 1; 
float KiMax = .3;
float KdMax = .3;

#include <Servo.h>                           // Include servo library
 
Servo servoRight;                            // Declare right servo
Servo servoLeft;

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}


// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (115200 chosen because it is required for Teapot Demo output, but it's
    // really up to you depending on your project)
    Serial.begin(115200);
    while (!Serial); // wait for Leonardo enumeration, others continue immediately

    // NOTE: 8MHz or slower host processors, like the Teensy @ 3.3v or Ardunio
    // Pro Mini running at 3.3v, cannot handle this baud rate reliably due to
    // the baud timing being too misaligned with processor ticks. You must use
    // 38400 or slower in these cases, or use some kind of external separate
    // crystal solution for the UART timer.

    // initialize device
    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // wait for ready
    Serial.println(F("\nSend any character to begin DMP programming and demo: "));
    while (Serial.available() && Serial.read()); // empty buffer
    //while (!Serial.available());                 // wait for data
    while (Serial.available() && Serial.read()); // empty buffer again

    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

    // configure LEDs for output
    pinMode(calibPin, OUTPUT);
    pinMode(calibDonePin, OUTPUT);

    // Servo setup
    servoRight.attach(10);                     // Attach right signal to pin 10
    servoRight.writeMicroseconds(rightcal);        // 1.5 ms: stay still signal

    servoLeft.attach(11);                     // Attach left signal to pin 11
    servoLeft.writeMicroseconds(leftcal);        // 1.5 ms: stay still signal

    // Potentiometer inputs
    pinMode(KpPin, INPUT);
    pinMode(KiPin, INPUT);
    pinMode(KdPin, INPUT);

}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
    // if programming failed, don't try to do anything
    if (!dmpReady) return;
    unsigned long StartTime = micros();

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            // Serial.print("angle");
            // Serial.print("\t");
            //Serial.print(ypr[1] * 180/M_PI);
            //Serial.print(", ");
           angle = ypr[1] * 180/M_PI;


           // calibration loop, to let sensor settle down and to 
           // set a zero point
           if (calibration == false){
              // red LED to indicate calibration activity
              digitalWrite(calibPin,true);
              
              calibnum = calibnum + 1;
              if (calibnum > 1400){
                // + green LED to indicate we're in second stage
                digitalWrite(calibDonePin,true);
                calibdata = calibdata + angle;
                zeropoint = calibdata/(calibnum-1400); 
                //Serial.println(angle);

                }
              if (calibnum > 2000){
                
                calibration = true;
                Serial.print("calibration angle: ");
                Serial.println(zeropoint);
                
              }
            }
            else{
              // green only after calibration is complete
              digitalWrite(calibPin,false);
              digitalWrite(calibDonePin,true);
              
              angle = angle - zeropoint;
            }
            
            
        #endif

 
        
        if (calibration == true){
          // read constant values from potentiometers
          Kp = map(analogRead(KpPin),0,1024,0,1000.0*KpMax)*1.163/1000.0;
          Ki = map(analogRead(KiPin),0,1024,0,1000.0*KiMax)*1.163/1000.0;
          Kd = map(analogRead(KdPin),0,1024,0,1000.0*KdMax)*1.163/1000.0;
          
          // calculate motor amplitude
          amp = -200 * angle * Kp + Ki * integral + Kd * derivative;
          amp = min(amp,200);

          // control servos
          servoLeft.writeMicroseconds(amp+leftcal);
          servoRight.writeMicroseconds(rightcal-amp);
          Serial.print("Tilt angle: ");
          Serial.print(angle);
          Serial.print(", Output: ");
          Serial.print(amp);
          Serial.print(", Kp: ");
          Serial.print(map(analogRead(KpPin),0,1024,0,1000.0*KpMax)*1.163/1000.0);
          Serial.print(", Ki: ");
          Serial.print(map(analogRead(KiPin),0,1024,0,1000.0*KiMax)*1.163/1000.0);
          Serial.print(", Kd: ");
          Serial.println(map(analogRead(KdPin),0,1024,0,1000.0*KdMax)*1.163/1000.0);
 
          // Calculate current error, integral, derivative, motor speed
          error = angle;
          
          unsigned long CurrentTime = micros();
          float ElapsedTime = float(CurrentTime - StartTime);
          integral = ElapsedTime*error/1000000 + integral;
        
          derivative = (error-lasterror)/(ElapsedTime);
          
          // This error is the next loop's previous error
          lasterror = error;
        }
        
    }
}
