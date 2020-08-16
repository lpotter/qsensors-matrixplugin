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

// Inspired by qtmatrix module

#include "matrixsensorbase.h"
#include "qsensors-matrixpluginprivate_p.h"

#include <QFile>
#include <QStandardPaths>
#include <QSensorBackend>

#include <unistd.h>
#include <errno.h>
#include <time.h>

#define RADIANS_TO_DEGREES 57.2957795
#define STANDARD_GRAVITY 9.80665
#define RADIANS_TO_DEGREES 57.2957795

static const int MAX_READ_ATTEMPTS = 5;
//Q_LOGGING_CATEGORY(qMatrix, "sensor.matrix")

quint64 produceTimestamp()
{
    struct timespec tv;
    int ok;

#ifdef CLOCK_MONOTONIC_RAW
    ok = clock_gettime(CLOCK_MONOTONIC_RAW, &tv);
    if (ok != 0)
#endif
    ok = clock_gettime(CLOCK_MONOTONIC, &tv);
    Q_ASSERT(ok == 0);

    quint64 result = (tv.tv_sec * 1000000ULL) + (tv.tv_nsec * 0.001); // scale to microseconds
    return result;
}


QMatrixSensorsPrivate::QMatrixSensorsPrivate(MatrixSensorBase *q_ptr)
    : q(q_ptr),
      matrixIOBus(Q_NULLPTR),
      imuInited(false),
      humidityInited(false),
      pressureInited(false),
      temperatureFromHumidity(true)
{
}

QMatrixSensorsPrivate::~QMatrixSensorsPrivate()
{
    delete matrixIOBus;
    matrixIOBus = 0;
}

bool QMatrixSensorsPrivate::open()
{
    if (!matrixIOBus) {
        matrixIOBus = new matrix_hal::MatrixIOBus();
    }
    if (matrixIOBus && matrixIOBus->Init()) {
        switch (q->sensorFlag) {
            case MatrixSensorBase::Pressure:
            case MatrixSensorBase::Altimeter:
                pressureSensor.Setup(matrixIOBus);
            break;
            case MatrixSensorBase::Temperature:
                humiditySensor.Setup(matrixIOBus);
            break;
            case MatrixSensorBase::Uv:
                uvSensor.Setup(matrixIOBus);
           break;
           default:
                imuSensor.Setup(matrixIOBus);
           break;
        };
    } else {
        qWarning() << "Error SpiInit";
        return false;
    }

    return true;
}

void QMatrixSensorsPrivate::update(MatrixSensorBase::UpdateFlag what)
{
    switch (what) {
    case MatrixSensorBase::Pressure:
    {
        pressureSensor.Read(&pressureData);
        qreal pascals = (qreal)pressureData.pressure * 0.01;
        if (pressure.pressure() != pascals) {
            pressure.setTimestamp(produceTimestamp());
            pressure.setPressure(pascals);
            emit q->pressureChanged(pressure);
        }
    }
        break;
    case MatrixSensorBase::Temperature:
    {
        humiditySensor.Read(&humidityData);
        if (temperature.temperature() != (qreal)humidityData.temperature) {
            temperature.setTemperature((qreal)humidityData.temperature);
            temperature.setTimestamp(produceTimestamp());
            emit q->temperatureChanged(temperature);
        }
    }
        break;

    case MatrixSensorBase::Altimeter:
    {
        pressureSensor.Read(&pressureData);
        if (altitude.altitude() != (qreal)pressureData.altitude) {
            altitude.setAltitude((qreal)pressureData.altitude);
            altitude.setTimestamp(produceTimestamp());
            emit q->altitudeChanged(altitude);
        }
    }

        break;
    case MatrixSensorBase::Acceleration:
    {
        imuSensor.Read(&imuData);
        acceleration.setTimestamp(produceTimestamp());
        acceleration.setX((qreal)imuData.accel_x * STANDARD_GRAVITY);
        acceleration.setY((qreal)imuData.accel_y * STANDARD_GRAVITY);
        acceleration.setZ((qreal)imuData.accel_z * STANDARD_GRAVITY);
        emit q->accelerationChanged(acceleration);
    }
        break;
    case MatrixSensorBase::Gyro:
    {
        imuSensor.Read(&imuData);
        gyro.setTimestamp(produceTimestamp());
        gyro.setX((qreal)imuData.gyro_x * RADIANS_TO_DEGREES);
        gyro.setY((qreal)imuData.gyro_y * RADIANS_TO_DEGREES);
        gyro.setZ((qreal)imuData.gyro_z * RADIANS_TO_DEGREES);

        emit q->gyroChanged(gyro);
    }

        break;
    case MatrixSensorBase::Magnetometer:
    {
        imuSensor.Read(&imuData);
        mag.setTimestamp(produceTimestamp());
        mag.setX((qreal)imuData.mag_x * .000001);
        mag.setY((qreal)imuData.mag_y * .000001);
        mag.setZ((qreal)imuData.mag_z * .000001);
        emit q->magnetometerChanged(mag);
    }
    break;
    case MatrixSensorBase::Uv:
    {
        uvSensor.Read(&uvData);
        lux.setTimestamp(produceTimestamp());
        lux.setLux((qreal)uvData.uv);
        emit q->uvChanged(lux);
    }
    break;
    default:
    break;
    };
}

static inline qreal toDeg360(qreal rad)
{
    const qreal deg = rad * RADIANS_TO_DEGREES;
    return deg < 0 ? deg + 360 : deg;
}

MatrixSensorBase::MatrixSensorBase(QSensor *sensor)
    : QSensorBackend(sensor),
      d_ptr(new QMatrixSensorsPrivate(this))
{
}

MatrixSensorBase::~MatrixSensorBase()
{
}

bool MatrixSensorBase::needsImu(MatrixSensorBase::UpdateFlag sensorFlag)
{
    bool isImu = false;
    switch (sensorFlag) {
        case MatrixSensorBase::Gyro:
        case MatrixSensorBase::Acceleration:
        case MatrixSensorBase::Compass:
        case MatrixSensorBase::Orientation:
        case MatrixSensorBase::Magnetometer:
            isImu = true;
        break;
        default:
        break;
    };
    return isImu;
}

void MatrixSensorBase::start()
{
    d_ptr->imuInited = needsImu(sensorFlag);
    if (d_ptr->open()) {
        if (d_ptr->pollInterval > 100)
            d_ptr->pollTimer.setInterval(50);
        else
            d_ptr->pollTimer.setInterval(d_ptr->pollInterval);
        connect(&d_ptr->pollTimer, &QTimer::timeout, [this] { d_ptr->update(sensorFlag); });
        d_ptr->update(sensorFlag);
    } else {
        qWarning() << Q_FUNC_INFO << "Sensor not opened";
        sensorError(-ENODEV);
        stop();
        return;
    }
    d_ptr->pollTimer.start();
}

void MatrixSensorBase::stop()
{
    if (d_ptr->imuInited)
        d_ptr->pollTimer.stop();
    sensorStopped();
}

bool MatrixSensorBase::isFeatureSupported(QSensor::Feature /*feature*/) const
{
    return false;
}

void MatrixSensorBase::poll(MatrixSensorBase::UpdateFlag sensorFlag)
{
    d_ptr->update(sensorFlag);
}
