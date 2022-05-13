#include "DemoTableModel.h"
#include "ui_DemoTableModel.h"

#include <QRandomGenerator>
#include <QSortFilterProxyModel>

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
    //想要利用view支持的点击表头排序功能
    //我们可以用QSortFilterProxyModel完成排序，或者实现model的sort接口来完成排序
    //其次，view要设置setSortingEnabled为true，且表头是可点击的
#if 0
    //借助QSortFilterProxyModel
    QSortFilterProxyModel *proxy=new QSortFilterProxyModel(ui->tableView);
    proxy->setSourceModel(model);
    table->setModel(proxy);
#else
    //借助model的sort接口，基类实现不执行任何操作，要自己实现
    table->setModel(model);
#endif

    //保存和恢复model选中项，因为在resetModel后会失效
    connect(model,&MyTableModel::modelAboutToBeReset,this,[=]{
        selectedIndex=table->currentIndex();
    },Qt::DirectConnection);
    connect(model,&MyTableModel::modelReset,this,[=]{
        //不用担心无效的index，接口内部会处理
        table->setCurrentIndex(selectedIndex);
    },Qt::DirectConnection);

    //开启隔行变色，设置之后qss的对应设置才有效
    table->setAlternatingRowColors(true);
    //单行选中=按行选中+单次选择
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    //默认行高
    table->verticalHeader()->setDefaultSectionSize(25);
    //行表头文字居中，默认列居中行左对齐，也可以在headerData接口里设置
    //table->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

    //修改表头，自定义接口
    model->setHorHeaderData(QList<QString>{"Name","Age","Info"});

    //表头实例
    QHeaderView *header=table->horizontalHeader();
    //拖拽交换行
    header->setSectionsMovable(true);
    //如何决策宽高
    //header->setSectionResizeMode(QHeaderView::Fixed);
    //是否可以点击
    header->setSectionsClickable(true);
    //选中时高亮
    //header->setHighlightSections(false);
    //默认宽高，放到table设置宽高的接口前，不然被覆盖
    header->setDefaultSectionSize(100);
    //最后一列填充
    header->setStretchLastSection(true);

    //排序
    //table->setSortingEnabled(true);
    //设置第三列列宽
    //table->setColumnWidth(2,200);
}

void DemoTableModel::initOperate()
{
    //设置model的数据
    connect(ui->btnSetData,&QPushButton::clicked,this,&DemoTableModel::setData);
    //导出model中的数据
    ui->btnGetData->setEnabled(false);
    connect(ui->btnGetData,&QPushButton::clicked,this,&DemoTableModel::getData);
    //添加行
    connect(ui->btnAddRow,&QPushButton::clicked,this,&DemoTableModel::addRow);
    //删除行
    connect(ui->btnDelRow,&QPushButton::clicked,this,&DemoTableModel::delRow);
    //排序开关
    connect(ui->sortBox,&QCheckBox::stateChanged,this,&DemoTableModel::sortEnableChange);

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

void DemoTableModel::sortEnableChange()
{
    if(ui->sortBox->isChecked()){
        //开启排序
        ui->tableView->setSortingEnabled(true);
        //第2列升序
        ui->tableView->sortByColumn(1,Qt::AscendingOrder);
    }else{
        //关闭排序功能
        ui->tableView->setSortingEnabled(false);
        //这里还有个问题是，如果需要恢复默认顺序如何做
        //对于sortproxy这个比较简单，因为他改变的是显示的位置，
        //但是model的sort直接改的容器中数据的位置
        //我们可以给每行的数据标记一个原始的index，恢复时根据index值排序
    }
}

