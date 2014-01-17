#-------------------------------------------------
#
# Project created by QtCreator 2013-02-03T22:32:33
#
#-------------------------------------------------

QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QuizMaster
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    element.cpp \
    answer.cpp \
    question.cpp \
    quiz.cpp \
    user.cpp \
    userslist.cpp \
    quizlist.cpp \
    loginwindow.cpp \
    userwindow.cpp \
    selectquizwindow.cpp

HEADERS  += mainwindow.h \
    element.h \
    commonHeaders.h \
    answer.h \
    question.h \
    quiz.h \
    user.h \
    userslist.h \
    quizlist.h \
    loginwindow.h \
    userwindow.h \
    selectquizwindow.h

FORMS    += mainwindow.ui \
    loginwindow.ui \
    userwindow.ui \
    selectquizwindow.ui
