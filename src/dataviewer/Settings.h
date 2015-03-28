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

#include "ui_Settings.h"

class DataTree;

class Settings : public QDialog
{
    Q_OBJECT
 public:
    Settings(QWidget *parent, DataTree *tree);

    void Load();

    Ui_Settings ui;

 signals:
    void DeviceName(QString);

 private slots:
    void ShowChanged();
    void Save();
    void DeviceChanged();

 private:
    void SetupTreeBox(QCheckBox *box, QString name, bool checked);

    DataTree *tree;
};
