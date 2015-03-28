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

#ifndef __HAVE_CONSOLE_WIDGET
#define __HAVE_CONSOLE_WIDGET

#include <QtGui>

class ConsoleWidget : public QScrollArea
{
    Q_OBJECT;
 public:
    ConsoleWidget(QWidget *parent=0);

 public slots:
    void update(QString update);
    void setScrollbackLines(int lines);
    void save();
    void clear();
    void setLocked(int lock) { locked = lock; }

 protected:
    virtual bool event(QEvent *e);
    virtual void keyPressEvent(QKeyEvent *event);

 signals:
    void RawData(QChar data);
    
 private:
    void ScKey(char c, int len=3);

    QLabel *label() { return static_cast<QLabel*>(widget()); }

    QString linebuffer, currentdata;
    QStringList lines;

    bool locked;
    int scrollbacklines;

    QTime lastupdate;
    QTimer updatelater;

 private slots:
    void fixtext();
    void scrolldown();
};

#endif
