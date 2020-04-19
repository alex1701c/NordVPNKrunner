add_definitions(-DTRANSLATION_DOMAIN=\"plasma_runner_org.kde.nordvpn\")

add_subdirectory(core)

set(nordvpn_SRCS runner/nordvpn.cpp runner/Match.cpp)

add_library(krunner_nordvpn MODULE ${nordvpn_SRCS})
target_link_libraries(krunner_nordvpn KF5::Runner KF5::I18n)
target_link_libraries(krunner_nordvpn core_api_static)

set(kcm_krunner_nordvpn_SRCS
        config/nordvpn_config.cpp
        )

ki18n_wrap_ui(kcm_krunner_nordvpn_SRCS config/nordvpn_config.ui)
add_library(kcm_krunner_nordvpn MODULE ${kcm_krunner_nordvpn_SRCS})
target_link_libraries(kcm_krunner_nordvpn
        Qt5::Core
        Qt5::Gui
        KF5::CoreAddons
        KF5::ConfigCore
        KF5::I18n
        KF5::ConfigWidgets
        KF5::Runner
        )
target_link_libraries(kcm_krunner_nordvpn core_api_static)

install(TARGETS krunner_nordvpn kcm_krunner_nordvpn DESTINATION ${KDE_INSTALL_QTPLUGINDIR})
install(FILES config/plasma-runner-nordvpn_config.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})

configure_file(runner/plasma-runner-nordvpn.desktop.in runner/plasma-runner-nordvpn.desktop)
install(FILES ${CMAKE_BINARY_DIR}/src/runner/plasma-runner-nordvpn.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
