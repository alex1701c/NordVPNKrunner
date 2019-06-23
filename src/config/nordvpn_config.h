/******************************************************************************
 *  Copyright (C) 2013 – 2018 by David Baum <david.baum@naraesk.eu>           *
 *                                                                            *
 *  This library is free software; you can redistribute it and/or modify      *
 *  it under the terms of the GNU Lesser General Public License as published  *
 *  by the Free Software Foundation; either version 2 of the License or (at   *
 *  your option) any later version.                                           *
 *                                                                            *
 *  This library is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         *
 *  Library General Public License for more details.                          *
 *                                                                            *
 *  You should have received a copy of the GNU Lesser General Public License  *
 *  along with this library; see the file COPYING.LIB.                        *
 *  If not, see <http://www.gnu.org/licenses/>.                               *
 *****************************************************************************/

#ifndef NORDVPNCONFIG_H
#define NORDVPNCONFIG_H

#include "ui_nordvpn_config.h"
#include <KCModule>
#include <KConfigCore/KConfigGroup>

class NordVPNConfigForm : public QWidget, public Ui::NordVPNConfigUi {
Q_OBJECT

public:
    explicit NordVPNConfigForm(QWidget *parent);
};

class NordVPNConfig : public KCModule {
Q_OBJECT

public:
    explicit NordVPNConfig(QWidget *parent = nullptr, const QVariantList &args = QVariantList());

    KConfigGroup config;

    QString newIcon;

    const QString defaultIcon = "/usr/share/icons/nordvpn.png";

public Q_SLOTS:

    void save() override;

    void load() override;

    void defaults() override;

    void showExampleStatusNotification();

    void openIconFileChooser();

    void setDefaultIcon();


private:
    NordVPNConfigForm *m_ui;

    void setCurrentSettings();

    QString getStatusNotificationKeys();

    void writeConfigText(QString key, QString text);
};

#endif
