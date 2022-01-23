#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QGraphicsTextItem"
#include "QListWidget"

//can control radius of vertices and distance between them
#define ellipseRadius 30
#define radius 120

QBrush whiteBrush(Qt::white);

QPen bluepen(Qt::blue);
QPen redpen(Qt::red);
QPen graypen(Qt::gray);
QPen bluepenDot(Qt::blue);

std::vector <Connections> edges;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->spinBox->setMinimum(0);

    scene = new QGraphicsScene(this);
    scene->clearFocus();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    graypen.setWidth(3);
    bluepen.setWidth(2);
    bluepenDot.setWidth(2);
    bluepen.setCapStyle(Qt::RoundCap);
    bluepenDot.setStyle(Qt::DashLine);

    redpen.setWidth(2);
    redpen.setCapStyle(Qt::RoundCap);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//function for building vertices
void MainWindow::paintVerticies(int numberOfverticies){
    if (numberOfverticies==0)
        return;
    for (int i=1; i<=numberOfverticies; ++i){
        scene->addEllipse(radius*cos(i),radius*sin(i),ellipseRadius,ellipseRadius,graypen,whiteBrush);
        QGraphicsTextItem *text = scene->addText(QString::number(i));
        text->setPos(radius*cos(i)+7,radius*sin(i)+3);
    }
};

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    ui->tableSum->setRowCount(arg1);
    ui->tableSum->setColumnCount(arg1);

    for (int i=0;i<ui->tableSum->rowCount();++i){
        for (int j=0;j<ui->tableSum->columnCount();++j) {
            ui->tableSum->setColumnWidth(i,10);
            QTableWidgetItem *Zero = new QTableWidgetItem();
            Zero->setText(QString::fromStdString("0"));
            Zero->setTextAlignment(Qt::AlignHCenter);
            ui->tableSum->setItem(i,j,Zero);
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->tableSum->clear();
    ui->spinBox->setValue(0);
}


void MainWindow::on_pushButton_2_clicked()
{
    edges.clear();
    scene->clear();

    //remembering every connection between vertices
    for(int i=0; i<ui->tableSum->rowCount(); ++i){
        for(int j=0; j<ui->tableSum->rowCount(); ++j){
            if (i>j){
                continue;
            }
            if(ui->tableSum->item(i,j)->text()!="0"){
                Connections pair;
                pair.first=i+1;
                pair.second=j+1;
                edges.push_back(pair);
            }
        }
    }

    for (Connections item : edges) {
        if(item.first==item.second){
            scene->addEllipse(radius*cos(item.first)-5,radius*sin(item.second)-5,ellipseRadius+5,ellipseRadius+5,bluepenDot);
        }else{
            scene->addLine(radius*cos(item.first)+7,radius*sin(item.first)+3,
                           radius*cos(item.second)+7,radius*sin(item.second)+3,bluepen);
        }
    }
    paintVerticies(ui->tableSum->columnCount());
}


void MainWindow::on_tableSum_cellClicked(int row, int column)
{
    QTableWidgetItem *mirrored = new QTableWidgetItem();
    mirrored->setText(ui->tableSum->item(row,column)->text());
    mirrored->setTextAlignment(Qt::AlignHCenter);
    ui->tableSum->setItem(column,row,mirrored);
}

//copies symbol "∞" to clipboard
void MainWindow::on_pushButton_3_clicked()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText("∞");
}

bool include(std::vector <int> myVector, int element){
    for (int i=0; i<(int)myVector.size(); ++i){
        if(myVector[i]==element){
            return true;
        }
    }
    return false;
}

void eraseElement(std::vector <int> &myVector, int element){
    for (int i=0; i<(int)myVector.size(); ++i){
        if(myVector[i]==element){
            myVector.erase(myVector.begin()+i);
        }
    }
}

int sum = 0;

void MainWindow::buildTreePath(int &i, std::vector <int> &vertices){
    int min=INT_MAX, position=0;
    for(int j=0; j<ui->tableSum->rowCount(); ++j){
        if(ui->tableSum->item(i,j)->text()=="∞" || i==j || ui->tableSum->item(i,j)->text().toInt()==0){
            continue;
        }
        else if (ui->tableSum->item(i,j)->text().toInt()<min){
            if(!include(vertices,j)){
                continue;
            }
            position = j;
            min = ui->tableSum->item(i,j)->text().toInt();
        }
    }
    sum+=min;
    eraseElement(vertices,i);
    scene->addLine(radius*cos(i+1)+7,radius*sin(i+1)+3,
                   radius*cos(position+1)+7,radius*sin(position+1)+3,redpen);
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(QString::number(i+1)+"-"+QString::number(position+1));
    ui->listWidget->addItem(item);
    i=position;
}

//spanning tree
void MainWindow::on_pushButton_4_clicked()
{
    ui->listWidget->clear();
    std::vector <int> vertices;
    sum=0;
    for (int i=0; i<ui->tableSum->rowCount(); ++i){
        vertices.push_back(i);
    }
    int i=0;
    do{
        buildTreePath(i,vertices);
    }while(vertices.size()!=1);

    paintVerticies(ui->tableSum->columnCount());
    QListWidgetItem *item = new QListWidgetItem;
    item->setText("Spanning tree \nweight: "+QString::number(sum));
    ui->listWidget->addItem(item);

}
