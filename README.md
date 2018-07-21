# I-Palmaid
  
    For Exceed Camp 2018


## Table of Contents
**[Stepper Motor with L9110](#stepper-motor-with-l9110)**<br>

## Stepper Motor with L9110

### Code: [**../Hardware/motor_drive_L9110/motor_drive_L9110.ino**](https://github.com/skconan/I-Palmaid/blob/master/Hardware/motor_drive_L9110/motor_drive_L9110.ino)
* ต่อมอเตอร์ทั้ง 2 ตัว เข้ากับช่อง MOTOR A และ MOTOR B (ยังไม่ต้องสนใจทิศทาง)
* อีกด้านของ board drive L9110 จะมี 6 ช่อง ให้เสียบ VCC กับ GND ไปก่อน 
* คราวนี้จะเหลือ ช่อง B-1A, B-1B และ A-1A, A-1B
* เราจะให้มอเตอร์ด้านซ้ายเป็น B เราก็ Port digital เลือกมา 2 ช่อง จากโค้ดเลือก Digital 7 กับ 3 
* ลองสั่งให้มอเตอร์หมุน โดยใช้คำสั่ง step(stepsPerRevolution); ตอนนี้ stepsPerRevolution เป็น `+` ก็ควรจะหมุนไปข้างหน้าไม่ไม่ไปข้างหน้าให้สลับสาย
* ทำแบบนี้กับอีกมอเตอร์
* คราวนี้เราจะมาทำให้มันเลี้ยวขวา เลี้ยวซ้ายกัน เราก็สร้างตัว stepper สำหรับเลี้ยวซ้าย ขวา มา โดยการสลับ port จะให้มอเตอร์หมุนถิยหลังข้างนึง ีกข้างนึงเดินหน้า
