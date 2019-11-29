/*
 Copyright (C) %{CURRENT_YEAR} by %{AUTHOR} <%{EMAIL}>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License as
 published by the Free Software Foundation; either version 2 of
 the License or (at your option) version 3 or any later version
 accepted by the membership of KDE e.V. (or its successor approved
 by the membership of KDE e.V.), which shall act as a proxy 
 defined in Section 14 of version 3 of the license.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "dtsviewwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QProcess>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "libdtc/dtc.h"

DTSViewWindow::DTSViewWindow()
    : QMainWindow()
{
    QWidget *widget = new QWidget(this);
    setCentralWidget(widget);
    m_ui.setupUi(widget);

    m_ui.treeWidget->setColumnCount(2);
    m_ui.treeWidget->setHeaderLabels(QStringList{"prop", "value"});

    QObject::connect(m_ui.btn_browse, &QPushButton::clicked,
                     this, &DTSViewWindow::onBtnBrowseClicked);

#ifdef QT_DEBUG
    m_ui.le_dtsFile->setText(QStringLiteral("/home/minnehanov/dev/kernels/"
                                            "android_kernel_xiaomi_lavender/"
                                            "arch/arm/boot/dts/qcom/sdm660-mtp_f7a.dts"));
#endif
}

DTSViewWindow::~DTSViewWindow()
{
}

QString DTSViewWindow::findKernelDir()
{
    // example input: "/home/minnehanov/dev/kernels/android_kernel_xiaomi_lavender/arch/arm/boot/dts/qcom/sdm660-mtp_f7a.dts"
    // output should be: "/home/minnehanov/dev/kernels/android_kernel_xiaomi_lavender"
    // detector strings:
    static const QVector<QLatin1String> detectors = {
        QLatin1String("arch/arm/boot/dts"),
        QLatin1String("arch/arm64/boot/dts"),
        // maybe more
    };
    for (const auto &detector: detectors) {
        const int idx = m_dtsFileName.indexOf(detector, Qt::CaseSensitive);
        if (idx > 0) {
            const QString kdir = m_dtsFileName.left(idx - 1);
            return kdir;
        }
    }
    qWarning() << Q_FUNC_INFO << "could not detect kernel directory!";
    return QString();
}

bool DTSViewWindow::runGccPreprocess()
{
    const QFileInfo fi(m_dtsFileName);
    const QString dtsBaseName = fi.baseName(); // only file name without .dts/.dtsi extesion

    m_preprocessedDtsFileName = QStringLiteral("/tmp/%1.dts_pp").arg(dtsBaseName);
    const QString depsFileName = QStringLiteral("/tmp/%1.dts_deps").arg(dtsBaseName);

    /*
gcc -E -Wp,-MD,arch/arm64/boot/dts/marvell/.armada-3720-espressobin.dtb.d.pre.tmp \
    -nostdinc -I../scripts/dtc/include-prefixes -undef -D__DTS__ \
    -x assembler-with-cpp -o arch/arm64/boot/dts/marvell/.armada-3720-espressobin.dtb.dts.tmp \
    ../arch/arm64/boot/dts/marvell/armada-3720-espressobin.dts
    */
    const QStringList arguments = {
        QStringLiteral("-E"),         //  preprocess only
        QStringLiteral("-Wp,-MD,%1").arg(depsFileName), // generate deps file (?)
        QStringLiteral("-nostdinc"),  // Do not search standard system include directories
        QStringLiteral("-I%1/scripts/dtc/include-prefixes").arg(m_kernelDir),  // path to standard DTC includes
        QStringLiteral("-undef"),     // Do not predefine system-specific and GCC-specific macros.
        QStringLiteral("-D__DTS__"),
        QStringLiteral("-x"),         // Specify the language of the following input files (you cannot guess by file extension)
        QStringLiteral("assembler-with-cpp"),
        QStringLiteral("-o"),         // Specify output file
        m_preprocessedDtsFileName,  // output file path
        m_dtsFileName   // input file path
    };

    qDebug() << Q_FUNC_INFO << "gcc arguments:";
    qDebug() << arguments;

    int ret = QProcess::execute(QStringLiteral("gcc"), arguments);
    qDebug() << "gcc exec result:" << ret;
    return (ret == 0);
}

static void fillNodes(struct node *dt, QTreeWidget *treeWidget, QTreeWidgetItem *parent)
{
    if (!dt) return;

    QTreeWidgetItem *it = new QTreeWidgetItem();
    it->setText(0, QString::fromUtf8(dt->name));

    // properties
    struct property *prop = dt->proplist;
    while (prop) {
        QTreeWidgetItem *propItem = new QTreeWidgetItem();
        if (prop->deleted) {
            propItem->setText(0, QStringLiteral("(deleted) ") + QString::fromUtf8(prop->name));
        } else {
            propItem->setText(0, QString::fromUtf8(prop->name));
        }
        if (prop->val.val)
            propItem->setText(1, QString::fromUtf8(prop->val.val));
        else
            propItem->setText(1, QString::asprintf("len: %d", prop->val.len));
        prop = prop->next;
    }

    // recurse children
    struct node *child = dt->children;
    while (child) {
        fillNodes(child, treeWidget, it);
        child = child->next_sibling;
    }

    parent->addChild(it);
}

void DTSViewWindow::onBtnBrowseClicked()
{
    QString startDir;
    if (!m_ui.le_dtsFile->text().isEmpty()) {
        QFileInfo fi(m_ui.le_dtsFile->text());
        startDir = fi.absolutePath();
    }
    m_dtsFileName = QFileDialog::getOpenFileName(
                this, tr("Open DTS"), startDir, tr("DTS Files (*.dts *.dtsi)"));
    if (m_dtsFileName.isEmpty()) return;
    qDebug() << "DTS full path: " << m_dtsFileName;

    const QString kDir = findKernelDir();
    if (kDir.isEmpty()) {
        qWarning() << "DTS file should be opened directly form kernel source directory!";
        return;
    }
    qDebug() << "Detected kernel dir: " << kDir;
    m_kernelDir = kDir;
    m_ui.le_dtsFile->setText(m_dtsFileName);

    if (!runGccPreprocess()) {
        qWarning() << "Preprocessing failed!";
        return;
    }

    struct dt_info *devicetree = dt_from_source(m_preprocessedDtsFileName.toUtf8().constData());
    if (!devicetree) {
        qWarning() << "parser failed?";
        return;
    }

    QTreeWidgetItem *rootItem = new QTreeWidgetItem();
    rootItem->setText(0, QStringLiteral("devicetree"));
    rootItem->setText(1, QString());
    m_ui.treeWidget->clear();
    m_ui.treeWidget->addTopLevelItem(rootItem);

    QTreeWidgetItem *tiBootCpu = new QTreeWidgetItem();
    tiBootCpu->setText(0, QStringLiteral("boot_cpuid_phys"));
    tiBootCpu->setText(1, QString::number(devicetree->boot_cpuid_phys, 10));
    rootItem->addChild(tiBootCpu);

    struct reserve_info *ri = devicetree->reservelist;
    while (ri) {
        QTreeWidgetItem *tiReserveInfo = new QTreeWidgetItem();
        tiReserveInfo->setText(0, QStringLiteral("reserve_info"));
        tiReserveInfo->setText(1, QString::asprintf("<%08lX, %08lX>", ri->address, ri->size));
        // TODO: also add labels..?
        rootItem->addChild(tiReserveInfo);
        ri = ri->next;
    }

    // add all child nodes
    fillNodes(devicetree->dt, m_ui.treeWidget, rootItem);

    qDebug() << devicetree->dt->name;
}
