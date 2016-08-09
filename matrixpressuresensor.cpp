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

#include "matrixpressuresensor.h"
#include "matrixsensorbase.h"
#include <QDebug>

char const * const MatrixPressureSensor::id("matrix.pressure");


MatrixPressureSensor::MatrixPressureSensor(QSensor *sensor)
    : MatrixSensorBase(sensor)
{
    sensorFlag = MatrixSensorBase::Pressure;
    setReading<QPressureReading>(&m_reading);
    qDebug() << Q_FUNC_INFO;
    connect(this,SIGNAL(pressureChanged(QPressureReading)),this,SLOT(pressureChanged(QPressureReading)));
}


void MatrixPressureSensor::pressureChanged(const QPressureReading &value)
{
    m_reading.setPressure(value.pressure());
  //  m_reading.setTemperature(value.y());
    m_reading.setTimestamp(value.timestamp());
    newReadingAvailable();
}
