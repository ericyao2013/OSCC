/**
 * @file init.cpp
 *
 */


#include <Arduino.h>
#include "oscc_serial.h"
#include "oscc_can.h"
#include "debug.h"
#include "oscc_time.h"

#include "globals.h"
#include "init.h"
#include "accumulator.h"
#include "master_cylinder.h"
#include "brake_control.h"


void init_globals( void )
{
    memset( &g_brake_control_state,
            0,
            sizeof(g_brake_control_state) );

    // Initialize the timestamps to avoid timeout warnings on start up
    g_brake_command_last_rx_timestamp = GET_TIMESTAMP_MS( );
    g_brake_command_last_rx_timestamp = GET_TIMESTAMP_MS( );
    g_chassis_state_1_report_last_rx_timestamp = GET_TIMESTAMP_MS( );

    pid_zeroize( &g_pid, PID_WINDUP_GUARD );

    g_pid.proportional_gain = PID_PROPORTIONAL_GAIN;
    g_pid.integral_gain = PID_INTEGRAL_GAIN;
    g_pid.derivative_gain = PID_DERIVATIVE_GAIN;
}


void init_devices( void )
{
    // set the Arduino's PWM timers to 3.921 KHz, above the acoustic range
    TCCR3B = (TCCR3B & 0xF8) | 0x02; // pins 2,3,5 | timer 3
    TCCR4B = (TCCR4B & 0xF8) | 0x02; // pins 6,7,8 | timer 4

    accumulator_init( );
    master_cylinder_init( );
    brake_init( );

    accumulator_turn_pump_off( );
    master_cylinder_open( );

    set_release_solenoid_duty_cycle( SOLENOID_PWM_OFF );
    set_accumulator_solenoid_duty_cycle( SOLENOID_PWM_OFF );
}


void init_communication_interfaces( void )
{
    #ifdef DEBUG
    init_serial( );
    #endif

    DEBUG_PRINT( "init Control CAN - " );
    init_can( g_control_can );
}
