/* Copyright (C) 2007, 2008 Sean D'Epagnier <sean@depagnier.com>
 *
 * This Program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * For more information on the GPL, please go to:
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <QtGui>
#include "DataViewer.h"

DataViewer::DataViewer()
{
    ui.setupUi(this);

    /* process */
    process = new DataProcess(this);
    connect(ui.respawnButton, SIGNAL(clicked()),
            process, SLOT(respawn()));

    /* settings */
    settings = new Settings(this, ui.tree);
    connect(settings, SIGNAL(DeviceName(QString)),
            process, SLOT(setDeviceName(QString)));

    /* menu */
    connect(ui.actionSettings, SIGNAL(triggered()),
            settings, SLOT(show()));
    connect(ui.actionExit, SIGNAL(triggered()),
            this, SLOT(close()));
    connect(ui.action_About, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui.actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    /* tree widget */
    connect(process, SIGNAL(QueryResult(QString, QString)),
            ui.tree, SLOT(QueryResult(QString, QString)));
    connect(ui.tree, SIGNAL(Command(QString)),
            process, SLOT(Command(QString)));
    connect(ui.treePopulateButton, SIGNAL(clicked()),
            ui.tree, SLOT(populate()));
    connect(ui.treeGetValuesButton, SIGNAL(clicked()),
            ui.tree, SLOT(getvalues()));
    connect(ui.treeGetSelectedValuesButton, SIGNAL(clicked()),
            ui.tree, SLOT(getselectedvalues()));
    connect(ui.treeStopButton, SIGNAL(clicked()),
            ui.tree, SLOT(stop()));
    connect(ui.tree, SIGNAL(status(const QString&)),
            ui.treeStatusLabel, SLOT(setText(const QString&)));

    /* console widget */
    connect(settings->ui.ConsoleScrollbackBox,
            SIGNAL(valueChanged(int)),
            ui.console, SLOT(setScrollbackLines(int)));
    connect(process, SIGNAL(update(QString)),
            ui.console, SLOT(update(QString)));
    connect(ui.console, SIGNAL(RawData(QChar)),
            process, SLOT(RawData(QChar)));
    connect(ui.consoleClearButton, SIGNAL(clicked()),
            ui.console, SLOT(clear()));

    /* output widget */
    connect(settings->ui.OutputScrollbackBox,
            SIGNAL(valueChanged(int)),
            ui.output, SLOT(setScrollbackLines(int)));
    connect(process, SIGNAL(asyncUpdate(QString)),
            ui.output, SLOT(update(QString)));
    connect(ui.lockOutputBox, SIGNAL(stateChanged(int)),
            ui.output, SLOT(setLocked(int)));
    connect(ui.outputSaveButton, SIGNAL(clicked()),
            ui.output, SLOT(save()));
    connect(ui.outputClearButton, SIGNAL(clicked()),
            ui.output, SLOT(clear()));

    settings->Load();
    process->respawn();
}

void DataViewer::about()
{
    QMessageBox::about(this, tr("About DataViewer"),
            tr("<p>The <b>DataViewer</b> program provides a graphical version "
               "of the dataclient console application used to communicate to "
               "devices implementing the data interface"
               "<p>Written by Sean D'Epagnier (C) 2008"));
}
