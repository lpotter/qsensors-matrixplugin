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

#include "matrixgyroscope.h"
#include "matrixsensorbase.h"


char const * const MatrixGyroscope::id("matrix.gyroscope");

MatrixGyroscope::MatrixGyroscope(QSensor *sensor)
    : MatrixSensorBase(sensor)

{
    sensorFlag = MatrixSensorBase::Gyro;
    setReading<QGyroscopeReading>(&m_reading);
    qDebug() << Q_FUNC_INFO;
    connect(this,SIGNAL(gyroChanged(QGyroscopeReading)),this,SLOT(gyroChanged(QGyroscopeReading)));
}


void MatrixGyroscope::gyroChanged(const QGyroscopeReading &value)
{
    m_reading.setX(value.x());
    m_reading.setY(value.y());
    m_reading.setZ(value.z());
    m_reading.setTimestamp(value.timestamp());
    newReadingAvailable();
}
