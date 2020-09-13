#include "DemoTableModel.h"
#include "ui_DemoTableModel.h"

DemoTableModel::DemoTableModel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DemoTableModel)
{
    ui->setupUi(this);

    initTable();
    initOperate();
}

DemoTableModel::~DemoTableModel()
{
    delete ui;
}

void DemoTableModel::initTable()
{
    model=new MyTableModel(ui->tableView);
    ui->tableView->setModel(model);
}

void DemoTableModel::initOperate()
{

}

void DemoTableModel::updateData()
{

}
