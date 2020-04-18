#include "nordvpn_config.h"
#include "core/Status.h"
#include <KSharedConfig>
#include <KPluginFactory>
#include <krunner/abstractrunner.h>
#include <QtDebug>
#include <QtWidgets/QGridLayout>
#include <iostream>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFileDialog>
#include <core/NotificationManager.h>

#include "kcmutils_version.h"

K_PLUGIN_FACTORY(NordVPNConfigFactory, registerPlugin<NordVPNConfig>("kcm_krunner_nordvpn");)

NordVPNConfigForm::NordVPNConfigForm(QWidget *parent) : QWidget(parent) {
    setupUi(this);
}

NordVPNConfig::NordVPNConfig(QWidget *parent, const QVariantList &args) : KCModule(parent, args) {
    m_ui = new NordVPNConfigForm(this);
    auto *layout = new QGridLayout(this);
    layout->addWidget(m_ui, 0, 0);
    config = KSharedConfig::openConfig("krunnerrc")->group("Runners").group("NordVPN");

#if KCMUTILS_VERSION >= QT_VERSION_CHECK(5, 64, 0)
    const auto changedSlotPointer = &NordVPNConfig::markAsChanged;
#else
    const auto changedSlotPointer = static_cast<void (NordVPNConfig::*)()>(&NordVPNConfig::changed);
#endif

    m_ui->krunnerStatusExampleLabel->hide();
    m_ui->krunnerStatusExample->hide();
    m_ui->iconDefaultButton->setHidden(config.readEntry("icon", defaultIcon) == defaultIcon);
    m_ui->statusKeysStatus->setEnabled(false);
    connect(m_ui->defaultConnectionTarget, &QLineEdit::textChanged, this, changedSlotPointer);
    connect(m_ui->krunnerStatus, &QLineEdit::textChanged, this, changedSlotPointer);
    connect(m_ui->krunnerStatus, &QLineEdit::textChanged, this, &NordVPNConfig::exampleStatus);
    connect(m_ui->source, &QLineEdit::textChanged, this, changedSlotPointer);
    connect(m_ui->changeScript, &QLineEdit::textChanged, this, changedSlotPointer);

    connect(m_ui->notify, &QCheckBox::clicked, this, changedSlotPointer);
    connect(m_ui->statusKeysStatus, &QCheckBox::clicked, this, changedSlotPointer);
    connect(m_ui->statusKeysCurrentServer, &QCheckBox::clicked, this, changedSlotPointer);
    connect(m_ui->statusKeysCountry, &QCheckBox::clicked, this, changedSlotPointer);
    connect(m_ui->statusKeysCity, &QCheckBox::clicked, this, changedSlotPointer);
    connect(m_ui->statusKeysNewIP, &QCheckBox::clicked, this, changedSlotPointer);
    connect(m_ui->statusKeysCurrentProtocol, &QCheckBox::clicked, this, changedSlotPointer);
    connect(m_ui->statusKeysTransfer, &QCheckBox::clicked, this, changedSlotPointer);
    connect(m_ui->statusKeysUptime, &QCheckBox::clicked, this, changedSlotPointer);

    connect(m_ui->statusKeysExampleNotification, &QPushButton::clicked, [this]() {
        NotificationManager::displayStatusNotification(exampleData, getStatusNotificationKeys());
    });
    connect(m_ui->iconButton, &QPushButton::clicked, this, &NordVPNConfig::openIconFileChooser);
    connect(m_ui->iconDefaultButton, &QPushButton::clicked, this, &NordVPNConfig::setDefaultIcon);
}

void NordVPNConfig::load() {
    setCurrentSettings();
    emit changed(false);
}


void NordVPNConfig::save() {
    writeConfigText("default", m_ui->defaultConnectionTarget->text());
    writeConfigText("status", m_ui->krunnerStatus->text());
    writeConfigText("source", m_ui->source->text());
    writeConfigText("script", m_ui->changeScript->text());

    config.writeEntry("notify", m_ui->notify->isChecked());
    if (!newIcon.isEmpty()) config.writeEntry("icon", newIcon);
    config.writeEntry("status_filter_keys", getStatusNotificationKeys());

    config.config()->sync();

    emit changed(true);
}

void NordVPNConfig::defaults() {
    m_ui->defaultConnectionTarget->setText("US");
    m_ui->krunnerStatus->setText("%STATUS");
    m_ui->iconButton->setIcon(QIcon(defaultIcon));
    newIcon = defaultIcon;
    m_ui->source->setText("nordvpn status");
    m_ui->changeScript->setText("");
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
    m_ui->krunnerStatus->setText(config.readEntry("status", "%STATUS"));
    m_ui->iconButton->setIcon(QIcon::fromTheme(config.readEntry("icon", defaultIcon)));
    m_ui->source->setText(config.readEntry("source", "nordvpn status"));
    m_ui->changeScript->setText(config.readEntry("script"));
    m_ui->notify->setChecked(config.readEntry("notify", true));

    auto values = config.readEntry("status_filter_keys", QStringList());
    if (values.isEmpty()) {
        values = QStringList({"Status", "Current server", "Transfer", "Your new IP"});
    }
    m_ui->statusKeysStatus->setChecked(values.contains("Status"));
    m_ui->statusKeysCurrentServer->setChecked(values.contains("Current server"));
    m_ui->statusKeysCountry->setChecked(values.contains("Country"));
    m_ui->statusKeysCity->setChecked(values.contains("City"));
    m_ui->statusKeysNewIP->setChecked(values.contains("Your new IP"));
    m_ui->statusKeysCurrentProtocol->setChecked(values.contains("Current protocol"));
    m_ui->statusKeysTransfer->setChecked(values.contains("Transfer"));
    m_ui->statusKeysUptime->setChecked(values.contains("Uptime"));
}

QStringList NordVPNConfig::getStatusNotificationKeys() {
    QStringList keys;
    if (m_ui->statusKeysStatus->isChecked()) keys.append("Status");
    if (m_ui->statusKeysCurrentServer->isChecked()) keys.append("Current server");
    if (m_ui->statusKeysCountry->isChecked()) keys.append("Country");
    if (m_ui->statusKeysCity->isChecked()) keys.append("City");
    if (m_ui->statusKeysNewIP->isChecked()) keys.append("Your new IP");
    if (m_ui->statusKeysCurrentProtocol->isChecked()) keys.append("Current protocol");
    if (m_ui->statusKeysTransfer->isChecked()) keys.append("Transfer");
    if (m_ui->statusKeysUptime->isChecked()) keys.append("Uptime");
    return keys;
}

void NordVPNConfig::exampleStatus() {
    m_ui->krunnerStatusExampleLabel->setHidden(false);
    m_ui->krunnerStatusExample->setHidden(false);
    const auto status = Status::objectFromRawData(exampleData);
    m_ui->krunnerStatusExample->setText(status.formatString(m_ui->krunnerStatus->text()));
}

void NordVPNConfig::writeConfigText(const QString &key, const QString &text) {
    if (text.isEmpty()) {
        config.deleteEntry(key);
    } else {
        config.writeEntry(key, text);
    }
}

void NordVPNConfig::openIconFileChooser() {
    const QString iconPath = QFileDialog::getOpenFileName(this, tr("Select Icon"), "",
                                                          tr("Images (.*.jpg *.jpeg *.png *.ico *.svg *.svgz)"));
    if (!iconPath.isEmpty()) {
        newIcon = iconPath;
        m_ui->iconButton->setIcon(QIcon(newIcon));
        m_ui->iconDefaultButton->setHidden(newIcon == defaultIcon);
        m_ui->iconDefaultButton->clearFocus();
        changed(true);
    }
    m_ui->iconButton->clearFocus();
}

void NordVPNConfig::setDefaultIcon() {
    m_ui->iconButton->setIcon(QIcon::fromTheme(defaultIcon));
    m_ui->iconDefaultButton->setHidden(true);
    newIcon = defaultIcon;
    m_ui->iconButton->clearFocus();
    changed(true);
}

#include "nordvpn_config.moc"
