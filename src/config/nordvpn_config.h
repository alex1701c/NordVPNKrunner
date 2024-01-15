#pragma once

#include "ui_nordvpn_config.h"
#include <KCModule>
#include <KConfigGroup>
#include <kconfigwidgets_version.h>

class NordVPNConfigForm : public QWidget, public Ui::NordVPNConfigUi
{
    Q_OBJECT

public:
    explicit NordVPNConfigForm(QWidget *parent);
};

class NordVPNConfig : public KCModule
{
    Q_OBJECT

public:
    explicit NordVPNConfig(QObject *parent, const QVariantList &);

public Q_SLOTS:

    void save() override;
    void load() override;
    void defaults() override;

    void exampleStatus();

    void openIconFileChooser();
    void setDefaultIcon();

private:
#if KCONFIGWIDGETS_VERSION < QT_VERSION_CHECK(5, 105, 0)
    QWidget widget()
    {
        return this;
    }
#endif
    void setCurrentSettings();
    QStringList getStatusNotificationKeys();
    void writeConfigText(const QString &key, const QString &text);

    NordVPNConfigForm *m_ui;
    KConfigGroup config;
    QString newIcon;
    const QString defaultIcon = "nordvpn";
    const QString exampleData = QStringLiteral(
        "Status: Connected\n"
        "Current server: us4276.nordvpn.com\n"
        "Country: United States\n"
        "City: Manassas\n"
        "Your new IP: 23.82.14.23\n"
        "Current protocol: UDP\n"
        "Transfer: 26.9 KiB received, 8.6 KiB sent\n"
        "Uptime: 7 seconds");
};
