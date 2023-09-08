#ifndef MENUWINDOWS_H
#define MENUWINDOWS_H

#include <QDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QRegularExpressionValidator>
#include <QRadioButton>
#include <QButtonGroup>
#include <QComboBox>
#include <QFileDialog>
#include "MainWindow.h"

class DialogWindowBase : public QDialog
{
public:
    DialogWindowBase(MainWindow& MainWindow, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~DialogWindowBase() = 0;
public:
    MainWindow& Window;
    QFormLayout layout;
    QHBoxLayout ButtonsLayout;
    QComboBox MatrixSelection;
    QLabel MatrixLabel{"Choose Matrix: "};
    QPushButton AcceptButton{"OK"}, RejectButton{"Cancel"};
};

class InfoWindowBase : public QDialog
{
public:
    InfoWindowBase(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~InfoWindowBase() = 0;
public:
    QFormLayout layout;
    QHBoxLayout ButtonsLayout;
    QLabel TextLabel;
    QPushButton AcceptButton{"OK"};
};

class SaveLoadMatrixWindowBase : public DialogWindowBase
{
public:
    SaveLoadMatrixWindowBase(MainWindow& MainWindow, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~SaveLoadMatrixWindowBase();
public:
    QLabel PathLabel, FileNameLabel{"Enter file name:"};
    QLineEdit PathLineEdit, FileNameLineEdit;
    QPushButton ChangePathButton{"Change..."};
    QRegularExpressionValidator Validator{QRegularExpression("^[a-zA-Z0-9_-]*$")};
public:
    void LineEditAdjustWidth();
private:
    virtual void ChangePath() = 0;
    virtual void ValidateInput() = 0;
};

class CreateMatrixWindow : public DialogWindowBase
{
public:
    CreateMatrixWindow(MainWindow& MainWindow, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~CreateMatrixWindow();
public:
    QLabel  RowsLabel{"Amount of Rows:"},
        ColumnsLabel{"Amount of Columns:"},
        ValueLabel{"Filled with value:"},
        IdentityLabel{"Make identity"},
        Matrixlabel{"Create Matrix at "};
    QCheckBox IdentityCheckBox;
    QLineEdit   RowsLineEdit,
        ColumnsLineEdit,
        ValueLineEdit;
    QIntValidator intValidator{1,999};
    QDoubleValidator doubleValidator;
private:
    void ValidateInput();
    void UpdateOnIdentity();
};

class SaveMatrixWindow : public SaveLoadMatrixWindowBase
{
public:
    SaveMatrixWindow(MainWindow& MainWindow, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~SaveMatrixWindow();
private:
    void ChangePath() override;
    void ValidateInput() override;
};

class LoadMatrixWindow : public SaveLoadMatrixWindowBase
{
public:
    LoadMatrixWindow(MainWindow& MainWindow, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~LoadMatrixWindow();
private:
    void ChangePath() override;
    void ValidateInput() override;
};

class SetPrecisionWindow : public DialogWindowBase
{
public:
    SetPrecisionWindow(MainWindow& MainWindow, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~SetPrecisionWindow();
public:
    QLabel  PrecisionLabel{"Amount of significant digits to show:"};
    QLineEdit   PrecisionLineEdit;
    QIntValidator intValidator{1,99};
private:
    void ValidateInput();
    void ShowPrecision(QString);
};

class DeterminantWindow : public DialogWindowBase
{
public:
    DeterminantWindow(MainWindow& MainWindow, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~DeterminantWindow();
public:
    QLabel  message{"Calculate Determinant of"};
};

class MinorWindow : public DialogWindowBase
{
public:
    MinorWindow(MainWindow& MainWindow, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~MinorWindow();
public:
    QLabel  message{"Calculate Minor of"},
        RowLabel{"Row:"},
        ColumnLabel{"Column:"};
    QLineEdit   RowLineEdit,
                ColumnLineEdit;
    QIntValidator intValidator{1,999};
public:
    void ValidateInput();
};

class InverseWindow : public DialogWindowBase
{
public:
    InverseWindow(MainWindow& MainWindow, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~InverseWindow();
public:
    QLabel  message{"Calculate Inverse of"};
public:
    void ValidateInput();
};

class TransposeWindow : public DialogWindowBase
{
public:
    TransposeWindow(MainWindow& MainWindow, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~TransposeWindow();
public:
    QLabel  message{"Calculate Transpose of"};
};

class IdentityWindow : public DialogWindowBase
{
public:
    IdentityWindow(MainWindow& MainWindow, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~IdentityWindow();
public:
    QLabel  message{"make identity of"};
public:
    void ValidateInput();
};

class ChangeElementAmountWindow : public DialogWindowBase
{
public:
    ChangeElementAmountWindow(MainWindow& MainWindow, QString Type, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~ChangeElementAmountWindow();
public:
    QString Type{"column"};
    QLabel  message{QString("Change %1 amount of").arg(Type)},
            ElemLabel{QString("Starting at %1:").arg(Type)},
            AmountLabel{QString("Amount of %1s").arg(Type)},
            ModeLabel{"Mode:"};
    QComboBox ModeSelection;
    QLineEdit   ElemLineEdit,
                AmountLineEdit;
    QIntValidator intValidator{1,999};
public:
    void ValidateInput();
};

class SwapMatricesWindow : public DialogWindowBase
{
public:
    SwapMatricesWindow(MainWindow& MainWindow, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~SwapMatricesWindow();
public:
    QLabel  SecondMatrixLabel{"With"};
    QComboBox SecondMatrixSelection;
};

class SwapElementsWindow : public DialogWindowBase
{
public:
    SwapElementsWindow(MainWindow& MainWindow, QString Type, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~SwapElementsWindow();
public:
    QString Type{"columns"};
    QLabel  message{QString("Swap %1").arg(Type)},
            FirstLabel{"First:"},
            SecondLabel{"Second:"};
    QLineEdit   FirstLineEdit,
                SecondLineEdit;
    QIntValidator intValidator{1,999};
public:
    void ValidateInput();
};

class ActionsOnElementWindow : public DialogWindowBase
{
public:
    ActionsOnElementWindow(MainWindow& MainWindow, QString Type, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~ActionsOnElementWindow();
public:
    QString Type{"column"};
    QComboBox ActionSelection, OperandSelection;
    QLabel  message{QString("Actions on %1").arg(Type)},
            ElementLabel{QString("index of %1:").arg(Type)},
            ActionLabel{"Action:"},
            OperationLabel{"With:"},
            OperandLabel{"Value:"};
    QLineEdit   ElementLineEdit,
                OperandLineEdit;
    QIntValidator intValidator{1,999};
    QDoubleValidator doubleValidator;
public:
    void ValidateInput();
    void UpdateOperand(const QString& Operand);
};

class HowToUseWindow : public InfoWindowBase
{
public:
    HowToUseWindow(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~HowToUseWindow();
};

class AuthorWindow : public InfoWindowBase
{
public:
    AuthorWindow(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~AuthorWindow();
public:
    QLabel LinkLabel;
};

#endif // MENUWINDOWS_H
