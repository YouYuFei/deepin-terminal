#include "customcommandsearchrstpanel.h"
#include "customcommandoptdlg.h"
#include "shortcutmanager.h"
#include "iconbutton.h"
#include "listview.h"
#include "utils.h"
#include "mainwindow.h"

#include <DApplicationHelper>
#include <DGuiApplicationHelper>
#include <DMessageBox>

#include <QAction>
#include <QKeyEvent>
#include <QApplication>
#include <QCoreApplication>
#include <QTimer>
#include <QDebug>

CustomCommandSearchRstPanel::CustomCommandSearchRstPanel(QWidget *parent)
    : CommonPanel(parent)
{
    initUI();
}

/*******************************************************************************
 1. @函数:    initUI
 2. @作者:    sunchengxi
 3. @日期:    2020-07-31
 4. @说明:    初始化自定义搜索面板界面
*******************************************************************************/
void CustomCommandSearchRstPanel::initUI()
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    m_rebackButton = new IconButton(this);
    m_rebackButton->setObjectName("CustomRebackButton");
    m_backButton = m_rebackButton;
    m_backButton->setIcon(DStyle::StandardPixmap::SP_ArrowLeave);
    m_backButton->setFixedSize(QSize(36, 36));
    m_backButton->setFocusPolicy(Qt::TabFocus);

    m_label = new DLabel(this);
    m_label->setAlignment(Qt::AlignCenter);
    // 字体颜色随主题变化变化
    DPalette palette = m_label->palette();
    QColor color;
    if (DApplicationHelper::instance()->themeType() == DApplicationHelper::DarkType) {
        color = QColor::fromRgb(192, 198, 212, 102);
    } else {
        color = QColor::fromRgb(85, 85, 85, 102);
    }
    palette.setBrush(QPalette::Text, color);
    m_label->setPalette(palette);

    m_cmdListWidget = new ListView(ListType_Custom, this);

    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->addSpacing(10);
    hlayout->addWidget(m_backButton);
    // 搜索框居中显示
    hlayout->addWidget(m_label, 0, Qt::AlignCenter);
    hlayout->setSpacing(0);
    hlayout->setMargin(0);

    QVBoxLayout *vlayout = new QVBoxLayout();
    vlayout->addSpacing(10);
    vlayout->addLayout(hlayout);
    vlayout->addWidget(m_cmdListWidget);
    vlayout->setMargin(0);
    vlayout->setSpacing(10);
    setLayout(vlayout);

    connect(m_cmdListWidget, &ListView::itemClicked, this, &CustomCommandSearchRstPanel::doCustomCommand);
    connect(m_backButton, &DIconButton::clicked, this, &CustomCommandSearchRstPanel::showCustomCommandPanel);
//    connect(m_rebackButton, &IconButton::preFocus, this, [ = ]() {
//        // 在派生类捕获方向键盘左键按下，转化为鼠标点击。
//        QKeyEvent pressSpace(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
//        QApplication::sendEvent(m_rebackButton, &pressSpace);
//        // 设置定时
//        QTimer::singleShot(80, this, [ = ]() {
//            // 模拟空格键松开事件
//            QKeyEvent releaseSpace(QEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier);
//            QApplication::sendEvent(m_rebackButton, &releaseSpace);
//        });
//    });
    // 字体颜色随主题变化变化
    connect(DApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, m_label, [ = ](DGuiApplicationHelper::ColorType themeType) {
        DPalette palette = m_label->palette();
        QColor color;
        if (themeType == DApplicationHelper::DarkType) {
            color = QColor::fromRgb(192, 198, 212, 102);
        } else {
            color = QColor::fromRgb(85, 85, 85, 102);
        }
        palette.setBrush(QPalette::Text, color);
        m_label->setPalette(palette);
    });

    connect(m_rebackButton, &IconButton::focusOut, this, [ = ](Qt::FocusReason type) {
        // 焦点切出，没值的时候
        if (type == Qt::TabFocusReason && m_cmdListWidget->count() == 0) {
            // tab 进入 +
            QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Tab, Qt::MetaModifier);
            QApplication::sendEvent(Utils::getMainWindow(this), &keyPress);
            qDebug() << "search panel focus to '+'";
        }
    });

    connect(m_cmdListWidget, &ListView::focusOut, this, [ = ](Qt::FocusReason type) {
        Q_UNUSED(type);
        if (Qt::TabFocusReason == type) {
            // tab 进入 +
            QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Tab, Qt::MetaModifier);
            QApplication::sendEvent(Utils::getMainWindow(this), &keyPress);
            qDebug() << "search panel focus on '+'";
            m_cmdListWidget->clearIndex();
        } else if (Qt::BacktabFocusReason == type || type == Qt::NoFocusReason) {
            // shift + tab 返回 返回键               // 列表为空，也返回到返回键上
            m_rebackButton->setFocus();
            m_cmdListWidget->clearIndex();
            qDebug() << "search panel type" << type;
        }

    });
}

/*******************************************************************************
 1. @函数:    setSearchFilter
 2. @作者:    sunchengxi
 3. @日期:    2020-07-31
 4. @说明:    设置搜索过滤条件及显示文本
*******************************************************************************/
void CustomCommandSearchRstPanel::setSearchFilter(const QString &filter)
{
    m_strFilter = filter;
    QString showText = Utils::getElidedText(m_label->font(), filter, ITEMMAXWIDTH, Qt::ElideMiddle);
    m_label->setText(QString("%1：%2").arg(tr("Search"), showText));
}

/*******************************************************************************
 1. @函数:    refreshData
 2. @作者:    sunchengxi
 3. @日期:    2020-07-31
 4. @说明:    根据 m_strFilter 刷新搜索面板自定义列表
*******************************************************************************/
void CustomCommandSearchRstPanel::refreshData()
{
    ShortcutManager::instance()->fillCommandListData(m_cmdListWidget, m_strFilter);
}

/*******************************************************************************
 1. @函数:    refreshData
 2. @作者:    sunchengxi
 3. @日期:    2020-07-31
 4. @说明:    根据 strFilter 刷新搜索面板自定义列表
*******************************************************************************/
void CustomCommandSearchRstPanel::refreshData(const QString &strFilter)
{
    setSearchFilter(strFilter);
    ShortcutManager::instance()->fillCommandListData(m_cmdListWidget, strFilter);
}

/*******************************************************************************
 1. @函数:    doCustomCommand
 2. @作者:    sunchengxi
 3. @日期:    2020-07-31
 4. @说明:    执行当前搜索面板自定义命令列表中itemData 中的自定义命令
*******************************************************************************/
void CustomCommandSearchRstPanel::doCustomCommand(const QString &strKey)
{
    QAction *item = ShortcutManager::instance()->findActionByKey(strKey);
    QString strCommand = item->data().toString();
    if (!strCommand.endsWith('\n')) {
        strCommand.append('\n');
    }
    emit handleCustomCurCommand(strCommand);
    emit focusOut();
}

