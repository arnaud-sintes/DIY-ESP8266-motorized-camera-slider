#include "Program.h"


// TODO fix servomotor rotation
// TODO better UI


Program program;


void setup()
{
	program.Setup( {
        .logger{
            .baudRate{ 115200 },
        },
        .servoMotor{
            .pin{ 16 },
        },
        .wifiAP{
            .ssid{ "slider" },
            .password{ "12345678" },
            .ip{ IPAddress( 192, 168, 1, 1 ) },
            .domain{ "slider.com" }, // TODO test without .com
        },
        .webServer{
            .port{ 80 }
        },
    } );
}


void loop()
{
    program.Loop();
}