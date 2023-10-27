/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_IIO_TYPES_H_
#define _UAPI_IIO_TYPES_H_
enum iio_chan_type {
  IIO_VOLTAGE,
  IIO_CURRENT,
  IIO_POWER,
  IIO_ACCEL,
  IIO_ANGL_VEL,
  IIO_MAGN,
  IIO_LIGHT,
  IIO_INTENSITY,
  IIO_PROXIMITY,
  IIO_TEMP,
  IIO_INCLI,
  IIO_ROT,
  IIO_ANGL,
  IIO_TIMESTAMP,
  IIO_CAPACITANCE,
  IIO_ALTVOLTAGE,
  IIO_CCT,
  IIO_PRESSURE,
  IIO_HUMIDITYRELATIVE,
  IIO_ACTIVITY,
  IIO_STEPS,
  IIO_ENERGY,
  IIO_DISTANCE,
  IIO_VELOCITY,
  IIO_CONCENTRATION,
  IIO_RESISTANCE,
  IIO_PH,
  IIO_UVINDEX,
  IIO_ELECTRICALCONDUCTIVITY,
  IIO_COUNT,
  IIO_INDEX,
  IIO_GRAVITY,
  IIO_POSITIONRELATIVE,
  IIO_PHASE,
  IIO_MASSCONCENTRATION,
};
enum iio_modifier {
  IIO_NO_MOD,
  IIO_MOD_X,
  IIO_MOD_Y,
  IIO_MOD_Z,
  IIO_MOD_X_AND_Y,
  IIO_MOD_X_AND_Z,
  IIO_MOD_Y_AND_Z,
  IIO_MOD_X_AND_Y_AND_Z,
  IIO_MOD_X_OR_Y,
  IIO_MOD_X_OR_Z,
  IIO_MOD_Y_OR_Z,
  IIO_MOD_X_OR_Y_OR_Z,
  IIO_MOD_LIGHT_BOTH,
  IIO_MOD_LIGHT_IR,
  IIO_MOD_ROOT_SUM_SQUARED_X_Y,
  IIO_MOD_SUM_SQUARED_X_Y_Z,
  IIO_MOD_LIGHT_CLEAR,
  IIO_MOD_LIGHT_RED,
  IIO_MOD_LIGHT_GREEN,
  IIO_MOD_LIGHT_BLUE,
  IIO_MOD_QUATERNION,
  IIO_MOD_TEMP_AMBIENT,
  IIO_MOD_TEMP_OBJECT,
  IIO_MOD_NORTH_MAGN,
  IIO_MOD_NORTH_TRUE,
  IIO_MOD_NORTH_MAGN_TILT_COMP,
  IIO_MOD_NORTH_TRUE_TILT_COMP,
  IIO_MOD_RUNNING,
  IIO_MOD_JOGGING,
  IIO_MOD_WALKING,
  IIO_MOD_STILL,
  IIO_MOD_ROOT_SUM_SQUARED_X_Y_Z,
  IIO_MOD_I,
  IIO_MOD_Q,
  IIO_MOD_CO2,
  IIO_MOD_VOC,
  IIO_MOD_LIGHT_UV,
  IIO_MOD_LIGHT_DUV,
  IIO_MOD_PM1,
  IIO_MOD_PM2P5,
  IIO_MOD_PM4,
  IIO_MOD_PM10,
  IIO_MOD_ETHANOL,
  IIO_MOD_H2,
  IIO_MOD_O2,
  IIO_MOD_LINEAR_X,
  IIO_MOD_LINEAR_Y,
  IIO_MOD_LINEAR_Z,
  IIO_MOD_PITCH,
  IIO_MOD_YAW,
  IIO_MOD_ROLL,
};
enum iio_event_type {
  IIO_EV_TYPE_THRESH,
  IIO_EV_TYPE_MAG,
  IIO_EV_TYPE_ROC,
  IIO_EV_TYPE_THRESH_ADAPTIVE,
  IIO_EV_TYPE_MAG_ADAPTIVE,
  IIO_EV_TYPE_CHANGE,
  IIO_EV_TYPE_MAG_REFERENCED,
  IIO_EV_TYPE_GESTURE,
};
enum iio_event_direction {
  IIO_EV_DIR_EITHER,
  IIO_EV_DIR_RISING,
  IIO_EV_DIR_FALLING,
  IIO_EV_DIR_NONE,
  IIO_EV_DIR_SINGLETAP,
  IIO_EV_DIR_DOUBLETAP,
};
#endif
