/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.8 at Mon May  1 14:35:55 2017. */

#include "vehiclecommand.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t VehicleCommand_fields[7] = {
    PB_FIELD(  1, FIXED64 , OPTIONAL, STATIC  , FIRST, VehicleCommand, speed, speed, 0),
    PB_FIELD(  2, FIXED64 , OPTIONAL, STATIC  , OTHER, VehicleCommand, acceleration, speed, 0),
    PB_FIELD(  3, FIXED64 , OPTIONAL, STATIC  , OTHER, VehicleCommand, steeringWheelAngle, acceleration, 0),
    PB_FIELD(  4, BOOL    , OPTIONAL, STATIC  , OTHER, VehicleCommand, brakeLights, steeringWheelAngle, 0),
    PB_FIELD(  5, BOOL    , OPTIONAL, STATIC  , OTHER, VehicleCommand, flashingLightsLeft, brakeLights, 0),
    PB_FIELD(  6, BOOL    , OPTIONAL, STATIC  , OTHER, VehicleCommand, flashingLightsRight, flashingLightsLeft, 0),
    PB_LAST_FIELD
};


/* @@protoc_insertion_point(eof) */
