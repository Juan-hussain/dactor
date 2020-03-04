#include "../headers/mainwindow.h"
#include <QApplication>
#include"../headers/recstatemachine.h"
int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    MainWindow w;
//    QFile File("stylesheet.qss");
//    File.open(QFile::ReadOnly);
//    QString StyleSheet = QLatin1String(File.readAll());
//    w.setStyleSheet(StyleSheet);
    w.show();

    return a.exec();
}
