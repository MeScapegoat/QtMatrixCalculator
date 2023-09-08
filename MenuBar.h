#ifndef MenuBar_H
#define MenuBar_H
#include <QMenuBar>
class MenuBar : public QMenuBar
{
    Q_OBJECT
    friend class MainWindow;

public:
    MenuBar(QWidget *parent = nullptr);
    ~MenuBar();

private slots:

private:
    QMenu *MatrixMenu,
        *OnMatrixMenu,
        *OnElementsMenu,
        *AboutMenu;
};

#endif // MenuBar_H
