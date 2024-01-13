cmake_minimum_required(VERSION 3.16)

project(infosystemk VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)



find_package(
   Qt6 6.5 REQUIRED COMPONENTS Quick Multimedia Positioning
)

qt_standard_project_setup()

qt_add_executable(appinfosystemk
    main.cpp

)

#set resource files

qt_add_qml_module(appinfosystemk
    URI infosystemk
    VERSION 1.0

    QML_FILES
        Main.qml
        UI/Gmap.qml

        UI/weatherForecast.qml
        UI/WeatherIcon.qml
        UI/CurrentWeatherIcon.qml
        UI/ForecastIcon.qml
        UI/MediaPlayer.qml
        UI/Clock.qml
    RESOURCES
        assets/camera.png
        assets/Car_28_OFF.png
        assets/google-maps.png
        assets/musical-note.png
        assets/settings.png
        assets/thermometer.png
        assets/play-button.png
        assets/pause.png
        assets/play.png
        assets/forward-button.png
        assets/rewind-button.png

        assets/defaultMusic.png
        assets/weather.png

    SOURCES
        mediaplayer.h
        mediaplayer.cpp
        providerapi.h
        providerapi.cpp
        meteomaticsbackend.h
        meteomaticsbackend.cpp
        weathermodel.h
        weathermodel.cpp
        RESOURCES assets/weather/clearsky-day.png assets/weather/clearsky-night.png assets/weather/few-clouds-day.png assets/weather/few-clouds-night.png assets/weather/fog-day.png assets/weather/fog-night.png assets/weather/freezing-rain.png assets/weather/rain.png assets/weather/sleet.png assets/weather/snow.png assets/weather/thunderstorm.png
        RESOURCES assets/sys-night.jpg
        RESOURCES assets/sys-day.jpg
        RESOURCES assets/center.png
        RESOURCES assets/clock-face.png assets/hours.png assets/minutes.png assets/seconds.png
        QML_FILES UI/MiniPlayer.qml
        QML_FILES UI/BackUpCamera.qml
        RESOURCES assets/Camera/guideLine.png
        QML_FILES UI/tester.qml
        RESOURCES UI/meshes/cube_002_mesh.mesh UI/meshes/cube_003_mesh.mesh
        QML_FILES UI/LineGuide.qml
        SOURCES SystemProvider.h SystemProvider.cpp
        RESOURCES assets/bell.png assets/location.png
        RESOURCES assets/world.png
        QML_FILES UI/Settings.qml

)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appinfosystemk PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appinfosystemk
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)


target_link_libraries(appinfosystemk
    PRIVATE Qt6::Quick
            Qt6::Multimedia
            Qt6::Positioning

)

include(GNUInstallDirs)
install(TARGETS appinfosystemk
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
