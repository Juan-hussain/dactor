#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog
{
    Q_OBJECT

public:

    explicit EditDialog(QWidget *parent = nullptr);
    ~EditDialog();
    void setText(QString text,Qt::Alignment textAlignment, QFont font);
    QString getText();

signals:
    void apply();
private slots:
    void on_closeButton_clicked();

    void on_applyButton_clicked();

    void on_text_minus_clicked();

    void on_text_plus_clicked();

    void on_rtl_clicked();

    void on_ltr_clicked();

private:
    Ui::EditDialog *ui;
    QFont textfont;
    const int min_font_size = 10;
    const int max_font_size = 34;

    void updateTextSize(int value);


};

#endif // EDITDIALOG_H
