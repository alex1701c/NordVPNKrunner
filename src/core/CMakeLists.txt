set(core_api_SRCS Status.cpp NotificationManager.cpp)

add_library(core_api_static STATIC ${core_api_SRCS})

target_link_libraries(core_api_static KF5::ConfigCore KF5::Notifications)