// #include "accelgyro.h"
//
// MPU6050 s_MPU(0x69);
// bool AccelGyro::s_Initialized;
//
// //     const   int AccelGyro::c_XGyroOffset = -40;
// //     const   int AccelGyro::c_YGyroOffset = 76;
// //     const   int AccelGyro::c_ZGyroOffset = 45;
// //     const   int AccelGyro::c_ZAccelOffset = 1220;
//
// // MPU control/status vars
// bool AccelGyro::dmpReady = false;  // set true if DMP init was successful
// uint8_t AccelGyro::mpuIntStatus;   // holds actual interrupt status byte from MPU
// uint8_t AccelGyro::devStatus;      // return status after each device operation (0 = success, !0 = error)
// uint16_t AccelGyro::packetSize;    // expected DMP packet size (default is 42 bytes)
// uint16_t AccelGyro::fifoCount;     // count of all bytes currently in FIFO
// uint8_t AccelGyro::fifoBuffer[64]; // FIFO storage buffer
//
// // orientation/motion vars
// Quaternion AccelGyro::q;           // [w, x, y, z]         quaternion container
// VectorInt16 AccelGyro::aa;         // [x, y, z]            accel sensor measurements
// VectorInt16 AccelGyro::aaReal;     // [x, y, z]            gravity-free accel sensor measurements
// VectorInt16 AccelGyro::s_DiffAAReal;
// VectorInt16 AccelGyro::s_LastAAReal;
// VectorInt16 AccelGyro::s_AvgAAReal;
// VectorInt16 AccelGyro::s_PrevAAReal[4];
// byte AccelGyro::s_PrevAARealIndex;
// VectorFloat AccelGyro::gravity;    // [x, y, z]            gravity vector
//
//
// AccelGyro::AccelGyro()
// {
//
// }
//
// AccelGyro::~AccelGyro()
// {
//
// }
//
// void AccelGyro::init()
// {
//     if (s_Initialized == false) {
// //         s_MPU.initialize();
// //
// //         devStatus = s_MPU.dmpInitialize();
// //
// //         s_MPU.setXGyroOffset(c_XGyroOffset);
// //         s_MPU.setYGyroOffset(c_YGyroOffset);
// //         s_MPU.setZGyroOffset(c_ZGyroOffset);
// //         s_MPU.setZAccelOffset(c_ZAccelOffset);
// //
// //         // make sure it worked (returns 0 if so)
// //         if (devStatus == 0) {
// //             // turn on the DMP, now that it's ready
// //             Serial.println(F("Enabling DMP..."));
// //             s_MPU.setDMPEnabled(true);
// //
// //             // enable Arduino interrupt detection
// //             //Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
// //             //attachInterrupt(0, dmpDataReady, RISING);
// //             mpuIntStatus = s_MPU.getIntStatus();
// //
// //             // set our DMP Ready flag so the main loop() function knows it's okay to use it
// //             Serial.println(F("DMP ready! Waiting for first interrupt..."));
// //             dmpReady = true;
// //
// //             // get expected DMP packet size for later comparison
// //             packetSize = s_MPU.dmpGetFIFOPacketSize();
// //         } else {
// //             // ERROR!
// //             // 1 = initial memory load failed
// //             // 2 = DMP configuration updates failed
// //             // (if it's going to break, usually the code will be 1)
// //             Serial.print(F("DMP Initialization failed (code "));
// //             Serial.print(devStatus);
// //             Serial.println(F(")"));
// //             while (1);
// //         }
// //
// //         //s_MPU.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
// //         //s_MPU.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
//     }
// }
//
// void AccelGyro::run()
// {
//     while (1) {
//         /*
//           if (!dmpReady) return;
//
//           // wait for MPU interrupt or extra packet(s) available
//           //while (!mpuInterrupt && fifoCount < packetSize) {}
//
//           mpuIntStatus = s_MPU.getIntStatus();
//
//           // get current FIFO count
//           fifoCount = s_MPU.getFIFOCount();
//
//           // check for overflow (this should never happen unless our code is too inefficient)
//           if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
//               // reset so we can continue cleanly
//               s_MPU.resetFIFO();
//               Serial.println(F("FIFO overflow!"));
//
//               // otherwise, check for DMP data ready interrupt (this should happen frequently)
//           } else if (mpuIntStatus & 0x02) {
//               // wait for correct available data length, should be a VERY short wait
//               while (fifoCount < packetSize) fifoCount = s_MPU.getFIFOCount();
//
//               // read a packet from FIFO
//               s_MPU.getFIFOBytes(fifoBuffer, packetSize);
//
//               // track FIFO count here in case there is > 1 packet available
//               // (this lets us immediately read more without waiting for an interrupt)
//               fifoCount -= packetSize;
//
//               // display real acceleration, adjusted to remove gravity
//               s_MPU.dmpGetQuaternion(&q, fifoBuffer);
//               s_MPU.dmpGetAccel(&aa, fifoBuffer);
//               s_MPU.dmpGetGravity(&gravity, &q);
//               s_MPU.dmpGetLinearAccel(&aaReal, &aa, &gravity);
//
//               s_DiffAAReal.x = aaReal.x - s_LastAAReal.x;
//               s_DiffAAReal.y = aaReal.y - s_LastAAReal.y;
//               s_DiffAAReal.z = aaReal.z - s_LastAAReal.z;
//               s_LastAAReal = aaReal;
//
//               s_PrevAAReal[s_PrevAARealIndex % 4] = s_DiffAAReal;
//               s_PrevAARealIndex++;
//
//               for (int i = 0; i < 4; i++) {
//                   s_AvgAAReal.x += s_PrevAAReal[i].x;
//                   s_AvgAAReal.y += s_PrevAAReal[i].y;
//                   s_AvgAAReal.z += s_PrevAAReal[i].z;
//               }
//
//               Serial.print("areal\t");
//               Serial.print(s_AvgAAReal.x);
//               Serial.print("\t");
//               Serial.print(s_AvgAAReal.y);
//               Serial.print("\t");
//               Serial.println(s_AvgAAReal.z);
//
//         // #ifdef OUTPUT_READABLE_WORLDACCEL
//         //         // display initial world-frame acceleration, adjusted to remove gravity
//         //         // and rotated based on known orientation from quaternion
//         //         s_MPU.dmpGetQuaternion(&q, fifoBuffer);
//         //         s_MPU.dmpGetAccel(&aa, fifoBuffer);
//         //         s_MPU.dmpGetGravity(&gravity, &q);
//         //         s_MPU.dmpGetLinearAccel(&aaReal, &aa, &gravity);
//         //         s_MPU.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
//         //         Serial.print("aworld\t");
//         //         Serial.print(aaWorld.x);
//         //         Serial.print("\t");
//         //         Serial.print(aaWorld.y);
//         //         Serial.print("\t");
//         //         Serial.println(aaWorld.z);
//         // #endif
//           }
//           */
//     }
// }
//
// bool AccelGyro::isMoving()
// {
//
// }
