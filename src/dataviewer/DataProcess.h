/* Copyright (C) 2007 Sean D'Epagnier <sean@depagnier.com>
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

class DataProcess : public QProcess
{
    Q_OBJECT;
 public:
    DataProcess(QObject *parent=0);
    ~DataProcess();

 public slots:
    void Command(QString cmd);
    void respawn();

 signals:
    void update(QString QueryData);
    void asyncUpdate(QString AsyncData);

    void QueryResult(QString query, QString result);

 private slots:
    void readStdout();
    void readStderr();
    void error(QProcess::ProcessError);
    void finished(int, QProcess::ExitStatus);
    void started();

    void setDeviceName(QString name);

 private:
    QString device;

    QString query;
    QString result;

    QString devicename;
};
