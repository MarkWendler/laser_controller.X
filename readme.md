# LaserController 

Laser controller board firmware for 32bit MCU.

### Features
* Control 6 Laser distance measure module 
* Control 6 pair (12) Servo
* Communicate on CAN-bus

### Peripheral chain

Peripheral   | Board function | 
CAN1         |CAN |
UART6        |  
GPIO_Boost_1 | Laser 1 power boost

### Pin configuration

Pin  | PIN ID | Function       | Board Function      |  BOARD Name  |
1    | RA7    |   C1RX         | CAN RX              |  CLRX        |
2    | RB14   |   GPIO_Boost_1 | Laser 1 power boost | MCU-I/O-1    |
3    | RB15   | C1TX           | CAN TX              | CLTX         |
4    | RG6    | U6RX           | Laser 1 TX          | U6RX         |
6    | RG8    | U6TX           | Laser 1 RX          | U6TX         |
14   | RA1    | GPIO_Boost_6   | Laser 6 power boost | MCU-I/O-6    |
15   | RB0    | U2TX           | Laser 6 TX          | U2TX         |
16   | RB1    | U2RX           | Laser 6 RX          | U2RX         |
21   | RC0    | U1TX           | Laser 5 TX          | U1TX         |
22   | RC1    | U1RX           | Laser 5 RX          | U1RX         |
23   | RC2    | GPIO_BTN_S2    | Button S2           | S2           |
24   | RC11   | GPIO_BTN_S3    | Button S3           | S3           |
27   | RE12   | GPIO_Boost_5   | Laser 5 power boost | MCU-I/O-5    |
29   | RE14   | GPIO_Boost_4   | Laser 4 power boost | MCU-I/O-4    |
30   | RE15   | U5TX           | Laser 4 TX          | U5TX         |
31   | RA8    | OC12           | Servo 6B            | PWM11        |
33   | RA4    | OC5            | Servo 6A            | PWM12        |
40   | RC15   | OC10           | Servo 5B            | PWM10        |
42   | RD8    | GPIO_LED1      | LED1                | LED1         |
43   | RB5    | OC2            | Servo 5A            | PWM9         |
44   | RB6    | OC4            | Servo 4B            | PWM8         |
45   | RC10   | OC3            | Servo 4A            | PWM7         |
46   | RB7    | OC1            | Servo 3B            | PWM6         |
47   | RC13   | GPIO_LED2      | LED2                | LED2         |
48   | RB8    | GPIO_LED3      | LED3                | LED3         |
49   | RB9    | OC1            | Servo 3A            | PWM5         |
50   | RC6    | U3TX           | Laser 3 TX          | U3TX         |
51   | RC7    | U3RX           | Laser 3 RX          | U3RX         |
52   | RC8    | OC8            | Servo 2B            | PWM4         |
53   | RD5    | OC7            | Servo 2A            | PWM3         |
54   | RD6    | OC9            | Servo 1B            | PWM2         |
55   | RC9    | OC11           | Servo 1A            | PWM1         |
58   | RF0    | U5RX           | Laser 4 RX          | U4RX         |
60   | RB10   | U4RX           | Laser 2 RX          | U4RX         |
61   | RB11   | U4TX           | Laser 2 TX          | U4TX         |
62   | RB12   | GPIO_Boost_2   | Laser 2 power boost | MCU-I/O-2    |
63   | RB13   | GPIO_Boost_3   | Laser 3 power boost | MCU-I/O-3    |