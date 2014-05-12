// #ifndef ACCELGYRO_H
// #define ACCELGYRO_H
//
// #include "MPU6050_6Axis_MotionApps20.h"
//
// #include "globals.h"
//
// class AccelGyro
// {
// public:
//     AccelGyro();
//     ~AccelGyro();
//
//     void init();
//     void run();
//     bool isMoving();
//
// private:
//     static MPU6050 s_MPU;
//     static bool s_Initialized;
//
//     const static int c_XGyroOffset = -40;
//     const static int c_YGyroOffset = 76;
//     const static int c_ZGyroOffset = 45;
//     const static int c_ZAccelOffset = 1220;
//
//     // MPU control/status vars
//     static bool dmpReady;  // set true if DMP init was successful
//     static uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
//     static uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
//     static uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
//     static uint16_t fifoCount;     // count of all bytes currently in FIFO
//     static uint8_t fifoBuffer[64]; // FIFO storage buffer
//
// // orientation/motion vars
//     static Quaternion q;           // [w, x, y, z]         quaternion container
//     static VectorInt16 aa;         // [x, y, z]            accel sensor measurements
//     static VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
//     static VectorInt16 s_DiffAAReal;
//     static VectorInt16 s_LastAAReal;
//     static VectorInt16 s_AvgAAReal;
//     static VectorInt16 s_PrevAAReal[4];
//     static byte s_PrevAARealIndex;
// //     static VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
//     static VectorFloat gravity;    // [x, y, z]            gravity vector
// //     static float euler[3];         // [psi, theta, phi]    Euler angle container
// //     static float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
// };
//
// #endif // ACCELGYRO_H
