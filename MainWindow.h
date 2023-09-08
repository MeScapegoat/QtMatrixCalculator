#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <functional>
#include <unordered_map>

#include <QMainWindow>
#include <QWidget>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>

#include "Matrix.h"
#include "MenuBar.h"
#include "TableWidget.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public:
    struct MatrixTable
    {
        Matrix data;
        TableWidget view{data};
        int precision;
        QLabel  Name,
                Log;
        QLineEdit Description;
        QVBoxLayout place;
        MatrixTable() : data(), view(data), precision(), Name(), Log(), Description(), place() {}
        MatrixTable(int precision, QString N, QString Descript)
            : data(), view(data), precision(precision), Name(N), Log(), Description(Descript), place()
        {
            Name.hide();
            Name.setAlignment(Qt::AlignCenter);
            Description.hide();
            Description.setAlignment(Qt::AlignCenter);
            Log.hide();
            Log.setAlignment(Qt::AlignCenter);
        }
    };
public:
    MenuBar menu;
    QVBoxLayout OperationsPlace,
                EqualityPlace;
    MatrixTable     LeftMatrix,
                    RightMatrix,
                    AnswerMatrix;
    std::unordered_map<QString, MatrixTable*> MatrixList;
public:
    void UpdateTable(MatrixTable& mat);
    bool isMatrixPresent(MatrixTable& mat);
    bool isMatrixSquare(MatrixTable& mat);
    void AlarmMatrixPresent();
    void AlarmMatrixSquare();
private slots:
    void OpenCreateMatrixWindow();
    void OpenSaveMatrixWindow();
    void OpenLoadMatrixWindow();
    void OpenSetPrecisionWindow();

    void OpenDeterminantWindow();
    void OpenMinorWindow();
    void OpenInverseWindow();
    void OpenTransposeWindow();
    void OpenIdentityWindow();
    void OpenChangeElementAmountWindow(const QString& Type);
    void OpenChangeColumnAmountWindow();
    void OpenChangeRowAmountWindow();
    void OpenSwapMatricesWindow();

    void OpenSwapElementsWindow(const QString& Type);
    void OpenSwapColumnsWindow();
    void OpenSwapRowsWindow();
    void OpenActionsOnElementWindow(const QString& Type);
    void OpenActionsOnColumnWindow();
    void OpenActionsOnRowWindow();

    void OpenHowToUseWindow();
    void OpenAuthorWindow();
private:
    void CreateMatrix(const QString& MatrixName, const size_t& row, const size_t& column,
                      const double& value, const bool& isOpenIdentityWindow);
    void LoadMatrix(const QString& MatrixName, const QString& filepath);
    void SaveMatrix(const QString& MatrixName, const QString& filepath);
    void SetPrecision(const QString& MatrixName, const size_t& precision);

    void Determinant(const QString& MatrixName);
    void Minor(const QString& MatrixName, const size_t& row, const size_t& column);
    void Inverse(const QString& MatrixName);
    void Transpose(const QString& MatrixName);
    void Identity(const QString& MatrixName);
    void ChangeElementAmount(const QString& MatrixName,const QString& Type,
                             const QString& Mode, const size_t& start, const size_t& amount);
    void SwapMatrices(const QString& first, const QString& second);

    void SwapElements(const QString& MatrixName, const QString& Type, const size_t& first, const size_t& second);
    void ActionsOnElement(const QString& MatrixName, const QString& Type, const size_t& element,
                          const QString& action, const QString& Operand_type, const double& operand_value);
    void sum();
    void sub();
    void mult();
};

#endif // MAINWINDOW_H
