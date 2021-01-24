#include "../headers/editdialog.h"
#include "ui_editdialog.h"

EditDialog::EditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditDialog)
{
    ui->setupUi(this);
}

EditDialog::~EditDialog()
{
    delete ui;
}

void EditDialog::setText(QString text,Qt::Alignment textAlignment, QFont font)
{
    this->textfont=font;
    ui->textEdit->setText(text);
    ui->textEdit->setAlignment(textAlignment);
    QTextCursor cursor = ui->textEdit->textCursor();
    ui->textEdit->selectAll();
    ui->textEdit->setCurrentFont(font);
    ui->textEdit->setTextCursor( cursor );

}

QString EditDialog::getText()
{
    return ui->textEdit->toPlainText();
}

void EditDialog::on_closeButton_clicked()
{
    this->close();
}

void EditDialog::on_applyButton_clicked()
{
    emit apply();
}

void EditDialog::updateTextSize(int value)
{
    // update the font size
    textfont.setPointSize(qBound(min_font_size, textfont.pointSize() + value, max_font_size));
    // set the new font to the table
    QTextCursor cursor = ui->textEdit->textCursor();
    ui->textEdit->selectAll();
    ui->textEdit->setCurrentFont(textfont);
    //ui->textEdit->setFontPointSize(32);
    ui->textEdit->setTextCursor( cursor );
}
// function to increment the size of the text when the appropriate button is clicked
void EditDialog::on_text_plus_clicked()
{
    updateTextSize(1);
}


// decrement the size of the text when the appropriate button is clicked
void EditDialog::on_text_minus_clicked()
{
    updateTextSize(-1);
}


void EditDialog::on_ltr_clicked()
{
    ui->textEdit->setAlignment(Qt::AlignLeft);
}

void EditDialog::on_rtl_clicked()
{
    ui->textEdit->setAlignment(Qt::AlignRight);
}
