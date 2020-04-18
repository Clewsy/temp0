EESchema Schematic File Version 4
LIBS:temp0_schematic-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L temp0_parts:ESP-01 U2
U 1 1 5E805EF5
P 2850 5000
F 0 "U2" H 2850 4750 50  0000 C CNN
F 1 "ESP-01" H 2850 5250 50  0000 C CNN
F 2 "temp0_parts:ESP-01" H 2850 5000 50  0001 C CNN
F 3 "" H 2850 5000 50  0001 C CNN
	1    2850 5000
	1    0    0    -1  
$EndComp
$Comp
L temp0_parts:hdc1080 U4
U 1 1 5E8077A0
P 7550 4950
F 0 "U4" H 7500 4950 50  0000 L CNN
F 1 "hdc1080" H 7400 5050 50  0000 L CNN
F 2 "temp0_parts:hdc1080" H 7550 4950 50  0001 C CNN
F 3 "" H 7550 4950 50  0001 C CNN
	1    7550 4950
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push RST1
U 1 1 5E8093DC
P 2300 2550
F 0 "RST1" H 2300 2500 50  0000 C CNN
F 1 "SW_Push" H 2300 2744 50  0000 C CNN
F 2 "Button_Switch_SMD:SW_Push_1P1T_NO_6x6mm_H9.5mm" H 2300 2750 50  0001 C CNN
F 3 "~" H 2300 2750 50  0001 C CNN
	1    2300 2550
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:NCP1117-3.3_SOT223 U5
U 1 1 5E80BA42
P 6300 2700
F 0 "U5" H 6300 2942 50  0000 C CNN
F 1 "3.3V_REG" H 6300 2851 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 6300 2900 50  0001 C CNN
F 3 "http://www.onsemi.com/pub_link/Collateral/NCP1117-D.PDF" H 6400 2450 50  0001 C CNN
	1    6300 2700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5E80CD31
P 5850 2900
F 0 "C1" H 5965 2946 50  0000 L CNN
F 1 "10uF" H 5965 2855 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5888 2750 50  0001 C CNN
F 3 "~" H 5850 2900 50  0001 C CNN
	1    5850 2900
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5E80D1FA
P 6650 2900
F 0 "C2" H 6765 2946 50  0000 L CNN
F 1 "10uF" H 6765 2855 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 6688 2750 50  0001 C CNN
F 3 "~" H 6650 2900 50  0001 C CNN
	1    6650 2900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5E8109E1
P 6300 3150
F 0 "#PWR0101" H 6300 2900 50  0001 C CNN
F 1 "GND" H 6305 2977 50  0000 C CNN
F 2 "" H 6300 3150 50  0001 C CNN
F 3 "" H 6300 3150 50  0001 C CNN
	1    6300 3150
	1    0    0    -1  
$EndComp
Text GLabel 2750 1650 0    50   Input ~ 0
5V
Text GLabel 5800 2700 0    50   Input ~ 0
5V
Text GLabel 6700 2700 2    50   Input ~ 0
3.3V
Text GLabel 3350 4850 2    50   Input ~ 0
3.3V
$Comp
L temp0_parts:Pro_Trinket_5V U1
U 1 1 5E803ED3
P 3450 2000
F 0 "U1" H 3450 2100 50  0000 C CNN
F 1 "Pro_Trinket_5V" H 3450 2000 50  0000 C CNN
F 2 "temp0_parts:Pro_Trinket_5V" H 3100 1900 50  0001 C CNN
F 3 "" H 3100 1900 50  0001 C CNN
	1    3450 2000
	1    0    0    -1  
$EndComp
Text GLabel 2750 2350 0    50   Input ~ 0
U1_TX
Text GLabel 2200 4350 2    50   Input ~ 0
U1_TX
Text GLabel 3300 5150 2    50   Input ~ 0
U1_RX
NoConn ~ 2400 5050
$Comp
L Device:R R3
U 1 1 5E837B90
P 2150 4950
F 0 "R3" H 2220 4996 50  0000 L CNN
F 1 "10k" H 2220 4905 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 2080 4950 50  0001 C CNN
F 3 "~" H 2150 4950 50  0001 C CNN
	1    2150 4950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 5E8383EE
P 2150 4550
F 0 "R4" H 2220 4596 50  0000 L CNN
F 1 "22k" H 2220 4505 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 2080 4550 50  0001 C CNN
F 3 "~" H 2150 4550 50  0001 C CNN
	1    2150 4550
	1    0    0    -1  
$EndComp
Text GLabel 2750 2450 0    50   Input ~ 0
U1_RX
$Comp
L power:GND #PWR0104
U 1 1 5E850B36
P 1550 2750
F 0 "#PWR0104" H 1550 2500 50  0001 C CNN
F 1 "GND" H 1555 2577 50  0000 C CNN
F 2 "" H 1550 2750 50  0001 C CNN
F 3 "" H 1550 2750 50  0001 C CNN
	1    1550 2750
	1    0    0    -1  
$EndComp
Text GLabel 4150 2450 2    50   Input ~ 0
SDA
Text GLabel 4150 2550 2    50   Input ~ 0
SCL
Text GLabel 7600 5350 3    50   Input ~ 0
SDA
Text GLabel 7500 5350 3    50   Input ~ 0
SCL
$Comp
L temp0_parts:oled_I2C_ssd1306 U3
U 1 1 5E8535DB
P 8800 2800
F 0 "U3" H 8750 2850 50  0000 L CNN
F 1 "oled_I2C_ssd1306" H 8450 3000 50  0000 L CNN
F 2 "temp0_parts:oled_i2c_ssd1306" H 8800 2800 50  0001 C CNN
F 3 "" H 8800 2800 50  0001 C CNN
	1    8800 2800
	1    0    0    -1  
$EndComp
Text GLabel 7700 5350 3    50   Input ~ 0
3.3V
$Comp
L power:GND #PWR0105
U 1 1 5E854F15
P 7400 5400
F 0 "#PWR0105" H 7400 5150 50  0001 C CNN
F 1 "GND" H 7405 5227 50  0000 C CNN
F 2 "" H 7400 5400 50  0001 C CNN
F 3 "" H 7400 5400 50  0001 C CNN
	1    7400 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 5E858B69
P 9100 3150
F 0 "#PWR0106" H 9100 2900 50  0001 C CNN
F 1 "GND" H 9105 2977 50  0000 C CNN
F 2 "" H 9100 3150 50  0001 C CNN
F 3 "" H 9100 3150 50  0001 C CNN
	1    9100 3150
	1    0    0    -1  
$EndComp
Text GLabel 8900 3150 3    50   Input ~ 0
5V
Text GLabel 8500 3150 3    50   Input ~ 0
SDA
Text GLabel 8700 3150 3    50   Input ~ 0
SCL
$Comp
L Switch:SW_Push MODE1
U 1 1 5E85CBA8
P 1800 2250
F 0 "MODE1" H 1800 2200 50  0000 C CNN
F 1 "SW_Push" H 1800 2444 50  0000 C CNN
F 2 "Button_Switch_SMD:SW_Push_1P1T_NO_6x6mm_H9.5mm" H 1800 2450 50  0001 C CNN
F 3 "~" H 1800 2450 50  0001 C CNN
	1    1800 2250
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D1
U 1 1 5E86E695
P 2600 1950
F 0 "D1" H 2600 1850 50  0000 C CNN
F 1 "LED" H 2593 2075 50  0000 C CNN
F 2 "LED_SMD:LED_PLCC-2" H 2600 1950 50  0001 C CNN
F 3 "~" H 2600 1950 50  0001 C CNN
	1    2600 1950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5E817521
P 2250 1950
F 0 "R1" H 2320 1996 50  0000 L CNN
F 1 "100" H 2320 1905 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 2180 1950 50  0001 C CNN
F 3 "~" H 2250 1950 50  0001 C CNN
	1    2250 1950
	0    1    1    0   
$EndComp
NoConn ~ 2800 1850
NoConn ~ 2800 1750
NoConn ~ 2800 1450
NoConn ~ 2800 2050
NoConn ~ 2800 2150
NoConn ~ 4100 2350
NoConn ~ 4100 2250
NoConn ~ 4100 2150
NoConn ~ 4100 2050
NoConn ~ 4100 1950
NoConn ~ 4100 1850
NoConn ~ 4100 1750
NoConn ~ 4100 1650
NoConn ~ 4100 1550
NoConn ~ 4100 1450
$Comp
L Connector:Conn_01x02_Male J1
U 1 1 5E8088A4
P 1750 2600
F 0 "J1" V 1800 2650 50  0000 C CNN
F 1 "MODE1_EXT" V 1700 2550 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Horizontal" H 1750 2600 50  0001 C CNN
F 3 "~" H 1750 2600 50  0001 C CNN
	1    1750 2600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2400 4850 2350 4850
Wire Wire Line
	2750 2350 2800 2350
Wire Wire Line
	2800 2450 2750 2450
Wire Wire Line
	2750 1650 2800 1650
Wire Wire Line
	2750 1950 2800 1950
Wire Wire Line
	2450 1950 2400 1950
Wire Wire Line
	2500 2550 2800 2550
Wire Wire Line
	4150 2550 4100 2550
Wire Wire Line
	4100 2450 4150 2450
Wire Wire Line
	6600 2700 6650 2700
Wire Wire Line
	5800 2700 5850 2700
Wire Wire Line
	6650 2750 6650 2700
Connection ~ 6650 2700
Wire Wire Line
	6650 2700 6700 2700
Wire Wire Line
	5850 2750 5850 2700
Connection ~ 5850 2700
Wire Wire Line
	5850 2700 6000 2700
Wire Wire Line
	5850 3050 5850 3100
Wire Wire Line
	5850 3100 6300 3100
Wire Wire Line
	6650 3100 6650 3050
Wire Wire Line
	6300 3000 6300 3100
Connection ~ 6300 3100
Wire Wire Line
	6300 3100 6650 3100
Wire Wire Line
	6300 3150 6300 3100
Wire Wire Line
	7500 5300 7500 5350
Wire Wire Line
	7600 5350 7600 5300
Wire Wire Line
	7700 5300 7700 5350
Wire Wire Line
	7400 5400 7400 5300
Wire Wire Line
	9100 3150 9100 3100
Wire Wire Line
	8900 3100 8900 3150
Wire Wire Line
	8700 3150 8700 3100
Wire Wire Line
	8500 3100 8500 3150
Wire Notes Line
	4400 1250 4400 3000
Text Notes 3050 2950 0    50   ~ 0
Pro Trinket Module (Atemga328P)
Text Notes 3500 4550 2    50   ~ 0
ESP8266 Module (ESP-01)
Text Notes 5550 3600 0    50   ~ 0
3.3V Power Regulator\nInput: 5V from USB (via Pro Trinket)\nOutput 3.3V to  ESP8266 and HDC1080
Wire Notes Line
	5500 3650 5500 2400
Wire Notes Line
	5500 2400 7100 2400
Wire Notes Line
	7100 2400 7100 3650
Wire Notes Line
	7100 3650 5500 3650
Text Notes 8250 3550 0    50   ~ 0
128x64 Pixel OLED Module\nWith SSD1306 Driver in I2C Mode
Text Notes 6900 5700 0    50   ~ 0
HDC1080\nTemperature and Humidity Sensor
Wire Notes Line
	6850 5750 6850 4750
Wire Notes Line
	6850 4750 8250 4750
Wire Notes Line
	8250 4750 8250 5750
Wire Notes Line
	8250 5750 6850 5750
Text Notes 7400 7500 0    79   ~ 0
temp0 schematic
Text Notes 8150 7650 0    79   ~ 0
20200418
Text Notes 10550 7650 0    79   ~ 0
2
Wire Notes Line
	8200 3600 8200 2500
Wire Notes Line
	8200 2500 9550 2500
Wire Notes Line
	9550 2500 9550 3600
Wire Notes Line
	9550 3600 8200 3600
Wire Wire Line
	2100 2550 2050 2550
Wire Wire Line
	2050 2550 2050 2700
Wire Wire Line
	2050 2700 1550 2700
Wire Wire Line
	1550 2700 1550 2750
Wire Wire Line
	1550 2250 1550 2350
Connection ~ 1550 2700
Wire Wire Line
	2000 2250 2050 2250
Wire Wire Line
	1600 2250 1550 2250
Wire Wire Line
	2800 1550 1550 1550
Wire Wire Line
	1550 1550 1550 1950
Connection ~ 1550 2250
Wire Wire Line
	2100 1950 1550 1950
Connection ~ 1550 1950
Wire Wire Line
	1550 1950 1550 2250
Wire Notes Line
	4400 3000 1450 3000
Wire Notes Line
	1450 3000 1450 1250
Wire Notes Line
	1450 1250 4400 1250
Wire Wire Line
	1850 2400 1850 2350
Wire Wire Line
	1850 2350 2050 2350
Wire Wire Line
	2050 2350 2050 2250
Connection ~ 2050 2250
Wire Wire Line
	2050 2250 2800 2250
Wire Wire Line
	1750 2400 1750 2350
Wire Wire Line
	1750 2350 1550 2350
Connection ~ 1550 2350
Wire Wire Line
	1550 2350 1550 2700
NoConn ~ 2400 4950
$Comp
L power:GND #PWR0103
U 1 1 5E82DB9D
P 2150 5200
F 0 "#PWR0103" H 2150 4950 50  0001 C CNN
F 1 "GND" H 2155 5027 50  0000 C CNN
F 2 "" H 2150 5200 50  0001 C CNN
F 3 "" H 2150 5200 50  0001 C CNN
	1    2150 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 5150 2150 5150
Wire Wire Line
	2150 5200 2150 5150
Wire Wire Line
	3250 5050 3300 5050
Wire Wire Line
	3300 5050 3300 4950
Wire Wire Line
	3300 4850 3350 4850
Wire Wire Line
	3250 4950 3300 4950
Connection ~ 3300 4950
Wire Wire Line
	3300 4950 3300 4850
Wire Wire Line
	3250 4850 3300 4850
Connection ~ 3300 4850
Text Notes 7000 7000 0    50   ~ 0
Changes from Rev: 1\n- Removed ftdi header for programming ESP8266.\n- Removed ESP8266 program button\n- Removed ESP8266 reset button
Wire Wire Line
	2150 5150 2150 5100
Connection ~ 2150 5150
Wire Wire Line
	2350 4850 2350 4750
Wire Wire Line
	2350 4750 2150 4750
Wire Wire Line
	2150 4750 2150 4800
Wire Wire Line
	2150 4700 2150 4750
Connection ~ 2150 4750
Wire Wire Line
	2200 4350 2150 4350
Wire Wire Line
	2150 4350 2150 4400
Wire Wire Line
	3250 5150 3300 5150
Wire Notes Line
	2050 4250 2050 5450
Wire Notes Line
	2050 5450 3650 5450
Wire Notes Line
	3650 5450 3650 4250
Wire Notes Line
	3650 4250 2050 4250
$EndSCHEMATC
