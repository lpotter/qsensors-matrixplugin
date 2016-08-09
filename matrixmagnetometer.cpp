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

#include "matrixmagnetometer.h"
#include "matrixsensorbase.h"
#include <QDebug>

char const * const MatrixMagnetometer::id("matrix.magnetometer");

MatrixMagnetometer::MatrixMagnetometer(QSensor *sensor)
    : MatrixSensorBase(sensor)
{
    sensorFlag = MatrixSensorBase::Magnetometer;
    setReading<QMagnetometerReading>(&m_reading);
    qDebug() << Q_FUNC_INFO;
    connect(this,SIGNAL(magnetometerChanged(QMagnetometerReading)),this,SLOT(magChanged(QMagnetometerReading)));
}


void MatrixMagnetometer::magChanged(const QMagnetometerReading &value)
{
    m_reading.setX(value.x());
    m_reading.setY(value.y());
    m_reading.setZ(value.z());
    m_reading.setTimestamp(value.timestamp());
    newReadingAvailable();
}
