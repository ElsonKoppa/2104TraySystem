# Auto-Stabilizer (Tray system)

## Pre-Requisite
Ensure CCS and compiler versions are the most up to date
Updated version of SIMPLELINK MSP432 SDK must be installed.

## If you have not installed MSP432 SDK
1) Download and install SIMPLELINK MSP432 SDK from resource explorer.
2) Click "yes" when they prompt to install SIMPLELINK SDK Academy.

## Steps to set up and build project:
1) Download our submission zip file and unzip the contents into a folder X.
2) Launch CCS (Only version 9.0 and above)
3) Click File>Switch Workspace>Other and create a new workspace.
4) Once in the new workspace, click file>import>Code composer studio, CCS Project
5) At "Select search-directory", click browse and choose the first file that you unzipped into folder X. Tick the discovered project and then click "Finish".
6) Repeat Step 4 and this time import the other file contained in folder X.
7) Build the project "servo_mpu6050_tirtos_ccs"
8) Flash it into MSP432 with the current set up of MPU6050 and the servo motor.
9) Wait for up to 10 seconds for the MPU and the Servo motor to be set up, and when the tray starts moving, means the project have been successfully set up.



