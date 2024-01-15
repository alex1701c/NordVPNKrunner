add_definitions(-DTRANSLATION_DOMAIN=\"plasma_runner_org.kde.nordvpn\")

add_subdirectory(core)

add_library(krunner_nordvpn MODULE runner/nordvpn.cpp runner/Match.cpp)
target_link_libraries(krunner_nordvpn KF${QT_MAJOR_VERSION}::Runner KF${QT_MAJOR_VERSION}::I18n)
target_link_libraries(krunner_nordvpn core_api_static)

ki18n_wrap_ui(kcm_krunner_nordvpn_SRCS config/nordvpn_config.ui)
add_library(kcm_krunner_nordvpn MODULE config/nordvpn_config.cpp ${kcm_krunner_nordvpn_SRCS})
target_link_libraries(kcm_krunner_nordvpn
    Qt::Core
    Qt::Gui
    KF${QT_MAJOR_VERSION}::CoreAddons
    KF${QT_MAJOR_VERSION}::ConfigCore
    KF${QT_MAJOR_VERSION}::I18n
    KF${QT_MAJOR_VERSION}::KCMUtils
    KF${QT_MAJOR_VERSION}::Runner
)
target_link_libraries(kcm_krunner_nordvpn core_api_static)

install(TARGETS krunner_nordvpn DESTINATION ${KDE_INSTALL_QTPLUGINDIR}/kf${QT_MAJOR_VERSION}/krunner/)
install(TARGETS kcm_krunner_nordvpn DESTINATION ${KDE_INSTALL_QTPLUGINDIR})
configure_file(runner/plasma-runner-nordvpn.json.in plasma-runner-nordvpn.json)

