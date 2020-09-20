#include "MyStyledDelegate.h"

#include <QStyleOptionViewItem>
#include <QStyle>
#include <QApplication>

MyStyledDelegate::MyStyledDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void MyStyledDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //注意，此时index是logicIndex（model坐标），我们可以通过拖拽表头项交换列来测试
    Q_ASSERT(index.isValid());
    //QStyle会根据option的属性进行绘制，我们也可以不使用QStyle的规则，完全自定义
    QStyleOptionViewItem opt = option;
    //去掉焦点 setFocusPolicy(Qt::NoFocus);
    opt.state &= ~QStyle::State_HasFocus;
    //参照源码实现了自己的initStyleOption
    initMyStyleOption(&opt, index);

    const QWidget *widget = opt.widget;
    QStyle *style = widget ? widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
}

QSize MyStyledDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //暂略
    return QStyledItemDelegate::sizeHint(option,index);
}

QWidget *MyStyledDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    //参照了实例spinboxdelegate
    if (!index.isValid())
        return nullptr;

    switch(index.column())
    {
    default: break;
    case 0://bool
    {
        QComboBox *editor=new QComboBox(parent);
        editor->setFrame(false);
        editor->addItems({"True","False"});
        //editor->setCurrentIndex(index.data(Qt::DisplayRole).toBool()?0:1);
        return editor;
    }
    case 1://int
    {
        QSpinBox *editor=new QSpinBox(parent);
        editor->setFrame(false);
        editor->setMinimum(-10000);
        editor->setMaximum(10000);
        //editor->setValue(index.data(Qt::DisplayRole).toInt());
        return editor;
    }
    case 2://double
    {
        QDoubleSpinBox *editor=new QDoubleSpinBox(parent);
        editor->setFrame(false);
        editor->setMinimum(-10000);
        editor->setMaximum(10000);
        editor->setDecimals(3);
        //editor->setValue(index.data(Qt::DisplayRole).toDouble());
        return editor;
    }
    case 3://list
    {
        QComboBox *editor=new QComboBox(parent);
        editor->setFrame(false);
        const QStringList str_list=index.data(Qt::DisplayRole).toStringList();
        editor->addItems(str_list);
        //这里使用userrole来保存列表的下标
        //const int str_index=index.data(Qt::UserRole).toInt();
        //if(str_index>=0&&str_index<str_list.count())
        //    editor->setCurrentIndex(str_index);
        return editor;
    }
    case 4://string
    {
        QLineEdit *editor=new QLineEdit(parent);
        editor->setFrame(false);
        //editor->setText(index.data(Qt::DisplayRole).toString());
        return editor;
    }
    }
    return nullptr;
}

void MyStyledDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    switch(index.column())
    {
    default: break;
    case 0://bool
    {
        QComboBox *box = static_cast<QComboBox*>(editor);
        box->setCurrentIndex(index.data(Qt::DisplayRole).toBool()?0:1);
    }break;
    case 1://int
    {
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->setValue(index.data(Qt::DisplayRole).toInt());
    }break;
    case 2://double
    {
        QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
        spinBox->setValue(index.data(Qt::DisplayRole).toDouble());
    }break;
    case 3://list
    {
        QComboBox *box = static_cast<QComboBox*>(editor);
        //这里使用userrole来保存列表的下标
        const int str_index=index.data(Qt::UserRole).toInt();
        if(str_index>=0&&str_index<box->count())
            box->setCurrentIndex(str_index);
    }break;
    case 4://string
    {
        QLineEdit *edit = static_cast<QLineEdit*>(editor);
        edit->setText(index.data(Qt::DisplayRole).toString());
    }break;
    }
}

void MyStyledDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    switch(index.column())
    {
    default: break;
    case 0://bool
    {
        QComboBox *box = static_cast<QComboBox*>(editor);
        model->setData(index,box->currentIndex()==0?true:false,Qt::EditRole);
    }break;
    case 1://int
    {
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        model->setData(index,spinBox->value(),Qt::DisplayRole);
    }break;
    case 2://double
    {
        QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
        model->setData(index,spinBox->value(),Qt::DisplayRole);
    }break;
    case 3://list
    {
        QComboBox *box = static_cast<QComboBox*>(editor);
        //这里使用userrole来保存列表的下标
        model->setData(index,box->currentIndex(),Qt::UserRole);
    }break;
    case 4://string
    {
        QLineEdit *edit = static_cast<QLineEdit*>(editor);
        model->setData(index,edit->text(),Qt::DisplayRole);
    }break;
    }
}

void MyStyledDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}

void MyStyledDelegate::initMyStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
    //字体
    QVariant value = index.data(Qt::FontRole);
    if (value.isValid() && !value.isNull()) {
        option->font = qvariant_cast<QFont>(value).resolve(option->font);
        option->fontMetrics = QFontMetrics(option->font);
    }
    //对齐方式
    //value = index.data(Qt::TextAlignmentRole);
    //if (value.isValid() && !value.isNull())
    //    option->displayAlignment = Qt::Alignment(value.toInt());
    //也可以直接全部指定为居中对齐
    option->displayAlignment = Qt::AlignCenter;
    //前景色
    value = index.data(Qt::ForegroundRole);
    if (value.canConvert<QBrush>())
        option->palette.setBrush(QPalette::Text, qvariant_cast<QBrush>(value));
    option->index = index;
    //value = index.data(Qt::CheckStateRole); 未使用，暂略
    //value = index.data(Qt::DecorationRole); 未使用，暂略
    //文本
    //value = index.data(Qt::DisplayRole);
    //if (value.isValid() && !value.isNull()) {
    option->features |= QStyleOptionViewItem::HasDisplay;
    option->text = getDisplayText(index); //displayText(value, option->locale);
    //}
    //背景色
    option->backgroundBrush = qvariant_cast<QBrush>(index.data(Qt::BackgroundRole));
    // disable style animations for checkboxes etc. within itemviews (QTBUG-30146)
    option->styleObject = nullptr;
}

QString MyStyledDelegate::getDisplayText(const QModelIndex &index) const
{
    //注意，此时index是logicIndex（model坐标），我们可以通过拖拽表头项交换列来测试
    const QVariant value = index.data(Qt::DisplayRole);
    //我们可以根据variant的type或者index的行列来特殊处理
    switch(index.column())
    {
    default: break;
    case 0://bool
        return value.toBool()?"True":"False";
    case 1://int
        return QString::number(value.toInt());
    case 2://double
        return QString::number(value.toDouble(),'f',3);
    case 3://list
    {
        const QStringList str_list=value.toStringList();
        //这里使用userrole来保存列表的下标
        const int str_index=index.data(Qt::UserRole).toInt();
        if(str_index>=0&&str_index<str_list.count())
            //给字符串加个括号
            return QString("[ %1 ]").arg(str_list.at(str_index));
    }
        break;
    case 4://string
        //给字符串加个括号
        return QString("[ %1 ]").arg(value.toString());
    }
    return QString();
}

