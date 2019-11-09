EESchema Schematic File Version 4
LIBS:SegaControllerForApple2-1.4-cache
EELAYER 29 0
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
L Connector:Conn_01x04_Female J3(OLED)1
U 1 1 5CF70799
P 7050 4700
F 0 "J3(OLED)1" H 7158 4981 50  0000 C CNN
F 1 "Conn_01x04_Female" H 7158 4890 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 7050 4700 50  0001 C CNN
F 3 "~" H 7050 4700 50  0001 C CNN
	1    7050 4700
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J2(Apple2)1
U 1 1 5D195A9C
P 3250 2350
F 0 "J2(Apple2)1" H 3300 2767 50  0000 C CNN
F 1 "Conn_02x05_Odd_Even" H 3300 2676 50  0000 C CNN
F 2 "Connector_IDC:IDC-Header_2x05_P2.54mm_Horizontal" H 3250 2350 50  0001 C CNN
F 3 "~" H 3250 2350 50  0001 C CNN
	1    3250 2350
	1    0    0    -1  
$EndComp
$Comp
L MCU_Module:Arduino_Nano_v3.x U1
U 1 1 5D189CF3
P 8750 3100
F 0 "U1" H 8750 2011 50  0000 C CNN
F 1 "Arduino_Nano_v3.x" H 8750 1920 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 8900 2150 50  0001 L CNN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 8750 2100 50  0001 C CNN
	1    8750 3100
	-1   0    0    -1  
$EndComp
Wire Wire Line
	10950 4150 10450 4150
Wire Wire Line
	8750 4150 8750 4100
NoConn ~ 7400 3400
Wire Wire Line
	7950 4700 7950 1850
Wire Wire Line
	8650 4600 8650 4150
Wire Wire Line
	8000 4800 8000 3600
Wire Wire Line
	8000 3600 8250 3600
Wire Wire Line
	8250 3500 8050 3500
Wire Wire Line
	8050 3500 8050 4900
Wire Wire Line
	7350 2550 7700 2550
Wire Wire Line
	7700 2550 7700 3600
Wire Wire Line
	7700 4450 9550 4450
Wire Wire Line
	9550 4450 9550 3500
Wire Wire Line
	9550 3500 9250 3500
Wire Wire Line
	7350 2250 7850 2250
Wire Wire Line
	7850 2250 7850 4350
Wire Wire Line
	7850 4350 9250 4350
Wire Wire Line
	9250 4350 9250 3800
Wire Wire Line
	7350 2350 7600 2350
Wire Wire Line
	7600 2350 7600 4550
Wire Wire Line
	7600 4550 9350 4550
Wire Wire Line
	9350 4550 9350 3600
Wire Wire Line
	9350 3600 9250 3600
Wire Wire Line
	6850 3050 6850 4150
Wire Wire Line
	6850 4150 8650 4150
Connection ~ 8650 4150
Wire Wire Line
	8650 4150 8650 4100
Wire Wire Line
	6350 2350 6350 2250
Wire Wire Line
	6350 2750 6350 2650
Wire Wire Line
	6850 2050 6850 1850
Wire Wire Line
	6850 1850 7450 1850
Connection ~ 7950 1850
Wire Wire Line
	6350 2450 6150 2450
Wire Wire Line
	6150 2450 6150 1850
Wire Wire Line
	6150 1850 6850 1850
Connection ~ 6850 1850
Wire Wire Line
	6350 2850 6150 2850
Wire Wire Line
	6150 2850 6150 2450
Connection ~ 6150 2450
Wire Wire Line
	7350 2850 7350 2750
Wire Wire Line
	7350 2750 7450 2750
Wire Wire Line
	7450 2750 7450 1850
Connection ~ 7450 1850
Wire Wire Line
	7450 1850 7950 1850
Wire Wire Line
	8850 2100 8850 1850
Connection ~ 8850 1850
Wire Wire Line
	7950 1850 8850 1850
Wire Wire Line
	8650 4150 8750 4150
Connection ~ 8750 4150
NoConn ~ 8650 2100
NoConn ~ 8550 2100
NoConn ~ 8250 2500
NoConn ~ 8250 2600
NoConn ~ 8250 2900
NoConn ~ 8250 3700
NoConn ~ 9250 2600
NoConn ~ 9250 2500
NoConn ~ 8250 3300
NoConn ~ 8250 3400
Wire Wire Line
	3550 2150 3550 1850
Wire Wire Line
	3550 1850 5650 1850
Connection ~ 6150 1850
NoConn ~ 3550 2250
Wire Wire Line
	3050 2250 2450 2250
Wire Wire Line
	2450 2250 2450 4150
Wire Wire Line
	3050 2350 2550 2350
NoConn ~ 3050 2550
NoConn ~ 3550 2350
NoConn ~ 3550 2550
$Comp
L Device:Q_PNP_EBC Q1
U 1 1 5D25B413
P 950 2650
F 0 "Q1" H 1141 2604 50  0000 L CNN
F 1 "BC557" H 1141 2695 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92" H 1150 2750 50  0001 C CNN
F 3 "~" H 950 2650 50  0001 C CNN
	1    950  2650
	-1   0    0    1   
$EndComp
Wire Wire Line
	1000 1850 1550 1850
Connection ~ 3550 1850
$Comp
L Device:R R1
U 1 1 5D279C83
P 850 3250
F 0 "R1" H 920 3296 50  0000 L CNN
F 1 "680" H 920 3205 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 780 3250 50  0001 C CNN
F 3 "~" H 850 3250 50  0001 C CNN
	1    850  3250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5D27D02C
P 1300 2650
F 0 "R2" V 1093 2650 50  0000 C CNN
F 1 "1K" V 1184 2650 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 1230 2650 50  0001 C CNN
F 3 "~" H 1300 2650 50  0001 C CNN
	1    1300 2650
	0    1    1    0   
$EndComp
Wire Wire Line
	2700 2450 3050 2450
Wire Wire Line
	1000 1850 1000 2450
Wire Wire Line
	1000 2450 850  2450
Wire Wire Line
	850  3400 850  4150
Wire Wire Line
	850  4150 1400 4150
Wire Wire Line
	2250 2650 2250 3100
Wire Wire Line
	850  2850 850  2900
Wire Wire Line
	2550 2350 2550 2950
Wire Wire Line
	850  2900 2700 2900
Wire Wire Line
	2700 2900 2700 2450
Connection ~ 850  2900
Wire Wire Line
	850  2900 850  3100
$Comp
L Device:Q_PNP_EBC Q2
U 1 1 5D29A069
P 1500 3350
F 0 "Q2" H 1691 3304 50  0000 L CNN
F 1 "BC557" H 1691 3395 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92" H 1700 3450 50  0001 C CNN
F 3 "~" H 1500 3350 50  0001 C CNN
	1    1500 3350
	-1   0    0    1   
$EndComp
$Comp
L Device:R R3
U 1 1 5D29A912
P 1400 3850
F 0 "R3" H 1470 3896 50  0000 L CNN
F 1 "680" H 1470 3805 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 1330 3850 50  0001 C CNN
F 3 "~" H 1400 3850 50  0001 C CNN
	1    1400 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1400 3550 1400 3650
Wire Wire Line
	1400 4000 1400 4150
Connection ~ 1400 4150
Wire Wire Line
	1400 4150 2450 4150
Wire Wire Line
	1400 3150 1550 3150
Wire Wire Line
	1550 3150 1550 1850
Connection ~ 1550 1850
Wire Wire Line
	1550 1850 3550 1850
$Comp
L Device:R R4
U 1 1 5D2A6E58
P 1850 3350
F 0 "R4" V 1643 3350 50  0000 C CNN
F 1 "1K" V 1734 3350 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 1780 3350 50  0001 C CNN
F 3 "~" H 1850 3350 50  0001 C CNN
	1    1850 3350
	0    1    1    0   
$EndComp
Wire Wire Line
	8050 3350 8050 3200
Wire Wire Line
	8050 3200 8250 3200
Wire Wire Line
	2000 3350 8050 3350
Wire Wire Line
	1400 3650 2150 3650
Wire Wire Line
	2150 3650 2150 2150
Wire Wire Line
	2150 2150 3050 2150
Connection ~ 1400 3650
Wire Wire Line
	1400 3650 1400 3700
$Comp
L power:GND #PWR0101
U 1 1 5D2B1429
P 10950 4150
F 0 "#PWR0101" H 10950 3900 50  0001 C CNN
F 1 "GND" H 10955 3977 50  0000 C CNN
F 2 "" H 10950 4150 50  0001 C CNN
F 3 "" H 10950 4150 50  0001 C CNN
	1    10950 4150
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0102
U 1 1 5D2B22B4
P 3550 1850
F 0 "#PWR0102" H 3550 1700 50  0001 C CNN
F 1 "+5V" H 3565 2023 50  0000 C CNN
F 2 "" H 3550 1850 50  0001 C CNN
F 3 "" H 3550 1850 50  0001 C CNN
	1    3550 1850
	1    0    0    -1  
$EndComp
Text Label 3550 2450 0    50   ~ 0
PDL1
Text Label 2850 2350 0    50   ~ 0
PDL0
Text Label 2900 2150 0    50   ~ 0
SW1
Text Label 2900 2450 0    50   ~ 0
SW0
Text Label 2900 2250 0    50   ~ 0
GND
Text Label 3550 2150 0    50   ~ 0
5V
Wire Wire Line
	9550 2600 9550 3200
Wire Wire Line
	9550 3200 9250 3200
$Comp
L Connector:DB9_Male J1(SEGA)1
U 1 1 5CF741FC
P 10700 2500
F 0 "J1(SEGA)1" H 10750 2917 50  0000 C CNN
F 1 "Conn_02x05_Odd_Even" H 10750 2826 50  0000 C CNN
F 2 "Connector_Dsub:DSUB-9_Male_Horizontal_P2.77x2.84mm_EdgePinOffset4.94mm_Housed_MountingHolesOffset7.48mm" H 10700 2500 50  0001 C CNN
F 3 "~" H 10700 2500 50  0001 C CNN
	1    10700 2500
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Male J4(A/D_Breakout)1
U 1 1 5D357616
P 10650 3500
F 0 "J4(A/D_Breakout)1" H 10758 3781 50  0000 C CNN
F 1 "Conn_01x04_Male" H 10758 3690 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Horizontal" H 10650 3500 50  0001 C CNN
F 3 "~" H 10650 3500 50  0001 C CNN
	1    10650 3500
	-1   0    0    -1  
$EndComp
Wire Wire Line
	9250 3400 10450 3400
Wire Wire Line
	10450 3700 10450 4150
Connection ~ 10450 4150
Wire Wire Line
	10450 4150 10250 4150
Wire Wire Line
	10450 3600 9700 3600
Wire Wire Line
	10450 3500 9850 3500
Wire Wire Line
	9850 3500 9850 4700
Wire Wire Line
	8250 4700 8250 3800
Wire Wire Line
	9700 1850 9700 3600
Wire Wire Line
	10400 2600 9550 2600
Wire Wire Line
	10400 2900 9650 2900
Wire Wire Line
	9650 2900 9650 2700
Wire Wire Line
	9650 2700 9250 2700
Wire Wire Line
	10400 2700 9800 2700
Wire Wire Line
	9800 2700 9800 2800
Wire Wire Line
	9800 2800 9250 2800
Wire Wire Line
	10400 2500 9900 2500
Wire Wire Line
	9900 2500 9900 2850
Wire Wire Line
	9900 2850 9250 2850
Wire Wire Line
	9250 2850 9250 2900
Wire Wire Line
	10400 2300 9950 2300
Wire Wire Line
	9950 2300 9950 3000
Wire Wire Line
	9950 3000 9250 3000
Wire Wire Line
	10400 2800 10050 2800
Wire Wire Line
	10050 2800 10050 3100
Wire Wire Line
	10050 3100 9250 3100
Wire Wire Line
	10400 2200 10150 2200
Wire Wire Line
	10150 2200 10150 3300
Wire Wire Line
	10150 3300 9250 3300
Wire Wire Line
	10400 2100 10400 1850
Wire Wire Line
	10400 1850 9700 1850
Connection ~ 9700 1850
Wire Wire Line
	10400 2400 10250 2400
Wire Wire Line
	10250 2400 10250 4150
Connection ~ 10250 4150
Wire Wire Line
	7250 4600 8650 4600
Wire Wire Line
	7250 4700 7950 4700
Wire Wire Line
	7250 4800 8000 4800
Wire Wire Line
	7250 4900 8050 4900
Wire Wire Line
	6050 2650 6350 2650
Wire Wire Line
	5950 2250 6350 2250
Wire Wire Line
	2450 4150 5050 4150
Connection ~ 2450 4150
Connection ~ 6850 4150
Wire Wire Line
	1450 2650 2250 2650
$Comp
L Potentiometer_Digital:MCP42100 U3
U 1 1 5D9B11A9
P 5050 2550
F 0 "U3" H 5050 3231 50  0000 C CNN
F 1 "MCP42100" H 5050 3140 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm" H 5050 2650 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/11195c.pdf" H 5050 2650 50  0001 C CNN
	1    5050 2550
	-1   0    0    -1  
$EndComp
Connection ~ 6350 2250
Connection ~ 6350 2650
Connection ~ 7350 2750
$Comp
L Potentiometer_Digital:MCP42100 U2
U 1 1 5CF6A381
P 6850 2550
F 0 "U2" H 6850 3231 50  0000 C CNN
F 1 "MCP42100" H 6850 3140 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm" H 6850 2650 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/11195c.pdf" H 6850 2650 50  0001 C CNN
	1    6850 2550
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2250 3100 8250 3100
Wire Wire Line
	5550 2250 5550 2000
Wire Wire Line
	5550 2000 7850 2000
Wire Wire Line
	7850 2000 7850 2250
Connection ~ 7850 2250
Wire Wire Line
	7350 2450 7550 2450
Wire Wire Line
	7550 2450 7550 3200
Wire Wire Line
	7550 3200 5750 3200
Wire Wire Line
	5750 3200 5750 2350
Wire Wire Line
	5750 2350 5550 2350
NoConn ~ 5550 2450
Wire Wire Line
	5550 2850 5550 2750
Wire Wire Line
	5550 2750 5650 2750
Wire Wire Line
	5650 2750 5650 1850
Connection ~ 5550 2750
Connection ~ 5650 1850
Wire Wire Line
	5650 1850 6150 1850
Wire Wire Line
	7700 3600 5700 3600
Wire Wire Line
	5700 3600 5700 2550
Wire Wire Line
	5700 2550 5550 2550
Connection ~ 7700 3600
Wire Wire Line
	7700 3600 7700 4450
Wire Wire Line
	5050 3050 5050 4150
Connection ~ 5050 4150
Wire Wire Line
	5050 4150 6850 4150
Wire Wire Line
	4550 2750 4550 2650
Wire Wire Line
	4550 2350 4550 2250
Wire Wire Line
	6050 2650 6050 3850
Wire Wire Line
	6050 3850 4550 3850
Wire Wire Line
	4550 3850 4550 2850
Wire Wire Line
	5950 2250 5950 3950
Wire Wire Line
	5950 3950 4450 3950
Wire Wire Line
	4450 3950 4450 2450
Wire Wire Line
	4450 2450 4550 2450
Wire Wire Line
	2550 2950 4200 2950
Wire Wire Line
	4200 2950 4200 2250
Wire Wire Line
	4200 2250 4550 2250
Connection ~ 4550 2250
Wire Wire Line
	4550 2650 3800 2650
Connection ~ 4550 2650
Wire Wire Line
	3550 2450 3800 2450
Wire Wire Line
	3800 2450 3800 2650
Wire Wire Line
	10250 4150 8750 4150
Wire Wire Line
	8850 1850 9700 1850
NoConn ~ 9250 3700
Wire Wire Line
	9850 4700 8250 4700
$EndSCHEMATC
