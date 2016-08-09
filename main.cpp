/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSensors module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU Lesser General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "matrixcompass.h"
#include "matrixaccelerometer.h"
#include "matrixgyroscope.h"
#include "matrixpressuresensor.h"
#include "matrixtemperaturesensor.h"
#include "matrixmagnetometer.h"

#include <qsensorplugin.h>
#include <qsensorbackend.h>
#include <qsensormanager.h>

#include <QtCore/QFile>
#include <QtCore/QDebug>

class QMatrixSensorPlugin : public QObject, public QSensorPluginInterface, public QSensorBackendFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.qt-project.Qt.QSensorPluginInterface/1.0" FILE "plugin.json")
    Q_INTERFACES(QSensorPluginInterface)

public:
    void registerSensors()
    {
        if (!QSensorManager::isBackendRegistered(QCompass::type, MatrixCompass::id))
            QSensorManager::registerBackend(QCompass::type, MatrixCompass::id, this);

        if (!QSensorManager::isBackendRegistered(QAccelerometer::type, MatrixAccelerometer::id))
            QSensorManager::registerBackend(QAccelerometer::type, MatrixAccelerometer::id, this);

        if (!QSensorManager::isBackendRegistered(QGyroscope::type, MatrixGyroscope::id))
            QSensorManager::registerBackend(QGyroscope::type, MatrixGyroscope::id, this);

        if (!QSensorManager::isBackendRegistered(QPressureSensor::type, MatrixPressureSensor::id))
            QSensorManager::registerBackend(QPressureSensor::type, MatrixPressureSensor::id, this);

        if (!QSensorManager::isBackendRegistered(QAmbientTemperatureSensor::type, MatrixTemperatureSensor::id))
            QSensorManager::registerBackend(QAmbientTemperatureSensor::type, MatrixTemperatureSensor::id, this);

        if (!QSensorManager::isBackendRegistered(QMagnetometer::type, MatrixMagnetometer::id))
            QSensorManager::registerBackend(QMagnetometer::type, MatrixMagnetometer::id, this);

    }

    QSensorBackend *createBackend(QSensor *sensor)
    {

        if (sensor->identifier() == MatrixCompass::id)
            return new MatrixCompass(sensor);

        if (sensor->identifier() == MatrixAccelerometer::id)
            return new MatrixAccelerometer(sensor);

        if (sensor->identifier() == MatrixGyroscope::id)
            return new MatrixGyroscope(sensor);

        if (sensor->identifier() == MatrixPressureSensor::id)
            return new MatrixPressureSensor(sensor);

        if (sensor->identifier() == MatrixTemperatureSensor::id)
            return new MatrixTemperatureSensor(sensor);

        if (sensor->identifier() == MatrixMagnetometer::id)
            return new MatrixMagnetometer(sensor);

        return Q_NULLPTR;
    }
};

#include "main.moc"

