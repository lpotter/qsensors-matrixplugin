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

#ifndef MATRIXSENSORBASE_H
#define MATRIXSENSORBASE_H

#include <QObject>
#include <QSensorBackend>
#include <QCompassReading>
#include <QGyroscopeReading>
#include <QAccelerometerReading>
#include <QOrientationReading>
#include <QMagnetometerReading>
#include <QRotationReading>
#include <QAmbientTemperatureReading>
#include <QPressureReading>
#include <QAltimeterReading>

#include <matrix_hal/imu_data.h>
#include <matrix_hal/imu_sensor.h>
#include <matrix_hal/wishbone_bus.h>

#include <QLoggingCategory>

#include <QTimer>
//Q_DECLARE_LOGGING_CATEGORY(qMatrix)

class QMatrixSensorsPrivate;
class MatrixSensorBase : public QSensorBackend
{
    Q_OBJECT
public:
    enum UpdateFlag {
        Humidity = 0x01,
        Pressure = 0x02,
        Temperature = 0x04,
        Gyro = 0x08,
        Acceleration = 0x10,
        Compass = 0x20,
        Orientation = 0x40,
        Magnetometer = 0x80,
        Rotation = 0x170,
        Altimeter = 0x350,
        All = 0xFF
    };
    Q_DECLARE_FLAGS(UpdateFlags, UpdateFlag)

    MatrixSensorBase(QSensor *sensor);
    ~MatrixSensorBase();

    QMatrixSensorsPrivate *d_ptr;
    MatrixSensorBase::UpdateFlags sensorFlag;
    void start() Q_DECL_OVERRIDE;
    void stop() Q_DECL_OVERRIDE;
    void poll(MatrixSensorBase::UpdateFlags sensorFlag = All);
    bool isFeatureSupported(QSensor::Feature feature) const Q_DECL_OVERRIDE;

signals:
    void humidityChanged(qreal value);
    void pressureChanged(const QPressureReading &value);
    void temperatureChanged(const QAmbientTemperatureReading &value);
    void gyroChanged(const QGyroscopeReading &value);
    void accelerationChanged(const QAccelerometerReading &value);
    void compassChanged(const QCompassReading &value);
    void orientationChanged(const QOrientationReading &value);//
    void magnetometerChanged(const QMagnetometerReading &value);
    void rotationChanged(const QRotationReading &value);//
    void altitudeChanged(const QAltimeterReading &value);
private:
};

#endif // MATRIXSENSORBASE_H
