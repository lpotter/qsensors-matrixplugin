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

#ifndef MATRIXMAGNETOMETER_H
#define MATRIXMAGNETOMETER_H

#include <QObject>
#include "matrixsensorbase.h"
#include <QMagnetometerReading>

class MatrixMagnetometer : public MatrixSensorBase
{
    Q_OBJECT
public:
    MatrixMagnetometer(QSensor *sensor);
    static char const * const id;

private slots:
    void magChanged(const QMagnetometerReading &value);
private:
    QMagnetometerReading m_reading;

};

#endif // MATRIXMAGNETOMETER_H
