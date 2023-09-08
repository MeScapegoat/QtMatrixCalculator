#include "notification.h"
#include "MenuWindows.h"


//DialogWindowBase
DialogWindowBase::DialogWindowBase(MainWindow& MainWindow, QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f), Window(MainWindow)
{
    setWindowFlag(Qt::MSWindowsFixedSizeDialogHint, true);
    setSizeGripEnabled(false);
    setLayout(&layout);

    layout.setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    layout.addRow(&MatrixLabel, &MatrixSelection);
    layout.addRow(&ButtonsLayout);

    ButtonsLayout.addWidget(&AcceptButton, 0, Qt::AlignCenter);
    ButtonsLayout.addWidget(&RejectButton, 0, Qt::AlignCenter);

    MatrixSelection.setMinimumWidth(100);
    QObject::connect(&RejectButton, &QPushButton::pressed, this, &QDialog::reject);
}

DialogWindowBase::~DialogWindowBase(){}
//DialogWindowBase

//InfoWindowBase
InfoWindowBase::InfoWindowBase(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    setWindowFlag(Qt::MSWindowsFixedSizeDialogHint, true);
    setSizeGripEnabled(false);
    setLayout(&layout);
    layout.setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    layout.addWidget(&TextLabel);
    layout.addRow(&ButtonsLayout);
    ButtonsLayout.addWidget(&AcceptButton, 0, Qt::AlignCenter);
    QObject::connect(&AcceptButton, &QPushButton::pressed, this, &QDialog::accept);
}

InfoWindowBase::~InfoWindowBase(){}
//InfoWindowBase

//SaveLoadMatrixWindowBase
SaveLoadMatrixWindowBase::SaveLoadMatrixWindowBase(MainWindow& MainWindow, QWidget *parent, Qt::WindowFlags f)
    : DialogWindowBase(MainWindow, parent, f)
{
    PathLineEdit.setFixedWidth(100);
    ChangePathButton.setFixedWidth(100);
    layout.insertRow(0, &PathLabel);
    layout.insertRow(1, &PathLineEdit, &ChangePathButton);

    connect(&PathLineEdit, &QLineEdit::textChanged, this, &SaveLoadMatrixWindowBase::LineEditAdjustWidth);
    connect(&ChangePathButton, &QPushButton::clicked, this, &SaveLoadMatrixWindowBase::ChangePath);
    connect(&AcceptButton, &QPushButton::clicked, this, &SaveLoadMatrixWindowBase::ValidateInput);
}

SaveLoadMatrixWindowBase::~SaveLoadMatrixWindowBase(){}

void SaveLoadMatrixWindowBase::LineEditAdjustWidth()
{
    QLineEdit* LineEdit = qobject_cast<QLineEdit*>(sender());
    size_t MinWidth = 100,
        NewWidth = LineEdit->fontMetrics().boundingRect(LineEdit->text()).width() + 9;
    LineEdit->setFixedWidth(NewWidth > MinWidth ? NewWidth : MinWidth);
    adjustSize();
}
//SaveLoadMatrixWindowBase

//CreateMatrixWindow
CreateMatrixWindow::CreateMatrixWindow(MainWindow& MainWindow, QWidget *parent, Qt::WindowFlags f)
    : DialogWindowBase(MainWindow, parent, f)
{
    setWindowIcon(QIcon(":/icons/icons/create192.png"));
    setWindowTitle("Create Matrix");

    layout.insertRow(0,&RowsLabel, &RowsLineEdit);
    layout.insertRow(1,&ColumnsLabel, &ColumnsLineEdit);
    layout.insertRow(2,&ValueLabel, &ValueLineEdit);
    layout.insertRow(3,&IdentityLabel, &IdentityCheckBox);

    MatrixSelection.addItem(Window.LeftMatrix.Name.text());
    MatrixSelection.addItem(Window.RightMatrix.Name.text());
    MatrixSelection.addItem(Window.LeftMatrix.Name.text() + " and " + Window.RightMatrix.Name.text());

    doubleValidator.setDecimals(20);
    RowsLineEdit.setValidator(&intValidator);
    ColumnsLineEdit.setValidator(&intValidator);
    ValueLineEdit.setValidator(&doubleValidator);

    QObject::connect(&AcceptButton, &QPushButton::clicked, this, &CreateMatrixWindow::ValidateInput);
    QObject::connect(&IdentityCheckBox, &QCheckBox::stateChanged, this, &CreateMatrixWindow::UpdateOnIdentity);

}

CreateMatrixWindow::~CreateMatrixWindow(){}

void CreateMatrixWindow::ValidateInput()
{
    size_t row_amount = RowsLineEdit.text().toULongLong();
    size_t column_amount = ColumnsLineEdit.text().toULongLong();
    if(IdentityCheckBox.isChecked()){
        if (row_amount > 0)
            accept();
        else
            ErrorMessage("Error", "Incorrect size input!");
    }
    else
    {
        if (row_amount > 0 and column_amount > 0)
        {
            accept();
        }
        else
        {
            if (row_amount < 1 and column_amount < 1)
                ErrorMessage("Error", "Incorrect row amount and column amount input!");
            else if (column_amount < 1)
                ErrorMessage("Error", "Incorrect column amount input!");
            else
                ErrorMessage("Error", "Incorrect row amount input!");
        }
    }
}

void CreateMatrixWindow::UpdateOnIdentity()
{
    if (IdentityCheckBox.isChecked())
    {
        RowsLabel.setText("Size:");
        ColumnsLabel.hide();
        ColumnsLineEdit.hide();
        ValueLabel.hide();
        ValueLineEdit.hide();
        adjustSize();
    }
    else
    {
        RowsLabel.setText("Amount of Rows:");
        ColumnsLabel.show();
        ColumnsLineEdit.show();
        ValueLabel.show();
        ValueLineEdit.show();
        adjustSize();
    }
}
//CreateMatrixWindow

//LoadMatrixWindow
LoadMatrixWindow::LoadMatrixWindow(MainWindow& MainWindow, QWidget* parent, Qt::WindowFlags f)
    : SaveLoadMatrixWindowBase(MainWindow, parent, f)
{
    setWindowIcon(QIcon(":/icons/icons/load192.png"));
    setWindowTitle("Load Matrix");
    PathLabel.setText("Select File to load:");
    MatrixSelection.addItem(Window.LeftMatrix.Name.text());
    MatrixSelection.addItem(Window.RightMatrix.Name.text());
}
LoadMatrixWindow::~LoadMatrixWindow(){}

void LoadMatrixWindow::ValidateInput()
{
    QFileInfo file(PathLineEdit.text());
    if (file.exists())
    {
        if (file.isReadable())
            accept();
        else
            ErrorMessage("Error", "File is not Readable!");
    }
    else
        ErrorMessage("Error", "File does not exists!");
}

void LoadMatrixWindow::ChangePath()
{
    QString path = QFileDialog::getOpenFileName(this, "Choose File", "*.matrix");
    PathLineEdit.setText(path);
}
//LoadMatrixWindow

//SaveMatrixWindow
SaveMatrixWindow::SaveMatrixWindow(MainWindow& MainWindow, QWidget* parent, Qt::WindowFlags f)
    : SaveLoadMatrixWindowBase(MainWindow, parent, f)
{
    setWindowIcon(QIcon(":/icons/icons/save192.png"));
    setWindowTitle("Save Matrix");
    layout.insertRow(2, &FileNameLabel, &FileNameLineEdit);

    for (auto Matrix = Window.MatrixList.begin(), end = Window.MatrixList.end(); Matrix != end; ++Matrix)
    {
        if (Window.isMatrixPresent(*Matrix->second))
            MatrixSelection.addItem(Matrix->first);
    }

    PathLabel.setText("Select Folder to save:");
    PathLineEdit.setText(QDir::currentPath());

    FileNameLineEdit.setValidator(&Validator);
    FileNameLineEdit.setMinimumWidth(100);
    connect(&FileNameLineEdit, &QLineEdit::textChanged, this, &SaveLoadMatrixWindowBase::LineEditAdjustWidth);
}
SaveMatrixWindow::~SaveMatrixWindow(){}

void SaveMatrixWindow::ValidateInput()
{
    if (FileNameLineEdit.text() == QString())
    {
        ErrorMessage("Error", "Enter File name!");
        return;
    }
    if(QDir(PathLineEdit.text()).exists())
        accept();
    else
        ErrorMessage("Error", "Folder is invalid!");
}

void SaveMatrixWindow::ChangePath()
{
    QString path = QFileDialog::getExistingDirectory(this, "Choose Directory");
    PathLineEdit.setText(path);
}
//SaveMatrixWindow

//SetPrecisionWindow
SetPrecisionWindow::SetPrecisionWindow(MainWindow& MainWindow, QWidget* parent, Qt::WindowFlags f)
    : DialogWindowBase(MainWindow, parent, f)
{
    setWindowIcon(QIcon(":/icons/icons/precision192.png"));
    setWindowTitle("Set Precision");

    for (auto Matrix = Window.MatrixList.begin(), end = Window.MatrixList.end(); Matrix != end; ++Matrix)
    {
        if (Window.isMatrixPresent(*Matrix->second))
            MatrixSelection.addItem(Matrix->first);
    }
    layout.insertRow(1, &PrecisionLabel, &PrecisionLineEdit);
    ShowPrecision(MatrixSelection.currentText());

    connect(&AcceptButton, &QPushButton::clicked, this, &SetPrecisionWindow::ValidateInput);
    connect(&MatrixSelection, &QComboBox::currentTextChanged, this, &SetPrecisionWindow::ShowPrecision);
}

SetPrecisionWindow::~SetPrecisionWindow(){}

void SetPrecisionWindow::ValidateInput()
{
    if (PrecisionLineEdit.text().toInt() == 0) PrecisionLineEdit.setText("1");
    accept();
}
void SetPrecisionWindow::ShowPrecision(QString text)
{
    QString prec = QString::number(Window.MatrixList[text]->precision);
    PrecisionLineEdit.setText(prec);
}
//SetPrecisionWindow

//DeterminantWindow
DeterminantWindow::DeterminantWindow(MainWindow& MainWindow, QWidget* parent, Qt::WindowFlags f)
    :DialogWindowBase(MainWindow, parent, f)
{
    setWindowIcon(QIcon(":/icons/icons/determinant192.png"));
    setWindowTitle("Determinant");

    for (auto Matrix = Window.MatrixList.begin(), end = Window.MatrixList.end(); Matrix != end; ++Matrix)
    {
        if (Window.isMatrixPresent(*Matrix->second))
            MatrixSelection.addItem(Matrix->first);
    }

    layout.insertRow(0, &message);
    connect(&AcceptButton, &QPushButton::clicked, this, &QDialog::accept);
}

DeterminantWindow::~DeterminantWindow(){}
//DeterminantWindow

//MinorWindow
MinorWindow::MinorWindow(MainWindow& MainWindow, QWidget* parent, Qt::WindowFlags f)
    : DialogWindowBase(MainWindow, parent, f)
{
    setWindowIcon(QIcon(":/icons/icons/minor192.png"));
    setWindowTitle("Minor");

    for (auto Matrix = Window.MatrixList.begin(), end = Window.MatrixList.end(); Matrix != end; ++Matrix)
    {
        if (Window.isMatrixPresent(*Matrix->second))
            MatrixSelection.addItem(Matrix->first);
    }

    layout.insertRow(0, &ColumnLabel, &ColumnLineEdit);
    layout.insertRow(0, &RowLabel, &RowLineEdit);
    layout.insertRow(0, &message);

    RowLineEdit.setValidator(&intValidator);
    ColumnLineEdit.setValidator(&intValidator);


    connect(&AcceptButton, &QPushButton::clicked, this, &MinorWindow::ValidateInput);
}

MinorWindow::~MinorWindow(){}
void MinorWindow::ValidateInput()
{
    size_t input_row = RowLineEdit.text().toULongLong();
    size_t input_column = ColumnLineEdit.text().toULongLong();
    auto MatrixName = MatrixSelection.currentText();
    auto& Matrix = *Window.MatrixList[MatrixName];

    size_t matrix_row = Matrix.data.row();
    size_t matrix_col = Matrix.data.col();
    if (input_row > 0 and input_row <= matrix_row)
    {
        if (input_column > 0 and input_column <= matrix_col)
            accept();
        else
            ErrorMessage("Error", "Incorrect column input!");
    }
    else
        ErrorMessage("Error", "Incorrect row input!");
}
//MinorWindow

//InverseWindow
InverseWindow::InverseWindow(MainWindow& MainWindow, QWidget* parent, Qt::WindowFlags f)
    : DialogWindowBase(MainWindow, parent, f)
{
    setWindowIcon(QIcon(":/icons/icons/inverse192.png"));
    setWindowTitle("Inverse");
    for (auto Matrix = Window.MatrixList.begin(), end = Window.MatrixList.end(); Matrix !=  end; ++Matrix)
    {
        if (Matrix->first == Window.AnswerMatrix.Name.text())
            continue;
        if (Window.isMatrixPresent(*Matrix->second))
            MatrixSelection.addItem(Matrix->first);
    }
    layout.insertRow(0, &message);
    connect(&AcceptButton, &QPushButton::clicked, this, &InverseWindow::ValidateInput);
}

InverseWindow::~InverseWindow(){}

void InverseWindow::ValidateInput()
{
    auto MatrixName = MatrixSelection.currentText();
    auto& Matrix = *Window.MatrixList[MatrixName];
    if (Window.isMatrixSquare(Matrix))
    {
        if (Matrix.data.det() != 0)
            accept();
        else
            ErrorMessage("Error", "Matrix is not invertible!");
    }
    else
        ErrorMessage("Error", "Matrix is not square!");
}
//InverseWindow

//TransposeWindow
TransposeWindow::TransposeWindow(MainWindow& MainWindow, QWidget* parent, Qt::WindowFlags f)
    : DialogWindowBase(MainWindow, parent, f)
{
    setWindowIcon(QIcon(":/icons/icons/transpose192.png"));
    setWindowTitle("Transpose");
    for (auto Matrix = Window.MatrixList.begin(), end = Window.MatrixList.end(); Matrix !=  end; ++Matrix)
    {
        if (Matrix->first == Window.AnswerMatrix.Name.text())
            continue;
        if (Window.isMatrixPresent(*Matrix->second))
            MatrixSelection.addItem(Matrix->first);
    }
    layout.insertRow(0, &message);
    connect(&AcceptButton, &QPushButton::clicked, this, &QDialog::accept);
}

TransposeWindow::~TransposeWindow(){}
//TransposeWindow

//IdentityWindow
IdentityWindow::IdentityWindow(MainWindow& MainWindow, QWidget* parent, Qt::WindowFlags f)
    : DialogWindowBase(MainWindow, parent, f)
{
    setWindowIcon(QIcon(":/icons/icons/identity192.png"));
    setWindowTitle("Identity");
    for (auto Matrix = Window.MatrixList.begin(), end = Window.MatrixList.end(); Matrix !=  end; ++Matrix)
    {
        if (Matrix->first == Window.AnswerMatrix.Name.text())
            continue;
        if (Window.isMatrixPresent(*Matrix->second))
            MatrixSelection.addItem(Matrix->first);
    }
    layout.insertRow(0, &message);
    connect(&AcceptButton, &QPushButton::clicked, this, &IdentityWindow::ValidateInput);
}

IdentityWindow::~IdentityWindow(){}

void IdentityWindow::ValidateInput()
{
    auto MatrixName = MatrixSelection.currentText();
    auto& Matrix = *Window.MatrixList[MatrixName];
    if (Window.isMatrixSquare(Matrix))
        accept();
    else
        ErrorMessage("Error", "Matrix is not square!");
}
//IdentityWindow

//ChangeElementAmountWindow
ChangeElementAmountWindow::ChangeElementAmountWindow(MainWindow& MainWindow, QString Type, QWidget* parent, Qt::WindowFlags f)
    :DialogWindowBase(MainWindow, parent, f), Type(Type)
{
    setWindowIcon(QIcon(QString(":/icons/icons/%1_amount192.png").arg(Type)));
    setWindowTitle(QString("Change %1 amount").arg(Type));
    for (auto Matrix = Window.MatrixList.begin(), end = Window.MatrixList.end(); Matrix !=  end; ++Matrix)
    {
        if (Matrix->first == Window.AnswerMatrix.Name.text())
            continue;
        if (Window.isMatrixPresent(*Matrix->second))
            MatrixSelection.addItem(Matrix->first);
    }
    ModeSelection.addItem("Add");
    ModeSelection.addItem("Remove");

    layout.insertRow(0, &message);
    layout.insertRow(2, &ModeLabel, &ModeSelection);
    layout.insertRow(3, &ElemLabel, &ElemLineEdit);
    layout.insertRow(4, &AmountLabel, &AmountLineEdit);

    ElemLineEdit.setValidator(&intValidator);
    AmountLineEdit.setValidator(&intValidator);
    connect(&AcceptButton, &QPushButton::clicked, this, &ChangeElementAmountWindow::ValidateInput);
}

ChangeElementAmountWindow::~ChangeElementAmountWindow(){}

void ChangeElementAmountWindow::ValidateInput()
{
    size_t input_element = ElemLineEdit.text().toULongLong();
    size_t amount = AmountLineEdit.text().toULongLong();
    auto MatrixName = MatrixSelection.currentText();
    auto& Matrix = *Window.MatrixList[MatrixName];
    auto matrix = Matrix.data;
    size_t matrix_elements;
    if (Type == "column")
        matrix_elements = matrix.col();
    else
        matrix_elements = matrix.row();

    if (ModeSelection.currentText() == "Add")
    {
        if (input_element == 0 and input_element > matrix_elements + 1)
            ErrorMessage("Error", "Incorrect Column input!");
        else if (matrix_elements + amount > 999)
            ErrorMessage("Error", "Incorrect Amount input!");
        else
            accept();
    }
    else
    {
        if (input_element == 0 or input_element > matrix_elements)
            ErrorMessage("Error", "Incorrect Column input!");
        else if (amount == 0 or amount >= matrix_elements or (amount + input_element) > (matrix_elements + 1))
            ErrorMessage("Error", "Incorrect Amount input!");
        else
            accept();
    }
}
//ChangeElementAmountWindow

//SwapMatricesWindow
SwapMatricesWindow::SwapMatricesWindow(MainWindow& MainWindow, QWidget* parent, Qt::WindowFlags f)
    :DialogWindowBase(MainWindow, parent, f)
{
    setWindowIcon(QIcon(":/icons/icons/swap_matrices192.png"));
    setWindowTitle("Swap matrices");
    for (auto Matrix = Window.MatrixList.begin(), end = Window.MatrixList.end(); Matrix !=  end; ++Matrix)
    {
        if (Window.isMatrixPresent(*Matrix->second))
        {
            MatrixSelection.addItem(Matrix->first);
            SecondMatrixSelection.addItem(Matrix->first);
        }
    }
    MatrixLabel.setText("Swap");
    layout.insertRow(1, &SecondMatrixLabel, &SecondMatrixSelection);
    connect(&AcceptButton, &QPushButton::clicked, this, &QDialog::accept);
}

SwapMatricesWindow::~SwapMatricesWindow(){}
//SwapMatricesWindow

//SwapElementsWindow
SwapElementsWindow::SwapElementsWindow(MainWindow& MainWindow, QString Type, QWidget* parent, Qt::WindowFlags f)
    :DialogWindowBase(MainWindow, parent, f), Type(Type)
{
    setWindowIcon(QIcon(QString(":/icons/icons/%1_swap192.png").arg(Type)));
    setWindowTitle(QString("Change %1 amount").arg(Type));
    for (auto Matrix = Window.MatrixList.begin(), end = Window.MatrixList.end(); Matrix !=  end; ++Matrix)
    {
        if (Matrix->first == Window.AnswerMatrix.Name.text())
            continue;
        if (Window.isMatrixPresent(*Matrix->second))
            MatrixSelection.addItem(Matrix->first);
    }

    layout.insertRow(0, &message);
    layout.insertRow(2, &FirstLabel, &FirstLineEdit);
    layout.insertRow(3, &SecondLabel, &SecondLineEdit);

    FirstLineEdit.setValidator(&intValidator);
    SecondLineEdit.setValidator(&intValidator);
    connect(&AcceptButton, &QPushButton::clicked, this, &SwapElementsWindow::ValidateInput);
}

SwapElementsWindow::~SwapElementsWindow(){}

void SwapElementsWindow::ValidateInput()
{
    size_t first_input_element = FirstLineEdit.text().toULongLong();
    size_t second_input_element = SecondLineEdit.text().toULongLong();
    auto MatrixName = MatrixSelection.currentText();
    auto& Matrix = *Window.MatrixList[MatrixName];
    auto matrix = Matrix.data;
    size_t matrix_elements;
    if (Type == "columns")
        matrix_elements = matrix.col();
    else
        matrix_elements = matrix.row();

    if (first_input_element > 0 and first_input_element <= matrix_elements)
    {
        if (second_input_element > 0 and second_input_element <= matrix_elements)
            accept();
        else
            ErrorMessage("Error", "Invalid second input !");
    }
    else
        ErrorMessage("Error", "Invalid first input!");
}
//SwapElementsWindow

//ActionsOnElementWindow
ActionsOnElementWindow::ActionsOnElementWindow(MainWindow& MainWindow, QString Type, QWidget* parent, Qt::WindowFlags f)
    :DialogWindowBase(MainWindow, parent, f), Type(Type)
{
    setWindowIcon(QIcon(QString(":/icons/icons/%1_acts192.png").arg(Type)));
    setWindowTitle(QString("Actions on %1").arg(Type));
    for (auto Matrix = Window.MatrixList.begin(), end = Window.MatrixList.end(); Matrix !=  end; ++Matrix)
    {
        if (Matrix->first == Window.AnswerMatrix.Name.text())
            continue;
        if (Window.isMatrixPresent(*Matrix->second))
            MatrixSelection.addItem(Matrix->first);
    }
    ActionSelection.addItem("+");
    ActionSelection.addItem("-");
    ActionSelection.addItem("÷");
    ActionSelection.addItem("x");
    OperandSelection.addItem("value");
    OperandSelection.addItem(Type);
    layout.insertRow(0, &message);
    layout.insertRow(2, &ElementLabel, &ElementLineEdit);
    layout.insertRow(3, &ActionLabel, &ActionSelection);
    layout.insertRow(4, &OperationLabel, &OperandSelection);
    layout.insertRow(5, &OperandLabel, &OperandLineEdit);

    ElementLineEdit.setValidator(&intValidator);
    OperandLineEdit.setValidator(&doubleValidator);
    connect(&OperandSelection, &QComboBox::currentTextChanged, this, &ActionsOnElementWindow::UpdateOperand);
    connect(&AcceptButton, &QPushButton::clicked, this, &ActionsOnElementWindow::ValidateInput);
}

ActionsOnElementWindow::~ActionsOnElementWindow(){}

void ActionsOnElementWindow::ValidateInput()
{
    size_t input_element = ElementLineEdit.text().toULongLong();
    auto MatrixName = MatrixSelection.currentText();
    auto& Matrix = *Window.MatrixList[MatrixName];
    auto matrix = Matrix.data;
    size_t matrix_elements;
    if (Type == "column")
        matrix_elements = matrix.col();
    else
        matrix_elements = matrix.row();
    if (input_element == 0 or input_element > matrix_elements)
    {
        ErrorMessage("Error", QString("Invalid %1 input!").arg(Type));
        return;
    }
    if (OperandSelection.currentText() != "value")
    {
        size_t input_operand = OperandLineEdit.text().toULongLong();
        if (input_operand == 0 or input_operand > matrix_elements)
        {
            ErrorMessage("Error", QString("Invalid %1 input!").arg(Type));
            return;
        }
    }
    accept();
}
void ActionsOnElementWindow::UpdateOperand(const QString& Operand)
{
    if (Operand == "value")
    {
        if (ActionSelection.findText("÷") == -1)
            ActionSelection.addItem("÷");
        if (ActionSelection.findText("x") == -1)
            ActionSelection.addItem("x");
        OperandLabel.setText("value:");
        OperandLineEdit.setValidator(&doubleValidator);
        double value = OperandLineEdit.text().toDouble();
        OperandLineEdit.setText(QString::number(value));
    }
    else
    {
        ActionSelection.removeItem(ActionSelection.findText("÷"));
        ActionSelection.removeItem(ActionSelection.findText("x"));
        OperandLabel.setText("Index of " + Type + ":");
        OperandLineEdit.setValidator(&intValidator);
        size_t value = OperandLineEdit.text().toULongLong();
        OperandLineEdit.setText(QString::number(value));
    }
}
//ActionOnElementWindow

//HowToUseWindow
HowToUseWindow::HowToUseWindow(QWidget *parent, Qt::WindowFlags f) : InfoWindowBase(parent, f)
{
    setWindowIcon(QIcon(":/icons/icons/howtouse192.png"));
    setWindowTitle("How to Use");
    TextLabel.setText("1. Creating Matrix.\n"
                      "Choose optionally:\n Matrix->Create Matrix.\n"
                      "If you have .matrix file you can load it by Matrix->Load Matrix.\n\n"
                      "2. Operations.\n"
                      "You can browse operations like Determinant or Minor under \"Operations on Matrix\" menu.\n"
                      "Row and column specific operations, like adding number to row, are able under \"Operations on Elements of Matrix.\"\n\n"
                      "3. Additional.\n"
                      "You can change amount of showed significant digits by Matrix->Set Precision.\n"
                      "You can save matrix by Matrix->Save matrix.");
}
HowToUseWindow::~HowToUseWindow(){}
//HowToUseWindow

//AuthorWindow
AuthorWindow::AuthorWindow(QWidget *parent, Qt::WindowFlags f) : InfoWindowBase(parent, f)
{
    setWindowIcon(QIcon(":/icons/icons/author192.png"));
    setWindowTitle("Author");
    TextLabel.setText("GitHub:");
    TextLabel.setAlignment(Qt::AlignCenter);
    LinkLabel.setText(QString("<a href=\"%1\">%1</a>").arg("https://github.com/MeScapegoat"));
    LinkLabel.setOpenExternalLinks(true);
    layout.insertRow(1, &LinkLabel);
}
AuthorWindow::~AuthorWindow(){}
//AuthorWindow
