#include <fstream>

#include <MainWindow.h>
#include <QCoreApplication>
#include <Qfuture>
#include <QtConcurrent/QtConcurrent>
#include <QHeaderView>

#include "notification.h"

void MainWindow::CreateMatrix(const QString& MatrixName,
                              const size_t& row, const size_t& column,
                              const double& value, const bool& isIdentity)
{
    Matrix matrix;
    if (MatrixList.find(MatrixName) == MatrixList.end())
    {
        CreateMatrix(LeftMatrix.Name.text(), row, column, value, isIdentity);
        CreateMatrix(RightMatrix.Name.text(), row, column, value, isIdentity);
    }
    else
    {
        Notification notif("Creating Matrix...", "Please wait...");
        QCoreApplication::processEvents();
        MatrixTable& mat = *MatrixList[MatrixName];
        QFuture<int> future = QtConcurrent::run([&]() {
            if (isIdentity)
            {
                matrix = Matrix(row, row , 0);
                matrix.make_identity();
            }
            else
            {
                matrix = Matrix(row, column, value);
            }
            mat.data = matrix;
            return 0;
        });
        while (!future.isFinished())
            QCoreApplication::processEvents();
        future.result();
        UpdateTable(mat);
        notif.hide();
    }
}

void MainWindow::LoadMatrix(const QString& MatrixName, const QString& filepath)
{
    MatrixTable& mat = *MatrixList[MatrixName];
    std::ifstream file;
    file.open(filepath.toStdString());
    file>>mat.data;
    file.close();
    mat.Description.setText(QFileInfo(filepath).fileName());
    UpdateTable(mat);
}

void MainWindow::SaveMatrix(const QString& MatrixName, const QString& filepath)
{
    MatrixTable& mat = *MatrixList[MatrixName];
    std::ofstream file;
    file.open(filepath.toStdString());
    file << mat.data;
    file.close();
}

void MainWindow::SetPrecision(const QString& MatrixName, const size_t& precision)
{
    MatrixTable& mat = *MatrixList[MatrixName];
    mat.precision = precision;
    UpdateTable(mat);
}

void MainWindow::Determinant(const QString& MatrixName)
{
    MatrixTable& mat = *MatrixList[MatrixName];

    Notification notif("Calculating Determinant...", "Please wait for the end of calculations...");
    QCoreApplication::processEvents();
    QFuture<double> future = QtConcurrent::run([&]() {
        return mat.data.det();
    });
    while (!future.isFinished()) {
        QCoreApplication::processEvents();
    }
    QString info = "Determinant is %1.";
    notif.hide();
    InfoMessage("Determinant", info.arg(future.result()));
}

void MainWindow::Minor(const QString& MatrixName, const size_t& row, const size_t& column)
{
    MatrixTable& mat = *MatrixList[MatrixName];

    Notification notif("Calculating Minor...", "Please wait for the end of calculations...");
    QCoreApplication::processEvents();
    QFuture<double> future = QtConcurrent::run([&]() {
        return mat.data.minor(row, column);
    });
    while (!future.isFinished()) {
        QCoreApplication::processEvents();
    }
    QString info = "Minor is %1.";
    notif.hide();
    InfoMessage("Minor", info.arg(future.result()));

}

void MainWindow::Inverse(const QString& MatrixName)
{
    MatrixTable& mat = *MatrixList[MatrixName];
    QString announcement = "Inverse of %1:";

    Notification notif("Calculating Inverse...", "Please wait for the end of calculations...");
    QCoreApplication::processEvents();
    QFuture<Matrix> future = QtConcurrent::run([&]() {
        return Matrix(mat.data.inverse());
    });
    while (!future.isFinished()) {
        QCoreApplication::processEvents();
    }
    AnswerMatrix.data = future.result();
    UpdateTable(AnswerMatrix);
    AnswerMatrix.Log.setText(announcement.arg(MatrixName));
    AnswerMatrix.Log.show();
    notif.hide();
}

void MainWindow::Transpose(const QString& MatrixName)
{
    MatrixTable& mat = *MatrixList[MatrixName];
    QString announcement = "Transpose of %1:";

    Notification notif("Calculating Transpose...", "Please wait for the end of calculations...");
    QCoreApplication::processEvents();
    QFuture<Matrix> future = QtConcurrent::run([&]() {
        return Matrix(mat.data.transpose());
    });
    while (!future.isFinished()) {
        QCoreApplication::processEvents();
    }
    AnswerMatrix.data = future.result();
    UpdateTable(AnswerMatrix);
    AnswerMatrix.Log.setText(announcement.arg(MatrixName));
    AnswerMatrix.Log.show();
    notif.hide();
}

void MainWindow::Identity(const QString& MatrixName)
{
    MatrixTable& mat = *MatrixList[MatrixName];

    Notification notif("Calculating identity...", "Please wait for the end of calculations...");
    QCoreApplication::processEvents();
    QFuture<Matrix> future = QtConcurrent::run([&]() {
        return Matrix(mat.data.make_identity());
    });
    while (!future.isFinished()) {
        QCoreApplication::processEvents();
    }
    mat.data = future.result();
    UpdateTable(mat);
    notif.hide();
}


void MainWindow::ChangeElementAmount(const QString& MatrixName, const QString& Type,
                                     const QString& Mode, const size_t& start, const size_t& amount)
{
    MatrixTable& mat = *MatrixList[MatrixName];

    Notification notif("Changing Matrix Size...", "Please wait...");
    QCoreApplication::processEvents();
    QFuture<int> future = QtConcurrent::run([&]() {
        if (Type == "column")
        {
            if (Mode == "Add")
            {
                mat.data.add_cols(start, amount);
            }
            else
            {
                mat.data.remove_cols(start, amount);
            }
        }
        else
        {
            if (Mode == "Add")
            {
                mat.data.add_rows(start, amount);
            }
            else
            {
                mat.data.remove_rows(start, amount);
            }
        }
        return 0;
    });
    while (!future.isFinished()) {
        QCoreApplication::processEvents();
    }
    future.result();
    UpdateTable(mat);
    notif.hide();
}

void MainWindow::SwapMatrices(const QString& first, const QString& second)
{
    if (first == second) return;
    MatrixTable& mat1 = *MatrixList[first],
        &mat2 = *MatrixList[second];

    auto tmp_data = mat1.data;
    mat1.data = mat2.data;
    mat2.data = tmp_data;

    auto tmp_precision = mat1.precision;
    mat1.precision = mat2.precision;
    mat2.precision = tmp_precision;

    auto tmp_DescriptionText = mat1.Description.text();
    mat1.Description.setText(mat2.Description.text());
    mat2.Description.setText(tmp_DescriptionText);

    mat1.Log.setText("Swapped with " + mat2.Name.text());
    mat1.Log.show();
    mat2.Log.setText("Swapped with " + mat1.Name.text());
    mat2.Log.show();
    UpdateTable(mat1);
    UpdateTable(mat2);

}

void MainWindow::SwapElements(const QString& MatrixName, const QString& Type, const size_t& first, const size_t& second)
{
    MatrixTable& mat = *MatrixList[MatrixName];
    Notification notif("Swapping Matrix Elements...", "Please wait...");
    QCoreApplication::processEvents();
    QFuture<int> future = QtConcurrent::run([&]() {
        if (Type == "columns")
        {
            mat.data.swap_cols(first, second);
        }
        else
        {
            mat.data.swap_rows(first, second);
        }
        return 0;
    });
    while (!future.isFinished()) {
        QCoreApplication::processEvents();
    }
    future.result();
    UpdateTable(mat);
    notif.hide();
}

void MainWindow::ActionsOnElement(const QString& MatrixName, const QString& Type, const size_t& element,
                                  const QString& action, const QString& operand_type, const double& operand_value)
{
    MatrixTable& mat = *MatrixList[MatrixName];
    Notification notif("Calculating Actions...", "Please wait...");
    QCoreApplication::processEvents();
    QFuture<int> future = QtConcurrent::run([&]() {
        //MY APOLOGIES FOR THE CODE BELOW
        if (Type == "column")
        {
            if (action == "+")
            {
                if (operand_type == "value")
                {
                    mat.data.col_add(element, operand_value);
                }
                else
                {
                    mat.data.sum_cols(element, operand_value);
                }
            }
            else if (action == "-")
            {
                if (operand_type == "value")
                {
                    mat.data.col_sub(element, operand_value);
                }
                else
                {
                    mat.data.sub_cols(element, operand_value);
                }
            }
            else if (action == "x")
            {
                mat.data.col_mult(element, operand_value);
            }
            else
            {
                mat.data.col_divide(element, operand_value);
            }
        }
        else
        {
            if (action == "+")
            {
                if (operand_type == "value")
                {
                    mat.data.row_add(element, operand_value);
                }
                else
                {
                    mat.data.sum_rows(element, operand_value);
                }
            }
            else if (action == "-")
            {
                if (operand_type == "value")
                {
                    mat.data.row_sub(element, operand_value);
                }
                else
                {
                    mat.data.sub_rows(element, operand_value);
                }
            }
            else if (action == "x")
            {
                mat.data.row_mult(element, operand_value);
            }
            else
            {
                mat.data.row_divide(element, operand_value);
            }
        }
        return 0;
    });
    while (!future.isFinished()) {
        QCoreApplication::processEvents();
    }
    future.result();
    UpdateTable(mat);
    notif.hide();
}

void MainWindow::sum()
{
    if (LeftMatrix.data.row() != RightMatrix.data.row() or LeftMatrix.data.col() != RightMatrix.data.col())
    {
        ErrorMessage("Error", "Matricies of different size");
        return;
    }
    AnswerMatrix.data = LeftMatrix.data + RightMatrix.data;
    AnswerMatrix.Log.setText("Sum of Left Matrix and Right Matrix:");
    AnswerMatrix.Log.show();
    UpdateTable(AnswerMatrix);
}

void MainWindow::sub()
{
    if (LeftMatrix.data.row() != RightMatrix.data.row() or LeftMatrix.data.col() != RightMatrix.data.col())
    {
        ErrorMessage("Error", "Matricies of different size");
        return;
    }
    AnswerMatrix.data = LeftMatrix.data - RightMatrix.data;
    AnswerMatrix.Log.setText("Difference of Left Matrix and Right Matrix:");
    AnswerMatrix.Log.show();
    UpdateTable(AnswerMatrix);
}
void MainWindow::mult()
{
    if (LeftMatrix.data.col() != RightMatrix.data.row())
    {
        ErrorMessage("Error", "Matricies can not be multiplied");
        return;
    }
    AnswerMatrix.data = LeftMatrix.data * RightMatrix.data;
    AnswerMatrix.Log.setText("Product of Left Matrix and Right Matrix:");
    AnswerMatrix.Log.show();
    UpdateTable(AnswerMatrix);
}

void MainWindow::UpdateTable(MatrixTable& mat)
{
    mat.view = TableWidget(mat.data, mat.precision, this);
    mat.place.addWidget(&mat.view, 0, Qt::AlignHCenter);
    mat.Name.show();
    mat.Description.show();
    if (mat.Name.text() == AnswerMatrix.Name.text())
        EqualityPlace.itemAt(0)->widget()->show();
    if (isMatrixPresent(LeftMatrix) and isMatrixPresent(RightMatrix))
        for (auto counter = 0; counter < OperationsPlace.count(); ++counter)
            OperationsPlace.itemAt(counter)->widget()->show();
}

bool MainWindow::isMatrixPresent(MatrixTable& mat)
{
    if (mat.data.empty()){
        return false;
    }
    return true;
}

bool MainWindow::isMatrixSquare(MatrixTable& mat)
{
    if(!isMatrixPresent(mat)){
        return false;
    }
    else{
        if (!mat.data.is_square()){
            return false;
        }
    }
    return true;
}

void MainWindow::AlarmMatrixSquare()
{
    ErrorMessage("Error", "The matrix is not square!");
}

void MainWindow::AlarmMatrixPresent()
{
    ErrorMessage("Error", "Create matrix first!");
}
