#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QDialog>
#include <QMessageBox>

class Notification : public QDialog
{
public:
    Notification(const QString& title, const QString& text);
    ~Notification() = default;
};

class ErrorMessage : public QMessageBox
{
public:
    ErrorMessage(const QString& title, const QString& text);
    ~ErrorMessage() = default;
};

class InfoMessage : public QMessageBox
{
public:
    InfoMessage(const QString& title, const QString& text);
    ~InfoMessage() = default;
};

#endif // NOTIFICATION_H
