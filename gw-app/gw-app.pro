#-------------------------------------------------
#
# Project created by QtCreator 2019-07-01T14:05:56
#
#-------------------------------------------------

QT       += core

QT       -= gui

QT      += network
QT      += sql
QT      += xml

TARGET = gw-app
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    backstagemanager.cpp \
    system.cpp \
    busbox/busuart.cpp \
    busbox/busbox.cpp \
    can/receivethread.cpp \
    can/canport.cpp \
    can/canmanager.cpp \
    common/systemutils.cpp \
    common/fileutils.cpp \
    eeprom/eeprom.cpp \
    file/line.cpp \
    file/advertising.cpp \
    gpio/gpio.cpp \
    gpio/gpio_c.c \
    gps/gpsport.cpp \
    gps/gps.cpp \
    lcd/lcduart.cpp \
    lcd/lcdmanager.cpp \
    led/ledworker.cpp \
    led/ledsendlist.cpp \
    led/ledmanager.cpp \
    led/fileupdatelist.cpp \
    network/socketclient.cpp \
    network/networkmessage.cpp \
    network/networkmanager.cpp \
    network/httpworker.cpp \
    network/httptask.cpp \
    network/downloadqueue.cpp \
    pos/posuart.cpp \
    pos/posmanager.cpp \
    sim/simport.cpp \
    uart/serialport.cpp \
    mydaemon.cpp

HEADERS += \
    backstagemanager.h \
    type.h \
    system.h \
    busbox/busuart.h \
    busbox/busbox.h \
    can/receivethread.h \
    can/canport.h \
    can/canmanager.h \
    common/systemutils.h \
    common/mydebug.h \
    common/message.h \
    common/fileutils.h \
    eeprom/eeprom.h \
    file/line.h \
    file/advertising.h \
    gpio/gpio.h \
    gpio/gpio.c.autosave \
    gpio/gpio_c.h \
    gps/gpsport.h \
    gps/gps.h \
    lcd/lcduart.h \
    lcd/lcdmanager.h \
    led/ledworker.h \
    led/ledsendlist.h \
    led/ledmanager.h \
    led/fileupdatelist.h \
    network/socketconstant.h \
    network/socketclient.h \
    network/networkmessage.h \
    network/networkmanager.h \
    network/httpworker.h \
    network/httptask.h \
    network/downloadqueue.h \
    pos/posuart.h \
    pos/posmanager.h \
    sim/simport.h \
    uart/serialport.h \
    constant.h \
    mydaemon.h
