Flash the file eclipse-workspace/ArduinoByteTransfer/uartxchange.c onto your Arduino. Then open a terminal and type:
sudo rmmod cdc_acm
to remove the Arduino firmware interface (used for flashing code) and clear up communication line. Then run
sudo python3 PcSetupUart.py
In this case, you can send bytes to the Arduino and if the byte is odd, it turns on the Arduino Uno LED; if it's even, it turns LED off.
