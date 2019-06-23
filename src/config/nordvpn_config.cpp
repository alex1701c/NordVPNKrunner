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
    config = KSharedConfig::openConfig("krunnerrc")->group("Runners").group("NordVPN");

    m_ui->krunnerStatusExampleLabel->hide();
    m_ui->krunnerStatusExample->hide();

    setCurrentSettings();

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

void NordVPNConfig::setCurrentSettings() {
    m_ui->defaultConnectionTarget->setText(config.readEntry("default", "US"));
    m_ui->krunnerStatus->setText(config.readEntry("status", "%st"));
    m_ui->iconButton->setIcon(QIcon(config.readEntry("icon", "/usr/share/icons/nordvpn.png")));
    m_ui->source->setText(config.readEntry("source", "nordvpn status"));
    m_ui->changeScript->setText(config.readEntry("script", ""));
    m_ui->cleanHistory->setChecked(config.readEntry("clean_history", "true") == "true");
    m_ui->notify->setChecked(config.readEntry("notify", "true") == "true");

    QList<QString> values = config.readEntry("status_keys", "Status|Current server|Transfer|Your new IP").split('|');
    m_ui->statusKeysStatus->setChecked(values.contains("Status"));
    m_ui->statusKeysCurrentServer->setChecked(values.contains("Current server"));
    m_ui->statusKeysCountry->setChecked(values.contains("Country"));
    m_ui->statusKeysCity->setChecked(values.contains("City"));
    m_ui->statusKeysNewIP->setChecked(values.contains("Your new IP"));
    m_ui->statusKeysCurrentProtocol->setChecked(values.contains("Current protocol"));
    m_ui->statusKeysTransfer->setChecked(values.contains("Transfer"));
    m_ui->statusKeysUptime->setChecked(values.contains("Uptime"));
}

#include "nordvpn_config.moc"
