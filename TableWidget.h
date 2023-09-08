#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H
#include <vector>
#include <QTableWidget>
#include "Matrix.h"
class TableWidget : public QTableWidget
{
    Q_OBJECT
public:
    TableWidget(Matrix& matrix, QWidget *parent = nullptr);
    TableWidget(Matrix& matrix, const int& precision, QWidget *parent = nullptr);
    TableWidget& operator=(TableWidget&& other);
    TableWidget& operator=(const TableWidget& other);
    ~TableWidget() = default;
private:
    void UpdateMatrixElement(QTableWidgetItem*);
    void UpdateWidth();
    void setConnections();
    void removeConnections();
    void setSize();
private:
    Matrix& matrix;
    int precision{6};
    std::vector<QMetaObject::Connection> connections;
    bool isUserInput{false};
};

#endif // TABLEWIDGET_H
