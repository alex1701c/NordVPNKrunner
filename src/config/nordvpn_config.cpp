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

#include "nordvpn_config.h"
#include <KSharedConfig>
#include <KPluginFactory>
#include <krunner/abstractrunner.h>
#include <QtDebug>
#include <QtWidgets/QGridLayout>
#include <iostream>
#include <QtWidgets/QComboBox>

K_PLUGIN_FACTORY(NordVPNConfigFactory, registerPlugin<NordVPNConfig>("kcm_krunner_nordvpn");)

NordVPNConfigForm::NordVPNConfigForm(QWidget *parent) : QWidget(parent) {
    setupUi(this);
}

NordVPNConfig::NordVPNConfig(QWidget *parent, const QVariantList &args) : KCModule(parent, args) {
    m_ui = new NordVPNConfigForm(this);
    auto *layout = new QGridLayout(this);
    layout->addWidget(m_ui, 0, 0);

    load();

}

void NordVPNConfig::load() {
    KCModule::load();
    emit changed(false);
}


void NordVPNConfig::save() {

    KCModule::save();


    emit changed(false);
}

void NordVPNConfig::defaults() {

    emit changed(true);
}

#include "nordvpn_config.moc"
