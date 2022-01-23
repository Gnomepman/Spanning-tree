#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QTableWidgetItem>
#include <set>

#include <QMainWindow>

struct Connections{
    int first,second;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintVerticies(int numberOfverticies);

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_tableSum_cellClicked(int row, int column);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void buildTreePath(int &i, std::vector <int> &vertices);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsEllipseItem *ellipse;
    QGraphicsRectItem *rectangle;
};
#endif // MAINWINDOW_H
