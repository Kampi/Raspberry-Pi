#ifndef BMP280_H
#define BMP280_H

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <iostream>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/types.h>
#include <linux/i2c-dev.h>

class BMP280
{
    public:
        typedef enum
        {
            NO_ERROR = 0x00,
            INVALID_INTERFACE = 0x01,
            NO_DEVICE = 0x02,
            TRANSMISSION_ERROR = 0x03,
        } Error;

        typedef enum
        {
            MODE_SLEEP = 0x00,
            MODE_FORCED = 0x01,
            MODE_NORMAL = 0x03,
        } Mode;

        typedef enum
        {
            OSS_X1 = 0x01,
            OSS_X2 = 0x02,
            OSS_X4 = 0x03,
            OSS_X8 = 0x04,
            OSS_X16 = 0x05,
        } Oversampling;

        struct CalibCoef
        {
            uint16_t T1;
            int16_t T2;
            int16_t T3;
            uint16_t P1;
            int16_t P2;
            int16_t P3;
            int16_t P4;
            int16_t P5;
            int16_t P6;
            int16_t P7;
            int16_t P8;
            int16_t P9;

            friend std::ostream& operator << (std::ostream& Stream, const BMP280::CalibCoef& Coef)
            {
                Stream << "T1: " << Coef.T1 << "T2: " << Coef.T2 << "T3: " << Coef.T3;
                Stream << "P1: " << Coef.P1 << "P2: " << Coef.P3 << "P3: " << Coef.P3 << "P4: " << Coef.P4 << "P5: " << Coef.P5 << "P6: " << Coef.P6 << "P7: " << Coef.P7 << "P8: " << Coef.P8 << "P9: " << Coef.P9;
                return Stream;
            }
        };

    public:
        BMP280();
        ~BMP280();
        Error OpenDevice(const char* Interface);
        Error Reset(void);
        Error SetMode(Mode Mode);
        Error GetMode(Mode* Mode);
        Error ConversionRunning(bool* Running);
        Error MeasureTemperature(Oversampling OSS, double* Temperature);
        Error MeasurePressure(Oversampling OSS_Pressure, Oversampling OSS_Temperature, double* Pressure);
        CalibCoef calibCoef(void) const;

    private:
        int32_t _mHandle;
        int32_t _mTemperatureFine;
        CalibCoef _mCalibCoef;

        Error I2CReadByte(uint8_t Register, uint8_t* Data);
        Error I2CWriteByte(uint8_t Register, uint8_t Data);
        Error ReadUInt(uint8_t Register, uint16_t* Data);
        Error ReadSInt(uint8_t Register, int16_t* Data);
        Error ReadCalibCoef(void);
        Error ReadTemperature(Oversampling OSS, int32_t* RawTemp);
        Error ReadPressure(Oversampling OSS, int32_t* RawPress);
};

#endif // BMP280_H
