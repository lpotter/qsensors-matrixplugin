// Copyright (C) 2015, 2016 Canonical Ltd
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License version 3 as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef QSENSORSMATRIXPLUGINPRIVATE_P_H
#define QSENSORSMATRIXPLUGINPRIVATE_P_H

#include "matrixsensorbase.h"
#include <QCompassReading>
#include <QGyroscopeReading>
#include <QAccelerometerReading>
#include <QOrientationReading>
#include <QMagnetometerReading>
#include <QRotationReading>
#include <QAmbientTemperatureReading>

#include <imu_sensor.h>
#include <imu_data.h>

//temperature is part of humidity sensor
#include <humidity_data.h>
#include <humidity_sensor.h>

#include <pressure_sensor.h>
#include <pressure_data.h>

class QMatrixSensorsPrivate
{
public:
    QMatrixSensorsPrivate(MatrixSensorBase *q_ptr);
    ~QMatrixSensorsPrivate();
    MatrixSensorBase *q;

    matrix_hal::IMUSensor imuSensor;
    matrix_hal::IMUData imuData;

    matrix_hal::HumiditySensor humiditySensor;
    matrix_hal::HumidityData humidityData;

    matrix_hal::PressureSensor pressureSensor;
    matrix_hal::PressureData pressureData;


    bool imuInited;
    int pollInterval;
    bool humidityInited;
    bool pressureInited;
    QTimer pollTimer;
    bool temperatureFromHumidity;

    qreal humidity = 0;
    QPressureReading pressure;
    QAmbientTemperatureReading temperature;
    QGyroscopeReading gyro;
    QAccelerometerReading acceleration;
    QCompassReading compass;
    QOrientationReading orientation;
    QMagnetometerReading mag;
    QRotationReading rotation;
    QAmbientTemperatureReading temp;

    bool open();
    void update(MatrixSensorBase::UpdateFlags what);
    void report(const matrix_hal::IMUSensor &data, MatrixSensorBase::UpdateFlags what);

};



#endif // QSENSORSMATRIXPLUGINPRIVATE_P_H
