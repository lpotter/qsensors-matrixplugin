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
      imuInited(false),
      humidityInited(false),
      pressureInited(false),
      temperatureFromHumidity(true),
      bus(Q_NULLPTR)
{

}

QMatrixSensorsPrivate::~QMatrixSensorsPrivate()
{
    delete bus;
    bus = 0;
}

bool QMatrixSensorsPrivate::open()
{
    qDebug() << Q_FUNC_INFO << q->sensorFlag;

    if (bus == Q_NULLPTR) {
        bus = new matrix_hal::WishboneBus();
    }
    if (bus->SpiInit()) {

        if (q->sensorFlag.testFlag(MatrixSensorBase::Pressure)) {
            pressureSensor.Setup(bus);
        } else if (q->sensorFlag.testFlag(MatrixSensorBase::Temperature)) {
            humiditySensor.Setup(bus);
        } else {
            imuSensor.Setup(bus);
        }
        imuInited = true;
    }

    return imuInited;
}

void QMatrixSensorsPrivate::update(MatrixSensorBase::UpdateFlags what)
{

    if (what.testFlag(MatrixSensorBase::Pressure)) {
        pressureSensor.Read(&pressureData);
        if (pressure.pressure() != (qreal)pressureData.pressure) {
            pressure.setTimestamp(produceTimestamp());
            pressure.setPressure((qreal)pressureData.pressure);
            emit q->pressureChanged(pressure);
        }
    }
    if (what.testFlag(MatrixSensorBase::Temperature)) {
        humiditySensor.Read(&humidityData);
        if (temperature.temperature() != (qreal)humidityData.temperature) {
            temperature.setTemperature((qreal)humidityData.temperature);
            temperature.setTimestamp(produceTimestamp());
            emit q->temperatureChanged(temperature);
        }
    }

    if (what.testFlag(MatrixSensorBase::Acceleration)) {
        imuSensor.Read(&imuData);
        acceleration.setTimestamp(produceTimestamp());
        acceleration.setX((qreal)imuData.accel_x * STANDARD_GRAVITY);
        acceleration.setY((qreal)imuData.accel_y * STANDARD_GRAVITY);
        acceleration.setZ((qreal)imuData.accel_z * STANDARD_GRAVITY);
        emit q->accelerationChanged(acceleration);
    }

    if (what.testFlag(MatrixSensorBase::Gyro)) {
        imuSensor.Read(&imuData);
        gyro.setTimestamp(produceTimestamp());
        gyro.setX((qreal)imuData.gyro_x * RADIANS_TO_DEGREES);
        gyro.setY((qreal)imuData.gyro_y * RADIANS_TO_DEGREES);
        gyro.setZ((qreal)imuData.gyro_z * RADIANS_TO_DEGREES);

        emit q->gyroChanged(gyro);
    }

    if (what.testFlag(MatrixSensorBase::Magnetometer)) {
        imuSensor.Read(&imuData);
        mag.setTimestamp(produceTimestamp());
        mag.setX((qreal)imuData.mag_x * .000001);
        mag.setY((qreal)imuData.mag_y * .000001);
        mag.setZ((qreal)imuData.mag_z * .000001);
        emit q->magnetometerChanged(mag);
    }

}

static inline qreal toDeg360(qreal rad)
{
    const qreal deg = rad * RADIANS_TO_DEGREES;
    return deg < 0 ? deg + 360 : deg;
}

void QMatrixSensorsPrivate::report(const matrix_hal::IMUSensor &data, MatrixSensorBase::UpdateFlags what)
{
    //qDebug() << Q_FUNC_INFO << what;

    //    if (what.testFlag(MatrixSensorBase::Humidity)) {
    //        if (data.humidityValid) {
    //            humidity = data.humidity;
    //            emit q->humidityChanged(humidity);
    //        }
    //    }



//    if (what.testFlag(MatrixSensorBase::Gyro)) {
//        gyro.setTimestamp((quint64)data.timestamp);
//        gyro.setX((qreal)data.gyro_x * RADIANS_TO_DEGREES);
//        gyro.setY((qreal)data.gyro_y * RADIANS_TO_DEGREES);
//        gyro.setZ((qreal)data.gyro_z * RADIANS_TO_DEGREES);

//        emit q->gyroChanged(gyro);
//    }

//    if (what.testFlag(MatrixSensorBase::Acceleration)) {
//        acceleration.setTimestamp((quint64)data.timestamp);
//        acceleration.setX((qreal)data.accel_x * STANDARD_GRAVITY);
//        acceleration.setY((qreal)data.accel_y * STANDARD_GRAVITY);
//        acceleration.setZ((qreal)data.accel_z * STANDARD_GRAVITY);
//        emit q->accelerationChanged(acceleration);
//    }
//    //    if (what.testFlag(MatrixSensorBase::Rotation)) {
//    //            rotation.setTimestamp((quint64)data.timestamp);
//    //            rotation.setFromEuler(toDeg360(data.fusionPose.x()),
//    //                                   toDeg360(data.fusionPose.y()),
//    //                                   toDeg360(data.fusionPose.z()));
//    //            emit q->rotationChanged(rotation);
//    //    }
//    // in rtimulib compass is magnetometer and azimuth comes from
//    // sensorfusion algo
//    //    if (what.testFlag(MatrixSensorBase::Compass)) {
//    //            compass.setTimestamp((quint64)data.timestamp);
//    //            compass.setAzimuth(toDeg360(data.fusionPose.z()));
//    //                    emit q->compassChanged(compass);
//    //    }

//    if (what.testFlag(MatrixSensorBase::Magnetometer)) {
//        mag.setTimestamp((qreal)data.timestamp);
//        mag.setX((qreal)data.mag_x * .000001);
//        mag.setY((qreal)data.mag_y * .000001);
//        mag.setZ((qreal)data.mag_z * .000001);
//        emit q->magnetometerChanged(mag);
//    }
}

MatrixSensorBase::MatrixSensorBase(QSensor *sensor)
    : QSensorBackend(sensor),
      d_ptr(new QMatrixSensorsPrivate(this))
{
    qDebug() << Q_FUNC_INFO;
    if (d_ptr->open()) {
        d_ptr->pollTimer.setInterval(d_ptr->pollInterval);
        connect(&d_ptr->pollTimer, &QTimer::timeout, [this] { d_ptr->update(sensorFlag); });
    } else
        qWarning() << "Could not open matrix hal";
}

MatrixSensorBase::~MatrixSensorBase()
{
}

void MatrixSensorBase::start()
{
    qDebug() << Q_FUNC_INFO << d_ptr->imuInited;
    if (d_ptr->imuInited)
        d_ptr->pollTimer.start();
    else {
        sensorError(-ENODEV);
        stop();
    }
}

void MatrixSensorBase::stop()
{
    qDebug() << Q_FUNC_INFO << d_ptr->imuInited;
    if (d_ptr->imuInited)
        d_ptr->pollTimer.stop();
    sensorStopped();
}

bool MatrixSensorBase::isFeatureSupported(QSensor::Feature /*feature*/) const
{
    return false;
}

void MatrixSensorBase::poll(MatrixSensorBase::UpdateFlags sensorFlag)
{
    qWarning() << Q_FUNC_INFO;
    d_ptr->update(sensorFlag);
}
