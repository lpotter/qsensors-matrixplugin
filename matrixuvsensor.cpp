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

#include "matrixuvsensor.h"
#include "matrixsensorbase.h"
#include <QDebug>

char const * const MatrixUvSensor::id("matrix.lightsensor");


MatrixUvSensor::MatrixUvSensor(QSensor *sensor)
    : MatrixSensorBase(sensor)
{
    sensorFlag = MatrixSensorBase::Uv;
    setReading<QLightReading>(&m_reading);
    qDebug() << Q_FUNC_INFO;
    connect(this,SIGNAL(uvChanged(QLightReading)),this,SLOT(luxChanged(QLightReading)));
}


void MatrixUvSensor::luxChanged(const QLightReading &value)
{
    m_reading.setLux(value.lux());
  //  m_reading.setTemperature(value.y());
    m_reading.setTimestamp(value.timestamp());
    newReadingAvailable();
}
