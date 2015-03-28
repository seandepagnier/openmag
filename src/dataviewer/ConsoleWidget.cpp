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

#include "ConsoleWidget.h"

ConsoleWidget::ConsoleWidget(QWidget *parent)
   : QScrollArea(parent),
     locked(false)
{
   setWidget(new QLabel());

   label()->setAlignment(Qt::AlignTop);
   setAlignment(Qt::AlignBottom);

   setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

   label()->setTextInteractionFlags(Qt::TextSelectableByMouse);

   QFont font = label()->font();
   font.setPixelSize(12);
   label()->setFont(font);
   label()->setTextFormat(Qt::LogText);
   label()->setWordWrap(true);

   setScrollbackLines(30);

   lastupdate.start();
   connect(&updatelater, SIGNAL(timeout()), this, SLOT(fixtext()));
   updatelater.setSingleShot(true);

   label()->setFocusPolicy(Qt::NoFocus);
}

void ConsoleWidget::update(QString data)
{
   foreach(QChar c, data)
      if(c != '\n')
         currentdata += c;
      else {
         while(lines.count() >= scrollbacklines)
            lines.removeFirst();

         lines.append(currentdata);
         currentdata.clear();

         if(!locked)
            fixtext();
      }
}

void
ConsoleWidget::setScrollbackLines(int lines)
{
   if(lines < 1)
      lines = 1;
   scrollbacklines = lines;
}

void
ConsoleWidget::save()
{
   QSettings settings;
   QString filename = QFileDialog
      ::getSaveFileName(this, "Save File",
                        settings.value("outputSaveLastDir", "").toString());
   if(!filename.size())
      return;

   settings.setValue("outputSaveLastDir", QFileInfo(filename).absolutePath());

   QFile file(filename);
   if(file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
      file.write(lines.join("").toLocal8Bit());
      file.close();
   } else
      update("failed to open " + filename + " for writing\n");
}

void ConsoleWidget::clear()
{
   lines.clear();
   fixtext();
}

bool ConsoleWidget::event(QEvent *e)
{
   /* always handle keypresses (even if tab) */
   if(e->type() == QEvent::KeyPress) {
      keyPressEvent(static_cast<QKeyEvent*>(e));
      return true;
   }
      
   return QAbstractScrollArea::event(e);
}

void ConsoleWidget::keyPressEvent(QKeyEvent *event)
{
   QString t = event->text();

   if(!t.length()) {
      switch(event->key()) {
      case Qt::Key_Up: ScKey('A'); break;
      case Qt::Key_Down: ScKey('B'); break;
      case Qt::Key_Right: ScKey('C'); break;
      case Qt::Key_Left: ScKey('D'); break;
      case Qt::Key_PageUp: /* scroll */ break;
      case Qt::Key_PageDown: /* scroll */ break;
      case Qt::Key_Home: emit RawData(1); break;
      case Qt::Key_End: emit RawData(5); break;
      case Qt::Key_Insert: /* don't bother */ break;
      case Qt::Key_Delete: emit RawData(4); break;
      default: event->ignore(); break;
      }
      return;
   }

   /* if alt is set add an escape character */
   if(event->modifiers() & Qt::AltModifier)
      emit RawData('\e');

   emit RawData(t[0]);
}

void ConsoleWidget::ScKey(char c, int len)
{
   char sckey[5] = {27, '[', c, '~', '~'};
   for(int i=0; i<len; i++)
      emit RawData(sckey[i]);
}

void ConsoleWidget::fixtext()
{
   /* don't update faster than 2hz */
   if(lastupdate.elapsed() < 500) {
      /* keep trying to update */
      updatelater.start(500);
      return;
   }

   lastupdate.start();
   updatelater.stop();

   /* Ensure we keep scrolled to the bottom if we were.
      The whole resize thing is delayed, so this has to be too */
   QScrollBar *vscrollbar = verticalScrollBar();
   if(vscrollbar->value() == vscrollbar->maximum())
      QTimer::singleShot(20, this, SLOT(scrolldown()));

   label()->setText(lines.join("\n") + linebuffer);
   label()->resize(label()->sizeHint());
}

void
ConsoleWidget::scrolldown()
{
   QScrollBar *vscrollbar = verticalScrollBar();
   vscrollbar->setValue(vscrollbar->maximum());
}
