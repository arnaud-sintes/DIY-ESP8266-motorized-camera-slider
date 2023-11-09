#include "Program.h"


Program program;


void setup()
{
	program.Setup( {
        .logger{
            .baudRate{ 115200 },
        },
        .servoMotor{
            .pin{ 16 },
            .pivot{ 96 },
            .max{ 8 },
        },
        .wifiAP{
            .ssid{ "slider" },
            .password{ "12345678" },
            .ip{ IPAddress( 192, 168, 1, 1 ) },
            .domain{ "slider" },
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