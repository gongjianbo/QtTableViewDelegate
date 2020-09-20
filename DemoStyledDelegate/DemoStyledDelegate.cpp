#include "DemoStyledDelegate.h"
#include "ui_DemoStyledDelegate.h"

#include <QRandomGenerator>
#include <QItemDelegate>

DemoStyledDelegate::DemoStyledDelegate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DemoStyledDelegate)
{
    ui->setupUi(this);

    initModel();
    initDelegate();
    initView();
    initOperate();
}

DemoStyledDelegate::~DemoStyledDelegate()
{
    delete ui;
}

void DemoStyledDelegate::initModel()
{
    //QStandardItemModel类提供用于存储自定义数据的通用模型
    model = new QStandardItemModel(this);

    //模拟一份固定的数据表
    //设置列
    const int col_count=5;
    model->setColumnCount(col_count);
    model->setHeaderData(0,Qt::Horizontal, "Bool");
    model->setHeaderData(1,Qt::Horizontal, "Int");
    model->setHeaderData(2,Qt::Horizontal, "Double");
    model->setHeaderData(3,Qt::Horizontal, "List");
    model->setHeaderData(4,Qt::Horizontal, "String");

    //设置行
    const int row_count=10;
    model->setRowCount(row_count);

    //设置数据
    for(int row=0;row<row_count;row++)
    {
        for(int col=0;col<col_count;col++)
        {
            QStandardItem *new_item=new QStandardItem;

            switch(col)
            {
            default: break;
                //checkbox bool
            case 0:
                new_item->setData(row%2?true:false,Qt::DisplayRole);
                break;
                //spinbox int
            case 1:
                new_item->setData(row,Qt::DisplayRole);
                break;
                //doublespinbox double
            case 2:
                new_item->setData(row*3.1415926,Qt::DisplayRole);
                break;
                //combobox list
            case 3:
                new_item->setData(QStringList{"A","B","C"},Qt::DisplayRole);
                //这里使用userrole来保存列表的下标
                new_item->setData(row%3,Qt::UserRole);
                break;
                //linedit string
            case 4:
                new_item->setData(QString("String %1").arg(row),Qt::DisplayRole);
                break;
            }
            model->setItem(row, col, new_item);
        }
    }

    //view会根据model提供的数据来渲染
    ui->tableView->setModel(model);
}

void DemoStyledDelegate::initDelegate()
{
    delegate=new MyStyledDelegate;
    ui->tableView->setItemDelegate(delegate);
}

void DemoStyledDelegate::initView()
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
    //默认列宽
    table->horizontalHeader()->setDefaultSectionSize(120);
    //最后一列填充宽度(宽度不够时)
    table->horizontalHeader()->setStretchLastSection(true);
    //可以拖动交换列
    table->horizontalHeader()->setSectionsMovable(true);
    //默认行高
    table->verticalHeader()->setDefaultSectionSize(25);
    //行表头文字居中，默认列居中行左对齐
    table->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

    //滚动默认是按照item宽，设置为平滑滚动
    //平滑 QAbstractItemView::ScrollPerPixel
    //元素 QAbstractItemView::ScrollPerItem
    table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    //scrollbar的显示时机
    //table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //这种方式添加的控件拖动单元格大小时，拖动结束他才去适应大小
    //根据文档描述这是用于展示静态内容的接口
    //table->setIndexWidget(model->index(0,0),new QPushButton("btn"));
}

void DemoStyledDelegate::initOperate()
{

}
