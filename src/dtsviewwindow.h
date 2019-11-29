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

#ifndef DTSVIEWWINDOW_H
#define DTSVIEWWINDOW_H


#include "ui_dtsview.h"

#include <QMainWindow>

/**
 * This class serves as the main window for dtsview.  It handles the
 * menus, toolbars and status bars.
 *
 */
class DTSViewWindow : public QMainWindow
{
    Q_OBJECT
public:
    DTSViewWindow();
    ~DTSViewWindow() override;

public slots:
    void onBtnBrowseClicked();

protected:
    QString findKernelDir();
    bool runGccPreprocess();

    QString m_dtsFileName;
    QString m_kernelDir;
    QString m_preprocessedDtsFileName;

private:
    Ui::mainWidget m_ui;
};

#endif // DTSVIEWWINDOW_H
