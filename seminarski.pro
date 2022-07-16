QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += C:\OpenCV\opencv\build\include
INCLUDEPATH += C:\Program Files\SICK\I2D\Develop\Lib
INCLUDEPATH += "C:\Program Files\SICK\I2D\USB driver package"

LIBS += C:\OpenCV\opencv\release\bin\libopencv_core454.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_highgui454.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_imgcodecs454.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_imgproc454.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_calib3d454.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_video454.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_videoio454.dll
LIBS += "C:\Program Files\SICK\I2D\USB driver package\uc480_64.dll"
LIBS += "C:\Program Files\SICK\I2D\USB driver package\uc480_tools_64.dll"





SOURCES += \
    Stereovizija.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    IDSVideoCapture.h \
    ueye.h
