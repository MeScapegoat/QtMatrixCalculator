#include "MenuBar.h"
#include <QIcon>
#include <QStyle>
#include <iostream>
MenuBar::MenuBar(QWidget *parent)
    : QMenuBar(parent),
    MatrixMenu(addMenu("Matrix")),
    OnMatrixMenu(addMenu("Operations on Matrix")),
    OnElementsMenu(addMenu("Operations on Elements of Matrix")),
    AboutMenu(addMenu("About"))
{
    MatrixMenu->addAction(QIcon(":/icons/icons/create192.png"),"Create Matrix");
    MatrixMenu->addAction(QIcon(":/icons/icons/load192.png"),"Load Matrix");
    MatrixMenu->addAction(QIcon(":/icons/icons/save192.png"),"Save Matrix");
    MatrixMenu->addAction(QIcon(":/icons/icons/precision192.png"),"Set precision");

    OnMatrixMenu->addAction(QIcon(":/icons/icons/determinant192.png"), "Calculate Determinant");
    OnMatrixMenu->addAction(QIcon(":/icons/icons/minor192.png"), "Calculate Minor");
    OnMatrixMenu->addAction(QIcon(":/icons/icons/inverse192.png"), "Calculate Inverse");
    OnMatrixMenu->addAction(QIcon(":/icons/icons/transpose192.png"), "Transpose");
    OnMatrixMenu->addAction(QIcon(":/icons/icons/identity192.png"), "Make Identity");
    OnMatrixMenu->addAction(QIcon(":/icons/icons/column_amount192.png"), "Change Columns Amount");
    OnMatrixMenu->addAction(QIcon(":/icons/icons/row_amount192.png"), "Change Rows Amount");
    OnMatrixMenu->addAction(QIcon(":/icons/icons/swap_matrices192.png"), "Swap Matrices");

    OnElementsMenu->addAction(QIcon(":/icons/icons/columns_swap192.png"), "Swap Columns");
    OnElementsMenu->addAction(QIcon(":/icons/icons/rows_swap192.png"), "Swap Rows");
    OnElementsMenu->addAction(QIcon(":/icons/icons/column_acts192.png"), "Actions on Column");
    OnElementsMenu->addAction(QIcon(":/icons/icons/row_acts192.png"), "Actions on Row");

    AboutMenu->addAction(QIcon(":/icons/icons/howtouse192.png"), "How to Use");
    AboutMenu->addAction(QIcon(":/icons/icons/author192.png"), "Author");
}

MenuBar::~MenuBar()
{
    delete MatrixMenu;
    delete OnMatrixMenu;
    delete OnElementsMenu;
    delete AboutMenu;
}
