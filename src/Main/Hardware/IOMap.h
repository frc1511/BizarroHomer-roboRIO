#pragma once

/*
  ██╗ ██████╗     ███╗   ███╗ █████╗ ██████╗ 
  ██║██╔═══██╗    ████╗ ████║██╔══██╗██╔══██╗
  ██║██║   ██║    ██╔████╔██║███████║██████╔╝
  ██║██║   ██║    ██║╚██╔╝██║██╔══██║██╔═══╝ 
  ██║╚██████╔╝    ██║ ╚═╝ ██║██║  ██║██║     
  ╚═╝ ╚═════╝     ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝     v2022
*/

// #define TEST_BOARD

#ifdef TEST_BOARD
#   include <Hardware/_TestBoardIOMap.h>
#else
#   include <Hardware/_MainIOMap.h>
#endif

// CAN

#define CAN_DRIVE_MOTOR_L0 1
#define CAN_DRIVE_MOTOR_L1 2
#define CAN_DRIVE_MOTOR_R0 3
#define CAN_DRIVE_MOTOR_R1 4

// PWM
