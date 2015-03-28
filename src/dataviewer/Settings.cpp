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

#include "Settings.h"
#include "DataTree.h"

Settings::Settings(QWidget *parent, DataTree *t)
   : QDialog(parent),
     tree(t)
{
   ui.setupUi(this);

   connect(ui.DeviceEdit, SIGNAL(editingFinished()),
           SLOT(DeviceChanged()));

   SetupTreeBox(ui.ShowTypeBox, "ShowType", false);
   SetupTreeBox(ui.ShowMemBox, "ShowMem", false);
   SetupTreeBox(ui.ShowClearBox, "ShowClear", true);
   SetupTreeBox(ui.ShowPermissionsBox, "ShowPermissions", true);
   ShowChanged();

   connect(ui.ShowTypeBox, SIGNAL(stateChanged(int)),
           SLOT(ShowChanged()));
   connect(ui.ShowMemBox, SIGNAL(stateChanged(int)),
           SLOT(ShowChanged()));
   connect(ui.ShowClearBox, SIGNAL(stateChanged(int)),
           SLOT(ShowChanged()));
   connect(ui.ShowPermissionsBox, SIGNAL(stateChanged(int)),
           SLOT(ShowChanged()));

   connect(ui.SaveButton, SIGNAL(clicked()),
           SLOT(Save()));
}

void
Settings::Load()
{
   QSettings s;

   ui.ConsoleScrollbackBox->setValue(s.value("ConsoleScrollbackLines", 20)
                                     .toInt());
   ui.OutputScrollbackBox->setValue(s.value("OutputScrollbackLines", 50)
                                    .toInt());

   ui.DeviceEdit->setText(s.value("Device", "").toString());
   DeviceChanged();
}

void
Settings::ShowChanged()
{
   tree->setColumnHidden(DataTree::TYPE_COL,
                         ui.ShowTypeBox->checkState() == Qt::Unchecked);
   tree->setColumnHidden(DataTree::MEM_COL,
                         ui.ShowMemBox->checkState() == Qt::Unchecked);
   tree->setColumnHidden(DataTree::CLEAR_COL,
                         ui.ShowClearBox->checkState() == Qt::Unchecked);
   tree->setColumnHidden(DataTree::PERMISSIONS_COL,
                         ui.ShowPermissionsBox->checkState() == Qt::Unchecked);
}

void
Settings::Save()
{
   QSettings s;
   s.setValue("ShowType", ui.ShowTypeBox->checkState() == Qt::Checked);
   s.setValue("ShowMem", ui.ShowMemBox->checkState() == Qt::Checked);
   s.setValue("ShowClear", ui.ShowClearBox->checkState()
              == Qt::Checked);
   s.setValue("ShowPermissions", ui.ShowPermissionsBox->checkState()
              == Qt::Checked);

   s.setValue("ConsoleScrollbackLines", ui.ConsoleScrollbackBox->value());
   s.setValue("OutputScrollbackLines", ui.OutputScrollbackBox->value());

   s.setValue("Device", ui.DeviceEdit->text());
}

void
Settings::DeviceChanged()
{
   emit DeviceName(ui.DeviceEdit->text());
}

void
Settings::SetupTreeBox(QCheckBox *box, QString name, bool checked)
{
   QSettings s;
   box->setCheckState(s.value(name, checked).toBool()
                      ? Qt::Checked : Qt::Unchecked);
}
