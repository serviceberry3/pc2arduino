Flash the file eclipse-workspace/ArduinoByteTransfer/uartxchange.c onto your Arduino. Then open a terminal and type:
sudo rmmod cdc_acm
to remove the Arduino firmware interface (used for flashing code) and clear up communication line.
Then run
sudo python3 PcSetupUart.py
