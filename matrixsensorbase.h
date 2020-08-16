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
#include <QLightReading>

#include <matrix_hal/matrixio_bus.h>
#include <matrix_hal/imu_data.h>
#include <matrix_hal/imu_sensor.h>

#include <QLoggingCategory>

#include <QTimer>
//Q_DECLARE_LOGGING_CATEGORY(qMatrix)

class QMatrixSensorsPrivate;
class MatrixSensorBase : public QSensorBackend
{
    Q_OBJECT
public:
    enum UpdateFlag {
        Humidity = 0,
        Pressure,
        Temperature,
        Gyro,
        Acceleration,
        Compass,
        Orientation,
        Magnetometer,
        Rotation,
        Altimeter,
        Uv,
        All = 0xFF
    };


    MatrixSensorBase(QSensor *sensor);
    ~MatrixSensorBase();

    QMatrixSensorsPrivate *d_ptr;
    MatrixSensorBase::UpdateFlag sensorFlag;
    void start() Q_DECL_OVERRIDE;
    void stop() Q_DECL_OVERRIDE;
    void poll(MatrixSensorBase::UpdateFlag sensorFlag = All);
    bool isFeatureSupported(QSensor::Feature feature) const Q_DECL_OVERRIDE;

    bool needsImu(MatrixSensorBase::UpdateFlag sensorFlag);

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

    void uvChanged(const QLightReading &value);
};

#endif // MATRIXSENSORBASE_H
