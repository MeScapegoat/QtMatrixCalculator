#include <QLabel>
#include <QLayout>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QDebug>

#include "MenuWindows.h"
#include "MainWindow.h"
#include "notification.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    LeftMatrix(6 , "Left Matrix", "Custom Left Matrix Text"),
    RightMatrix(6 , "Right Matrix", "Custom Right Matrix Text"),
    AnswerMatrix(6 , "Answer Matrix", "Custom Answer Matrix Text")
{
    QWidget* centralwidget = new QWidget;
    QHBoxLayout* layout =  new QHBoxLayout;

    setWindowTitle("Matrix Calculator");
    setWindowIcon(QIcon(":/icons/icons/app192.png"));
    setMenuBar(&menu);
    setCentralWidget(centralwidget);
    setMinimumSize(600,200);

    centralwidget->setLayout(layout);
    layout->addLayout(&LeftMatrix.place);
    layout->addLayout(&OperationsPlace);
    layout->addLayout(&RightMatrix.place);
    layout->addLayout(&EqualityPlace);
    layout->addLayout(&AnswerMatrix.place);

    LeftMatrix.place.addWidget(&LeftMatrix.Name, 0, Qt::AlignTop);
    LeftMatrix.place.addWidget(&LeftMatrix.Description, 0, Qt::AlignTop);
    LeftMatrix.place.addWidget(&LeftMatrix.Log, 0, Qt::AlignTop);
    LeftMatrix.Name.hide();
    LeftMatrix.Description.hide();
    LeftMatrix.Log.hide();

    QPushButton *PlusButton = new QPushButton("+"),
                *MinusButton = new QPushButton("-"),
                *MultButton = new QPushButton("*");
    OperationsPlace.addWidget(PlusButton, 0, Qt::AlignCenter);
    OperationsPlace.addWidget(MinusButton, 0, Qt::AlignCenter);
    OperationsPlace.addWidget(MultButton, 0, Qt::AlignCenter);
    for (auto counter = 0; counter < OperationsPlace.count(); ++counter)
        OperationsPlace.itemAt(counter)->widget()->hide();

    RightMatrix.place.addWidget(&RightMatrix.Name, 0, Qt::AlignTop);
    RightMatrix.place.addWidget(&RightMatrix.Description, 0, Qt::AlignTop);
    RightMatrix.place.addWidget(&RightMatrix.Log, 0, Qt::AlignTop);
    RightMatrix.Name.hide();
    RightMatrix.Description.hide();
    RightMatrix.Log.hide();

    QLabel *EqualityLabel = new QLabel("=");
    EqualityPlace.addWidget(EqualityLabel, 0, Qt::AlignCenter);
    EqualityPlace.itemAt(0)->widget()->hide();

    AnswerMatrix.place.addWidget(&AnswerMatrix.Name, 0, Qt::AlignTop);
    AnswerMatrix.place.addWidget(&AnswerMatrix.Description, 0, Qt::AlignTop);
    AnswerMatrix.place.addWidget(&AnswerMatrix.Log, 0, Qt::AlignTop);
    AnswerMatrix.Name.hide();
    AnswerMatrix.Description.hide();
    AnswerMatrix.Log.hide();

    MatrixList.insert({LeftMatrix.Name.text(), &LeftMatrix});
    MatrixList.insert({RightMatrix.Name.text(), &RightMatrix});
    MatrixList.insert({AnswerMatrix.Name.text(), &AnswerMatrix});

    auto MenuActions = menu.actions();
    auto MatrixMenuActions = MenuActions[0]->menu()->actions();
    auto OnMatrixMenuActions = MenuActions[1]->menu()->actions();
    auto OnElementsMenuActions = MenuActions[2]->menu()->actions();
    auto AboutMenuActions = MenuActions[3]->menu()->actions();

    connect(PlusButton, &QPushButton::clicked, this, &MainWindow::sum);
    connect(MinusButton, &QPushButton::clicked, this, &MainWindow::sub);
    connect(MultButton, &QPushButton::clicked, this, &MainWindow::mult);

    connect(MatrixMenuActions[0], &QAction::triggered, this, &MainWindow::OpenCreateMatrixWindow);
    connect(MatrixMenuActions[1], &QAction::triggered, this, &MainWindow::OpenLoadMatrixWindow);
    connect(MatrixMenuActions[2], &QAction::triggered, this, &MainWindow::OpenSaveMatrixWindow);
    connect(MatrixMenuActions[3], &QAction::triggered, this, &MainWindow::OpenSetPrecisionWindow);

    connect(OnMatrixMenuActions[0], &QAction::triggered, this, &MainWindow::OpenDeterminantWindow);
    connect(OnMatrixMenuActions[1], &QAction::triggered, this, &MainWindow::OpenMinorWindow);
    connect(OnMatrixMenuActions[2], &QAction::triggered, this, &MainWindow::OpenInverseWindow);
    connect(OnMatrixMenuActions[3], &QAction::triggered, this, &MainWindow::OpenTransposeWindow);
    connect(OnMatrixMenuActions[4], &QAction::triggered, this, &MainWindow::OpenIdentityWindow);
    connect(OnMatrixMenuActions[5], &QAction::triggered, this, &MainWindow::OpenChangeColumnAmountWindow);
    connect(OnMatrixMenuActions[6], &QAction::triggered, this, &MainWindow::OpenChangeRowAmountWindow);
    connect(OnMatrixMenuActions[7], &QAction::triggered, this, &MainWindow::OpenSwapMatricesWindow);

    connect(OnElementsMenuActions[0], &QAction::triggered, this, &MainWindow::OpenSwapColumnsWindow);
    connect(OnElementsMenuActions[1], &QAction::triggered, this, &MainWindow::OpenSwapRowsWindow);
    connect(OnElementsMenuActions[2], &QAction::triggered, this, &MainWindow::OpenActionsOnColumnWindow);
    connect(OnElementsMenuActions[3], &QAction::triggered, this, &MainWindow::OpenActionsOnRowWindow);

    connect(AboutMenuActions[0], &QAction::triggered, this, &MainWindow::OpenHowToUseWindow);
    connect(AboutMenuActions[1], &QAction::triggered, this, &MainWindow::OpenAuthorWindow);
}

MainWindow::~MainWindow(){}

void MainWindow::OpenCreateMatrixWindow()
{
    CreateMatrixWindow window(*this);
    if (window.exec() == QDialog::Accepted)
    {
        size_t  row_amount = window.RowsLineEdit.text().toULongLong(),
                column_amount = window.ColumnsLineEdit.text().toULongLong();
        double value = window.ValueLineEdit.text().replace(',','.').toDouble();
        bool isIdentity= window.IdentityCheckBox.isChecked() ? true : false;
        QString MatrixName = window.MatrixSelection.currentText();
        CreateMatrix(MatrixName, row_amount, column_amount, value, isIdentity);
    }
}

void MainWindow::OpenLoadMatrixWindow()
{
    LoadMatrixWindow window(*this);
    if (window.exec() == QDialog::Accepted)
    {
        auto MatrixName = window.MatrixSelection.currentText();
        auto filepath = window.PathLineEdit.text();
        LoadMatrix(MatrixName, filepath);
    }
}

void MainWindow::OpenSaveMatrixWindow()
{
    if (!isMatrixPresent(LeftMatrix) and !isMatrixPresent(RightMatrix) and !isMatrixPresent(AnswerMatrix))
    {
        ErrorMessage("Error", "No Matrix to save");
        return;
    }
    SaveMatrixWindow window(*this);
    if (window.exec() == QDialog::Accepted)
    {
        auto MatrixName = window.MatrixSelection.currentText();
        auto filepath = window.PathLineEdit.text() + "/" + window.FileNameLineEdit.text() + ".matrix";
        SaveMatrix(MatrixName, filepath);
    }
}

void MainWindow::OpenSetPrecisionWindow()
{
    if (!(isMatrixPresent(LeftMatrix) or isMatrixPresent(RightMatrix) or isMatrixPresent(AnswerMatrix)))
    {
        ErrorMessage("Error", "Create matrix first!");
        return;
    }
    SetPrecisionWindow window(*this);
    if (window.exec() == QDialog::Accepted)
    {
        auto MatrixName = window.MatrixSelection.currentText();
        auto precision = window.PrecisionLineEdit.text().toULongLong();
        SetPrecision(MatrixName, precision);
    }
}

void MainWindow::OpenDeterminantWindow()
{
    if (!isMatrixPresent(LeftMatrix) and !isMatrixPresent(RightMatrix) and !isMatrixPresent(AnswerMatrix))
    {
        AlarmMatrixPresent();
        return;
    }
    DeterminantWindow window(*this);
    if (window.exec() == QDialog::Accepted)
    {
        auto MatrixName = window.MatrixSelection.currentText();
        Determinant(MatrixName);
    }
}

void MainWindow::OpenMinorWindow()
{
    if (!isMatrixPresent(LeftMatrix) and !isMatrixPresent(RightMatrix) and !isMatrixPresent(AnswerMatrix))
    {
        AlarmMatrixPresent();
        return;
    }
    MinorWindow window(*this);
    if (window.exec() == QDialog::Accepted)
    {
        auto MatrixName = window.MatrixSelection.currentText();
        auto row = window.RowLineEdit.text().toULongLong();
        auto column = window.ColumnLineEdit.text().toULongLong();
        Minor(MatrixName, row, column);
    }
}

void MainWindow::OpenInverseWindow()
{
    if (!isMatrixPresent(LeftMatrix) and !isMatrixPresent(RightMatrix))
    {
        AlarmMatrixPresent();
        return;
    }
    InverseWindow window(*this);
    if (window.exec() == QDialog::Accepted)
    {
        auto MatrixName = window.MatrixSelection.currentText();
        Inverse(MatrixName);
    }
}

void MainWindow::OpenTransposeWindow()
{
    if (!isMatrixPresent(LeftMatrix) and !isMatrixPresent(RightMatrix))
    {
        AlarmMatrixPresent();
        return;
    }
    TransposeWindow window(*this);
    if (window.exec() == QDialog::Accepted)
    {
        auto MatrixName = window.MatrixSelection.currentText();
        Transpose(MatrixName);
    }
}

void MainWindow::OpenIdentityWindow()
{
    if (!isMatrixPresent(LeftMatrix) and !isMatrixPresent(RightMatrix))
    {
        AlarmMatrixPresent();
        return;
    }
    IdentityWindow window(*this);
    if (window.exec() == QDialog::Accepted)
    {
        auto MatrixName = window.MatrixSelection.currentText();
        Identity(MatrixName);
    }
}

void MainWindow::OpenChangeElementAmountWindow(const QString& Type)
{
    if (!isMatrixPresent(LeftMatrix) and !isMatrixPresent(RightMatrix))
    {
        AlarmMatrixPresent();
        return;
    }
    ChangeElementAmountWindow window(*this, Type);
    if (window.exec() == QDialog::Accepted)
    {
        auto MatrixName = window.MatrixSelection.currentText();
        auto Mode = window.ModeSelection.currentText();
        auto start = window.ElemLineEdit.text().toULongLong();
        auto amount = window.AmountLineEdit.text().toULongLong();
        ChangeElementAmount(MatrixName, Type, Mode, start, amount);
    }
}

void MainWindow::OpenChangeColumnAmountWindow()
{
    OpenChangeElementAmountWindow("column");
}

void MainWindow::OpenChangeRowAmountWindow()
{
    OpenChangeElementAmountWindow("row");
}

void MainWindow::OpenSwapMatricesWindow()
{
    if (!isMatrixPresent(LeftMatrix) and !isMatrixPresent(RightMatrix))
    {
        AlarmMatrixPresent();
        return;
    }
    SwapMatricesWindow window(*this);
    if (window.exec() == QDialog::Accepted)
    {
        auto first = window.MatrixSelection.currentText();
        auto second = window.SecondMatrixSelection.currentText();
        SwapMatrices(first, second);
    }
}

void MainWindow::OpenSwapElementsWindow(const QString& Type)
{
    if (!isMatrixPresent(LeftMatrix) and !isMatrixPresent(RightMatrix))
    {
        AlarmMatrixPresent();
        return;
    }
    SwapElementsWindow window(*this, Type);
    if (window.exec() == QDialog::Accepted)
    {
        auto MatrixName = window.MatrixSelection.currentText();
        auto first = window.FirstLineEdit.text().toULongLong();
        auto second = window.SecondLineEdit.text().toULongLong();
        SwapElements(MatrixName, Type, first, second);
    }
}

void MainWindow::OpenSwapColumnsWindow()
{
    OpenSwapElementsWindow("columns");
}

void MainWindow::OpenSwapRowsWindow()
{
    OpenSwapElementsWindow("rows");
}

void MainWindow::OpenActionsOnElementWindow(const QString& Type)
{
    if (!isMatrixPresent(LeftMatrix) and !isMatrixPresent(RightMatrix))
    {
        AlarmMatrixPresent();
        return;
    }
    ActionsOnElementWindow window(*this, Type);
    if (window.exec() == QDialog::Accepted)
    {
        auto MatrixName = window.MatrixSelection.currentText();
        auto element = window.ElementLineEdit.text().toULongLong();
        auto action = window.ActionSelection.currentText();
        auto operand_type = window.OperandSelection.currentText();
        auto operand_value = window.OperandLineEdit.text().toDouble();
        ActionsOnElement(MatrixName, Type, element, action, operand_type, operand_value);
    }
}

void MainWindow::OpenActionsOnColumnWindow()
{
    OpenActionsOnElementWindow("column");
}

void MainWindow::OpenActionsOnRowWindow()
{
    OpenActionsOnElementWindow("row");
}

void MainWindow::OpenHowToUseWindow()
{
    HowToUseWindow window;
    window.exec();
}

void MainWindow::OpenAuthorWindow()
{
    AuthorWindow window;
    window.exec();
}


