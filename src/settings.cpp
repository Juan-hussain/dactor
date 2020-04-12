#include "../headers/settings.h"
#include "ui_settings.h"
#include "../headers/mainwindow.h"

#include<iostream>

using namespace std;

static QString selectedDevice;

// define settings file
static QSettings savedSettings("RELATER", "settings");


//const string sep = ((QString)QDir::separator()).toStdString();

settings::settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settings)
{
    ui->setupUi(this);
    QString lastTextDir = savedSettings.value("directory").toString();
    ui->textDir->setText(lastTextDir);
}

settings::~settings()
{
    delete ui;
}

QString settings::getSelectedAudioInput()
{
    return selectedDevice;
}

QString settings::getSelectedText()
{
    return selectedText;
}


void settings::on_save_accepted()
{
}

//void settings::closeEvent(QCloseEvent *event)
//{
//    qDebug()<<event;
//}

void settings::on_browseButton_clicked()
{
    selectedText = QFileDialog::getOpenFileName(this, tr("Open Directory"), savedSettings.value("directory").toString(),
                                                    tr("Text files (*.txt);;STM files (*.stm)"));
    QFileInfo txtFile_info(selectedText);
    if (selectedText!="") {
        savedSettings.setValue("directory",txtFile_info.dir().absolutePath());
    }
//    if (!selectedTextDirectory.isEmpty())
//        ui->textDir->setText(selectedTextDirectory);
    this->close();
}
