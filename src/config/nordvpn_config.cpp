/******************************************************************************
 *  Copyright (C) 2013 – 2018                                                 *
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
#include <QtWidgets/QFileDialog>

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
    m_ui->iconDefaultButton->hide();
    m_ui->iconManually->hide();

    connect(m_ui->defaultConnectionTarget, SIGNAL(textChanged(QString)), this, SLOT(changed()));
    connect(m_ui->krunnerStatus, SIGNAL(textChanged(QString)), this, SLOT(changed()));
    connect(m_ui->source, SIGNAL(textChanged(QString)), this, SLOT(changed()));
    connect(m_ui->changeScript, SIGNAL(textChanged(QString)), this, SLOT(changed()));

    connect(m_ui->cleanHistory, SIGNAL(clicked(bool)), this, SLOT(changed()));
    connect(m_ui->notify, SIGNAL(clicked(bool)), this, SLOT(changed()));
    connect(m_ui->statusKeysStatus, SIGNAL(clicked(bool)), this, SLOT(changed()));
    connect(m_ui->statusKeysCurrentServer, SIGNAL(clicked(bool)), this, SLOT(changed()));
    connect(m_ui->statusKeysCountry, SIGNAL(clicked(bool)), this, SLOT(changed()));
    connect(m_ui->statusKeysCity, SIGNAL(clicked(bool)), this, SLOT(changed()));
    connect(m_ui->statusKeysNewIP, SIGNAL(clicked(bool)), this, SLOT(changed()));
    connect(m_ui->statusKeysCurrentProtocol, SIGNAL(clicked(bool)), this, SLOT(changed()));
    connect(m_ui->statusKeysTransfer, SIGNAL(clicked(bool)), this, SLOT(changed()));
    connect(m_ui->statusKeysUptime, SIGNAL(clicked(bool)), this, SLOT(changed()));

    connect(m_ui->statusKeysExampleNotification, SIGNAL(clicked(bool)), this, SLOT(showExampleStatusNotification()));
    connect(m_ui->iconButton, SIGNAL(clicked(bool)), this, SLOT(openIconFileChooser()));

    load();

}

void NordVPNConfig::load() {
    KCModule::load();
    setCurrentSettings();
    emit changed(false);
}


void NordVPNConfig::save() {

    KCModule::save();
    writeConfigText("default", m_ui->defaultConnectionTarget->text());
    writeConfigText("status", m_ui->krunnerStatus->text());
    writeConfigText("source", m_ui->source->text());
    writeConfigText("script", m_ui->changeScript->text());

    config.writeEntry("clean_history", m_ui->cleanHistory->isChecked());
    config.writeEntry("notify", m_ui->notify->isChecked());
    if (!newIcon.isEmpty()) config.writeEntry("icon", newIcon);
    config.writeEntry("status_keys", getStatusNotificationKeys());

    emit changed(true);
}

void NordVPNConfig::defaults() {
    m_ui->defaultConnectionTarget->setText("US");
    m_ui->krunnerStatus->setText("%st");
    m_ui->iconButton->setIcon(QIcon("/usr/share/icons/nordvpn.png"));
    newIcon = "/usr/share/icons/nordvpn.png";
    m_ui->source->setText("nordvpn status");
    m_ui->changeScript->setText("");
    m_ui->cleanHistory->setChecked(true);
    m_ui->notify->setChecked(true);

    m_ui->statusKeysStatus->setChecked(true);
    m_ui->statusKeysCurrentServer->setChecked(true);
    m_ui->statusKeysCountry->setChecked(false);
    m_ui->statusKeysCity->setChecked(false);
    m_ui->statusKeysNewIP->setChecked(true);
    m_ui->statusKeysCurrentProtocol->setChecked(false);
    m_ui->statusKeysTransfer->setChecked(true);
    m_ui->statusKeysUptime->setChecked(false);
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

QString NordVPNConfig::getStatusNotificationKeys() {
    QList<QString> keys;
    if (m_ui->statusKeysStatus->isChecked()) keys.append("Status");
    if (m_ui->statusKeysCurrentServer->isChecked()) keys.append("Current server");
    if (m_ui->statusKeysCountry->isChecked()) keys.append("Country");
    if (m_ui->statusKeysCity->isChecked()) keys.append("City");
    if (m_ui->statusKeysNewIP->isChecked()) keys.append("Your new IP");
    if (m_ui->statusKeysCurrentProtocol->isChecked()) keys.append("Current protocol");
    if (m_ui->statusKeysTransfer->isChecked()) keys.append("Transfer");
    if (m_ui->statusKeysUptime->isChecked()) keys.append("Uptime");
    return keys.join('|');
}

void NordVPNConfig::showExampleStatusNotification() {
    QString cmd = QString(
            "$($(vpnStatus=$(nordvpn status 2>&1 | grep -i -E '%1');" "notify-send  \"$vpnStatus\" --icon %2 )) 2>&1 &")
            .arg(getStatusNotificationKeys())
            .arg(config.readEntry("icon", "/usr/share/icons/nordvpn.png"));
    system(qPrintable(cmd));
}

void NordVPNConfig::writeConfigText(QString key, QString text) {
    if (text.isEmpty()) {
        config.deleteEntry(key);
    } else {
        config.writeEntry(key, text);
    }
}

void NordVPNConfig::openIconFileChooser() {
    QString iconPath = QFileDialog::getOpenFileName(this, tr("Select Icon"), "",
                                                    tr("Images (.*.jpg *.jpeg *.png *.ico *.svg *.svgz)"));
    if (!iconPath.isEmpty()) {
        newIcon = iconPath;
        m_ui->iconButton->setIcon(QIcon(newIcon));
        m_ui->iconButton->clearFocus();
        changed(true);
    }
}

#include "nordvpn_config.moc"
