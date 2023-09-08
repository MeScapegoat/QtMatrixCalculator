#include "notification.h"
#include <QVBoxLayout>
#include <QLabel>
Notification::Notification(const QString& title, const QString& text) : QDialog()
{
    setWindowIcon(QIcon(":/icons/icons/wait192.png"));
    QVBoxLayout* layout = new QVBoxLayout(this);
    QLabel* label = new QLabel(text, this);
    setWindowFlag(Qt::WindowCloseButtonHint, false);
    setWindowModality(Qt::ApplicationModal);
    setFixedSize(240,40);
    setLayout(layout);
    setWindowTitle(title);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label, 0, Qt::AlignCenter);
    adjustSize();
    show();
}

ErrorMessage::ErrorMessage(const QString& title, const QString& text) : QMessageBox(QMessageBox::Critical, title, text)
{
    setWindowIcon(QIcon(":/icons/icons/error192.png"));
    //setIconPixmap(QPixmap(":/icons/icons/error192.png"));
    adjustSize();
    exec();
}

InfoMessage::InfoMessage(const QString& title, const QString& text) : QMessageBox(QMessageBox::Information, title, text)
{
    setWindowIcon(QIcon(":/icons/icons/info192.png"));
    //setIconPixmap(QPixmap(":/icons/icons/info192.png"));
    adjustSize();
    exec();
}
