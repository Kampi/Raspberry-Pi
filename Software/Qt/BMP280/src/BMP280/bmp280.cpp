#include "bmp280.h"

#define BMP280_ADDRESS                          0x76
#define BMP280_ID                               0x58

#define BMP280_REGISTER_ID                      0xD0
#define BMP280_REGISTER_SOFT_RESET              0xE0
#define BMP280_REGISTER_STATUS                  0xF3
#define BMP280_REGISTER_CTRL_MEAS               0xF4
#define BMP280_REGISTER_TEMP_XLSB               0xFC
#define BMP280_REGISTER_TEMP_LSB                0xFB
#define BMP280_REGISTER_TEMP_MSB                0xFA
#define BMP280_REGISTER_PRESS_XLSB              0xF9
#define BMP280_REGISTER_PRESS_LSB               0xF8
#define BMP280_REGISTER_PRESS_MSB               0xF7

#define BMP280_BIT_MEASURE                      0x03

#define BMP280_CMD_RESET                        0xB6

BMP280::BMP280() : _mHandle(0)
{
}

BMP280::~BMP280()
{
    close(_mHandle);
}

BMP280::Error BMP280::OpenDevice(const char* Interface)
{
    _mHandle = open(Interface, O_RDWR);
    if(_mHandle < 0)
    {
        return INVALID_INTERFACE;
    }

    // Check if the device address is available
    if(ioctl(_mHandle, I2C_SLAVE, BMP280_ADDRESS) < 0)
    {
        return NO_DEVICE;
    }

    // Check the device ID
    uint8_t ID;
    BMP280::Error Error = I2CReadByte(BMP280_REGISTER_ID, &ID);
    if(Error != NO_ERROR)
    {
        return Error;
    }

    if(ID != BMP280_ID)
    {
        return NO_DEVICE;
    }

    return ReadCalibCoef();
}

BMP280::Error BMP280::Reset(void)
{
    return I2CWriteByte(BMP280_REGISTER_SOFT_RESET, BMP280_CMD_RESET);
}

BMP280::Error BMP280::SetMode(BMP280::Mode Mode)
{
    uint8_t Data;

    if(I2CReadByte(BMP280_REGISTER_CTRL_MEAS, &Data))
    {
        return TRANSMISSION_ERROR;
    }

    Data &= 0xFC;
    Data |= Mode;

    return I2CWriteByte(BMP280_REGISTER_CTRL_MEAS, Data);
}

BMP280::Error BMP280::GetMode(BMP280::Mode* Mode)
{
    uint8_t Data;

    if(I2CReadByte(BMP280_REGISTER_CTRL_MEAS, &Data))
    {
        return TRANSMISSION_ERROR;
    }

    *Mode = (BMP280::Mode)(Data & 0x03);

    return NO_ERROR;
}

BMP280::Error BMP280::ConversionRunning(bool* Running)
{
    uint8_t Data;
    if(I2CReadByte(BMP280_REGISTER_STATUS, &Data))
    {
        return TRANSMISSION_ERROR;
    }

    *Running = (Data & (0x01 << BMP280_BIT_MEASURE)) >> BMP280_BIT_MEASURE;

    return NO_ERROR;
}

BMP280::Error BMP280::MeasureTemperature(BMP280::Oversampling OSS, double* Temperature)
{
    int32_t RawTemp;

    if(ReadTemperature(OSS, &RawTemp))
    {
        return TRANSMISSION_ERROR;
    }

    int32_t var1 = (((RawTemp >> 0x03) - (_mCalibCoef.T1 << 0x01)) * _mCalibCoef.T2) >> 0x0B;
    int32_t var2 = (((((RawTemp >> 0x04) - _mCalibCoef.T1) * ((RawTemp >> 0x04) - _mCalibCoef.T1)) >> 0x0C) * _mCalibCoef.T3) >> 0x0E;
    _mTemperatureFine = var1 + var2;
    *Temperature = (double)((_mTemperatureFine * 0x05 + 0x80) >> 0x08) / 100.0;

    return NO_ERROR;
}

BMP280::Error BMP280::MeasurePressure(BMP280::Oversampling OSS_Pressure, BMP280::Oversampling OSS_Temperature, double* Pressure)
{
    int32_t RawTemp;
    int32_t RawPress;

    if(ReadTemperature(OSS_Temperature, &RawTemp) || ReadPressure(OSS_Pressure, &RawPress))
    {
        return TRANSMISSION_ERROR;
    }

    int64_t var1 = ((int64_t)_mTemperatureFine) - 128000;
    int64_t var2 = var1 * var1 * (int64_t)_mCalibCoef.P6;
    var2 = var2 + ((var1 * (int64_t)_mCalibCoef.P5) << 0x11);
    var2 = var2 + (((int64_t)_mCalibCoef.P4) << 0x23);
    var1 = ((var1 * var1 * (int64_t)_mCalibCoef.P3) >> 0x08) + ((var1 * (int64_t)_mCalibCoef.P2) << 0x0C);
    var1 = (((((int64_t)1) << 0x2F) + var1)) * ((int64_t)_mCalibCoef.P1) >> 0x21;

    if(var1 == 0)
    {
        return TRANSMISSION_ERROR;
    }

    int64_t p = 1048576 - RawPress;
    p = (((p << 0x1F) - var2) * 3125) / var1;
    var1 = (((int64_t)_mCalibCoef.P9) * (p >> 0x0D) * (p >> 0x0D)) >> 0x19;
    var2 = (((int64_t)_mCalibCoef.P8) * p) >> 0x13;
    p = ((p + var1 + var2) >> 0x08) + (((int64_t)_mCalibCoef.P7) << 0x04);

    *Pressure = (double)p / 25600;

    return NO_ERROR;
}

BMP280::CalibCoef BMP280::calibCoef(void) const
{
    return _mCalibCoef;
}

BMP280::Error BMP280::I2CReadByte(uint8_t Register, uint8_t* Data)
{
    union i2c_smbus_data I2CData;
    struct i2c_smbus_ioctl_data IOCtl;

    IOCtl.read_write = I2C_SMBUS_READ;
    IOCtl.command = Register;
    IOCtl.size = I2C_SMBUS_BYTE_DATA;
    IOCtl.data = &I2CData;

    if(ioctl(_mHandle, I2C_SMBUS, &IOCtl))
    {
        return TRANSMISSION_ERROR;
    }

    *Data = I2CData.byte;

    return NO_ERROR;
}

BMP280::Error BMP280::I2CWriteByte(uint8_t Register, uint8_t Data)
{
    union i2c_smbus_data I2CData;
    struct i2c_smbus_ioctl_data IOCtl;

    IOCtl.read_write = I2C_SMBUS_WRITE;
    IOCtl.command = Register;
    IOCtl.size = I2C_SMBUS_BYTE_DATA;
    IOCtl.data = &I2CData;
    I2CData.byte = Data;

    if(ioctl(_mHandle, I2C_SMBUS, &IOCtl))
    {
        return TRANSMISSION_ERROR;
    }

    return NO_ERROR;
}

BMP280::Error BMP280::ReadUInt(uint8_t Register, uint16_t* Data)
{
    uint8_t Bytes[2];

    if(BMP280::I2CReadByte(Register, &Bytes[0]) || BMP280::I2CReadByte(Register + 1, &Bytes[1]))
    {
        return TRANSMISSION_ERROR;
    }

    *Data = (Bytes[1] << 0x08) | Bytes[0];

    return NO_ERROR;
}

BMP280::Error BMP280::ReadSInt(uint8_t Register, int16_t* Data)
{
    uint8_t Bytes[2];

    if(BMP280::I2CReadByte(Register, &Bytes[0]) || BMP280::I2CReadByte(Register + 1, &Bytes[1]))
    {
        return TRANSMISSION_ERROR;
    }

    *Data = (Bytes[1] << 0x08) | Bytes[0];
    if(*Data & (0x01 << 0x0F))
    {
        *Data -= 0xFFFF;
    }

    return NO_ERROR;
}

BMP280::Error BMP280::ReadCalibCoef(void)
{
    if(ReadUInt(0x88, &_mCalibCoef.T1) || ReadSInt(0x8A, &_mCalibCoef.T2) || ReadSInt(0x8C, &_mCalibCoef.T3) ||
        ReadUInt(0x8E, &_mCalibCoef.P1) || ReadSInt(0x90, &_mCalibCoef.P2) || ReadSInt(0x92, &_mCalibCoef.P3) ||
        ReadSInt(0x94, &_mCalibCoef.P4) || ReadSInt(0x96, &_mCalibCoef.P5) || ReadSInt(0x98, &_mCalibCoef.P6) ||
        ReadSInt(0x9A, &_mCalibCoef.P7) || ReadSInt(0x9C, &_mCalibCoef.P8) || ReadSInt(0x9E, &_mCalibCoef.P9))
    {
        return TRANSMISSION_ERROR;
    }

    return NO_ERROR;
}

BMP280::Error BMP280::ReadTemperature(BMP280::Oversampling OSS, int32_t* RawTemp)
{
    uint8_t Data[3] = {0x00, 0x00, 0x00};
    bool Running = false;

    if(I2CReadByte(BMP280_REGISTER_CTRL_MEAS, &Data[0]))
    {
        return TRANSMISSION_ERROR;
    }

    Data[0] &= 0x1F;
    Data[0] |= (OSS & 0x07) << 0x05;

    if(I2CWriteByte(BMP280_REGISTER_CTRL_MEAS, Data[0]))
    {
        return TRANSMISSION_ERROR;
    }

    while(!Running)
    {
        if(ConversionRunning(&Running))
        {
            return TRANSMISSION_ERROR;
        }
    }

    for(uint8_t i = 0x00; i < 0x03; i++)
    {
        if(I2CReadByte(BMP280_REGISTER_TEMP_MSB, &Data[i]))
        {
            return TRANSMISSION_ERROR;
        }
    }

    *RawTemp = (Data[0] << 0x0C) | (Data[1] << 0x04) | (Data[2] >> 0x04);

    return NO_ERROR;
}

BMP280::Error BMP280::ReadPressure(BMP280::Oversampling OSS, int32_t* RawPress)
{
    uint8_t Data[3] = {0x00, 0x00, 0x00};
    bool Running = false;

    if(I2CReadByte(BMP280_REGISTER_CTRL_MEAS, &Data[0]))
    {
        return TRANSMISSION_ERROR;
    }

    Data[0] &= 0xE3;
    Data[0] |= (OSS & 0x07) << 0x02;

    if(I2CWriteByte(BMP280_REGISTER_CTRL_MEAS, Data[0]))
    {
        return TRANSMISSION_ERROR;
    }

    while(!Running)
    {
        if(ConversionRunning(&Running))
        {
            return TRANSMISSION_ERROR;
        }
    }

    for(uint8_t i = 0x00; i < 0x03; i++)
    {
        if(I2CReadByte(BMP280_REGISTER_PRESS_MSB, &Data[i]))
        {
            return TRANSMISSION_ERROR;
        }
    }

    *RawPress = (Data[0] << 0x0C) | (Data[1] << 0x04) | (Data[2] >> 0x04);

    return NO_ERROR;
}
