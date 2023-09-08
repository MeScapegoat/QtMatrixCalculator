#include "TableWidget.h"
#include <string>
#include <iostream>
#include <chrono>
#include <QHeaderView>
#include <QTimer>


TableWidget::TableWidget(Matrix& mat, QWidget *parent)
    :QTableWidget(parent), matrix(mat), precision(6){}

TableWidget::TableWidget(Matrix& mat, const int& prec, QWidget* parent)
    : QTableWidget(parent), matrix(mat), precision(prec)
{
    size_t rows = matrix.row();
    size_t columns = matrix.col();

    setRowCount(rows);
    setColumnCount(columns);
    for (size_t row = 1, rows = matrix.row(); row <= rows; ++row){
        for (size_t col = 1, cols = matrix.col(); col <= cols; ++col){
            QString string_value;
            string_value.setNum(matrix(row,col), 'g', precision);
            QTableWidgetItem* table_value = new QTableWidgetItem(string_value);
            table_value->setTextAlignment(Qt::AlignCenter);
            setItem(row-1,col-1, table_value);
        }
    }

    UpdateWidth();
    for (size_t row = 0, rows = rowCount(); row < rows; ++row)
        setRowHeight(row, 1);

    horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    horizontalHeader()->setStyleSheet("QHeaderView::section {background-color : pink;}");
    verticalHeader()->setStyleSheet("QHeaderView::section {background-color : pink;}");
    setConnections();
    setSize();
}

TableWidget& TableWidget::operator=(TableWidget&& other)
{
    removeConnections();
    matrix = other.matrix;
    precision = other.precision;
    size_t rows = matrix.row();
    size_t columns = matrix.col();
    setRowCount(rows);
    setColumnCount(columns);
    for (size_t row = 1, rows = matrix.row(); row <= rows; ++row){
        for (size_t col = 1, cols = matrix.col(); col <= cols; ++col){
            QString string_value;
            string_value.setNum(matrix(row,col), 'g', precision);
            QTableWidgetItem* table_value = new QTableWidgetItem(string_value);
            table_value->setTextAlignment(Qt::AlignCenter);
            setItem(row-1,col-1, table_value);
        }
    }

    UpdateWidth();
    for (size_t row = 0, rows = rowCount(); row < rows; ++row)
        setRowHeight(row, 1);

    horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    horizontalHeader()->setStyleSheet("QHeaderView::section {background-color : pink;}");
    verticalHeader()->setStyleSheet("QHeaderView::section {background-color : pink;}");

    setConnections();
    setSize();
    return *this;
}
TableWidget& TableWidget::operator=(const TableWidget& other)
{
    removeConnections();
    matrix = other.matrix;
    precision = other.precision;
    size_t rows = matrix.row();
    size_t columns = matrix.col();
    setRowCount(rows);
    setColumnCount(columns);
    for (size_t row = 1, rows = matrix.row(); row <= rows; ++row){
        for (size_t col = 1, cols = matrix.col(); col <= cols; ++col){
            QString string_value;
            string_value.setNum(matrix(row,col), 'g', precision);
            QTableWidgetItem* table_value = new QTableWidgetItem(string_value);
            table_value->setTextAlignment(Qt::AlignCenter);
            setItem(row-1,col-1, table_value);
        }
    }

    UpdateWidth();
    for (size_t row = 0, rows = rowCount(); row < rows; ++row)
        setRowHeight(row, 1);

    horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    horizontalHeader()->setStyleSheet("QHeaderView::section {background-color : pink;}");
    verticalHeader()->setStyleSheet("QHeaderView::section {background-color : pink;}");

    setConnections();
    setSize();
    return *this;
}

void TableWidget::setConnections()
{
    connections.push_back(connect(this, &TableWidget::itemChanged, this, &TableWidget::UpdateMatrixElement));
    connections.push_back(connect(this, &TableWidget::currentItemChanged, this, [&](){ isUserInput = true; }));
}
void TableWidget::removeConnections()
{
    for (auto &connection : connections) disconnect(connection);
}

void TableWidget::UpdateMatrixElement(QTableWidgetItem* item)
{
    QString string_value;
    size_t row = item->row();
    size_t column = item->column();
    double value = item->text().replace(',','.').toDouble();
    if (isUserInput)
    {
        matrix(row+1, column+1) = value;
        isUserInput = false;
    }
    string_value.setNum(value, 'g', precision);
    item->setText(string_value);
    UpdateWidth();
}

void TableWidget::UpdateWidth()
{
    double PixelsPerDigit = 7;
    size_t MaxColumnWidth = 1;
    for (double &el : matrix)
    {
        QString string_value;
        string_value.setNum(el, 'g', precision);
        size_t double_length = (string_value.size() + 1) * PixelsPerDigit;
        MaxColumnWidth = MaxColumnWidth > double_length ? MaxColumnWidth : double_length;
    }
    for (size_t column = 0, columns = columnCount(); column < columns; ++column)
        setColumnWidth(column, MaxColumnWidth);
}
void TableWidget::setSize()
{
    size_t width = verticalHeader()->width() + horizontalHeader()->length() + 2;
    size_t height = horizontalHeader()->height() + verticalHeader()->length() + 2;
    size_t max_width_elements = ((500 - verticalHeader()->width()) / columnWidth(0));
    size_t max_height_elements = ((750 - horizontalHeader()->height()) / rowHeight(0));
    size_t width_limit = max_width_elements * columnWidth(0) + verticalHeader()->width() + 20;
    size_t height_limit = max_height_elements * rowHeight(0) + horizontalHeader()->height() + 20;

    width = width >  width_limit ? width_limit : width;
    height = height >  height_limit ? height_limit : height;
    setFixedSize(width, height);
}
