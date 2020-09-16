#include "DemoTableModel.h"
#include "ui_DemoTableModel.h"

#include <QRandomGenerator>

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
    QTableView *table=ui->tableView;
    model=new MyTableModel(ui->tableView);
    table->setModel(model);

    //保存和恢复model选中项，因为在resetModel后会失效
    connect(model,&MyTableModel::modelAboutToBeReset,this,[=]{
        selectedIndex=table->currentIndex();
    },Qt::DirectConnection);
    connect(model,&MyTableModel::modelReset,this,[=]{
        //不用担心无效的index，接口内部会处理
        table->setCurrentIndex(selectedIndex);
    },Qt::DirectConnection);

    //设置第三列列宽
    table->setColumnWidth(2,200);
    //开启隔行变色，设置之后qss的对应设置才有效
    table->setAlternatingRowColors(true);
    //单行选中=按行选中+单次选择
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    //默认行高
    table->verticalHeader()->setDefaultSectionSize(25);
    //行表头文字居中，默认列居中行左对齐
    table->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
}

void DemoTableModel::initOperate()
{
    //设置model的数据
    connect(ui->btnSetData,&QPushButton::clicked,this,&DemoTableModel::setData);
    //导出model中的数据
    connect(ui->btnGetData,&QPushButton::clicked,this,&DemoTableModel::getData);
    //添加行
    connect(ui->btnAddRow,&QPushButton::clicked,this,&DemoTableModel::addRow);
    //删除行
    connect(ui->btnDelRow,&QPushButton::clicked,this,&DemoTableModel::delRow);

    //初始化数据
    setData();
}

void DemoTableModel::setData()
{
    //一般我们从数据库、文件、网络获取到了数据后，再通过model的接口去刷新
    //这里我们用随机数来模拟实际数据
    QRandomGenerator *random=QRandomGenerator::global();
    //随机的数据行数
    const int row_count=random->bounded(10,20);
    QList<MyModelItem> new_data;
    for(int row=0;row<row_count;row++)
    {
        new_data.push_back({
                               QString("name %1").arg(row),
                               random->bounded(20,40),
                               QString("info %1 %2").arg(row).arg(random->bounded(0,100))
                           });
    }
    //调用我们的自定义接口设置model的数据
    model->setModelData(new_data);
}

void DemoTableModel::getData()
{
    //调用自定义接口获取model数据
    //QList<MyModelItem> old_data=model->getModelData();
}

void DemoTableModel::addRow()
{
    //在选中行和下一行之间插入
    //model->insertRows(ui->tableView->currentIndex().row()+1,1);
    //model->insertRow(ui->tableView->currentIndex().row()+1);
    QRandomGenerator *random=QRandomGenerator::global();
    model->insertModelData(ui->tableView->currentIndex().row()+1,{
                               QString("name new"),
                               random->bounded(20,40),
                               QString("info new %1").arg(random->bounded(0,100))
                           });
}

void DemoTableModel::delRow()
{
    //删除选中行
    //model->removeRows(ui->tableView->currentIndex().row(),1);
    model->removeRow(ui->tableView->currentIndex().row());
}

