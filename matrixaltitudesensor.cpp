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

#include "matrixaltitudesensor.h"
#include "matrixsensorbase.h"
#include <QDebug>

char const * const MatrixAccelerometer::id("matrix.altimeter");

MatrixAltimeter::MatrixAltimeter(QSensor *sensor)
    : MatrixSensorBase(sensor)
{
    sensorFlag = MatrixSensorBase::Altimeter;
    setReading<QAltimeterReading>(&m_reading);
    qDebug() << Q_FUNC_INFO;
    connect(this,SIGNAL(altitudeChanged(QAltimeterReading)),this,SLOT(altitudeChanged(QAltimeterReading)));
}


void MatrixAltimeter::altitudeChanged(const QAltimeterReading &value)
{
    m_reading.setAltitude(value.altitude());
    m_reading.setTimestamp(value.timestamp());
    newReadingAvailable();
}
