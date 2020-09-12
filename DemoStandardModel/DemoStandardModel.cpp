#include "DemoStandardModel.h"
#include "ui_DemoStandardModel.h"

#include <QRandomGenerator>

//测试Item可以正常释放
//#include <QDebug>
//class MyItem : public QStandardItem
//{
//public:
//    MyItem(const QString &text):QStandardItem(text){}
//    ~MyItem(){
//        qDebug()<<"remove item"<<text();
//    }
//};

DemoStandardModel::DemoStandardModel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DemoStandardModel)
{
    ui->setupUi(this);

    initModel();
    initView();
    initOperate();

    //初始化一次数据
    updateData();
}

DemoStandardModel::~DemoStandardModel()
{
    delete ui;
}

void DemoStandardModel::initModel()
{
    //QStandardItemModel类提供用于存储自定义数据的通用模型
    //如果用这个类的话感觉可以考虑把QTableView换成QTableWidget
    model = new QStandardItemModel(this);

    //设置列表头
    model->setColumnCount(ColCount);
    for(int col=0;col<ColCount;col++){
        model->setHeaderData(col,Qt::Horizontal, QString("Col %1").arg(col));
    }

    //设置行表头
    //一般行数刷新数据的时候会动态修改，就不放这里了
    //model->setRowCount(1);
    //model->setHeaderData(0,Qt::Vertical, "Row 1");

    //view会根据model提供的数据来渲染
    ui->tableView->setModel(model);
    //即使item设置了edit flag，view的edit triggers还是可以屏蔽编辑
    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void DemoStandardModel::initView()
{
    //QTableView常用设置
    QTableView *table=ui->tableView;

    //开启隔行变色，设置之后qss的对应设置才有效
    table->setAlternatingRowColors(true);
    //单行选中=按行选中+单次选择
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    //取消编辑
    //table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //选中item时自动滚动以使其可见
    table->setAutoScroll(false);

    //取消表头高亮，取消之后选中checked样式不生效，不取消选中表头时字体加粗等没法直接去掉
    table->horizontalHeader()->setHighlightSections(false);
    //table->verticalHeader()->setHighlightSections(false);
    //默认行高
    table->verticalHeader()->setDefaultSectionSize(25);
    //行表头文字居中，默认列居中行左对齐
    table->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    //设置列宽，设置后很多情况会导致重置model data后恢复默认列宽
    //table->setColumnWidth(0,150);
    //固定列宽，默认为可交互的
    //table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    //隐藏列，貌似要先设置model
    //table->horizontalHeader()->setSectionHidden(1,true);
    //最后一列填充宽度(宽度不够时)
    //table->horizontalHeader()->setStretchLastSection(true);

    //滚动默认是按照item宽，设置为平滑滚动
    //平滑 QAbstractItemView::ScrollPerPixel
    //元素 QAbstractItemView::ScrollPerItem
    table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    //scrollbar的显示时机
    //table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void DemoStandardModel::initOperate()
{
    timer=new QTimer(this);
    timer->setInterval(3000); //刷新间隔ms
    connect(timer,&QTimer::timeout,this,&DemoStandardModel::updateData);

    //点击定时刷新
    connect(ui->checkUpdate,&QCheckBox::stateChanged,this,[=]{
        if(ui->checkUpdate->isChecked()){
            timer->start();
            this->updateData();
        }else{
            timer->stop();
        }
    });
}

void DemoStandardModel::updateData()
{
    //每个单元格设置一个随机数[0-300)
    //0-100红色，100-200绿色，200-300蓝
    QRandomGenerator *random=QRandomGenerator::global();

    const int row_count=random->bounded(10,30);
    int value=0;
    //填充表格
    model->setRowCount(row_count);
    for(int row=0;row<row_count;row++){
        for(int col=0;col<ColCount;col++){
            value=random->bounded(0,300);
            //QStandardItem提供了一些便捷的接口，也可以使用setData，setFlags这种通用接口
            //如果行列数是固定的，就不用每次new了，model->item(row,col)就可以获取那个单元格对象
            QStandardItem *new_item=new QStandardItem(QString::number(value));
            //文本居中
            new_item->setData(Qt::AlignCenter,Qt::TextAlignmentRole);
            switch (col) {
            case 0:
                //第一列：勾选框
                new_item->setText("");
                //要设置为勾选框
                //这个勾选框好像没办法设置居中，对于简单的情况可以用QTableView::indicator的margin撑过来
                //而且只有点击勾选框才行点单元格不能勾选的，可以关联单元格点击事件
                new_item->setCheckable(true);
                //new_item->setData(Qt::Unchecked,Qt::CheckStateRole);
                //new_item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
                break;
            case 1:
                //第二列：可编辑
                //未选中前景色，对于选中的颜色在delegate里可以改
                //new_item->setForeground();
                new_item->setData(QColor(Qt::green),Qt::ForegroundRole);
                //未选中背景色
                //new_item->setBackground();
                new_item->setData(QColor(0,75,125),Qt::BackgroundRole);
                //QStandardItem应该默认就可以编辑
                //new_item->setEditable(true);
                new_item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
                break;
            default:
                //其他列：展示数据
                //前景色,0-100红色，100-200绿色，200-300蓝
                new_item->setData(QColor(value<100?Qt::red:(value>=200?Qt::blue:Qt::green)),Qt::ForegroundRole);
                new_item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                break;
            }
            model->setItem(row, col, new_item);
        }
    }
}
