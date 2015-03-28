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

#include "DataTree.h"
#include "DataComboBox.h"
#include "DataClearButton.h"

DataTree::DataTree(QWidget *parent)
   : QTreeWidget(parent),
     editingItem(NULL),
     totalwork(0)
{
   QStringList headers;
   headers << "name" << "mem" << "type" << "value" << "clear" << "permissions";
   setColumnCount(headers.count());
   setHeaderLabels(headers);

   worktimeouttimer = new QTimer;
   connect(worktimeouttimer, SIGNAL(timeout()), SLOT(WorkTimeout()));
   worktimeouttimer->setSingleShot(true);

   connect(this, SIGNAL(itemActivated(QTreeWidgetItem *, int)),
           SLOT(ActivateItem(QTreeWidgetItem *, int)));
   connect(this, SIGNAL(itemChanged(QTreeWidgetItem *, int)),
           SLOT(ChangeItem(QTreeWidgetItem *, int)));

   sortItems(0, Qt::AscendingOrder);
}

void
DataTree::QueryResult(QString query, QString reply)
{
   QStringList q = query.split(" ", QString::SkipEmptyParts);
   if(q.count() < 2)
      return;

   if(reply.length() && reply[reply.length()-1] == '\n')
      reply.truncate(reply.length()-1);

   QString op = q[0];
   QString dir = q[1];
   QStringList dirs = dir.split("/", QString::SkipEmptyParts);

   QTreeWidgetItem *item = find(invisibleRootItem(), dirs);

   /* can't find the item for the reply */
   if(!item)
      return;

   /* always update get requests */
   if(op == "get") {
      DataComboBox *combo = dynamic_cast<DataComboBox*>
          (itemWidget(item, VALUE_COL));
      if(combo)
         combo->setValue(reply);
      else {
         QStringList replies = reply.split("\n");

         /* clear out old data */
         item->setText(VALUE_COL, "");
         foreach(QTreeWidgetItem *x, item->takeChildren())
             delete x;

         if(replies.count() == 1)
            item->setText(VALUE_COL, reply);
         else {
            int c = 0;
            foreach(QString reply, replies) {
               QTreeWidgetItem *x = new QTreeWidgetItem;
               x->setText(NAME_COL, "value [" + QString::number(c++) + "]");
               x->setText(VALUE_COL, reply);
               item->addChild(x);
            }
         }
      }
   }

   /* if we didn't make the request, ignore it here */
   if(work.empty() || work.head() != query)
      return;

   work.dequeue();

   if(reply.startsWith("<") && reply.endsWith(">")) {
      emit status("Query failed!");
      return;
   }

   if(op == "ls") {
      /* for a listing, add the appropriate number of children */
      QStringList names = reply.split(" ", QString::SkipEmptyParts);
      foreach(QString name, names) {
         QTreeWidgetItem *child = new QTreeWidgetItem(item);

         child->setFlags(Qt::ItemIsUserCheckable
                         | Qt::ItemIsEnabled);

         if(dir == "/")
            dir.clear();

         if(name[name.length()-1] == '/')
            name.truncate(name.length()-1);

         child->setText(NAME_COL, name);
         /* queue up request for operations on this item */
         AddWork("ops " + dir + "/" + name);
      }
   } else
   if(op == "ops") {
      /* got list of operations, now queue up ops */
      QStringList ops = reply.split(" ", QString::SkipEmptyParts);

      QStringList usableops;
      usableops << "ls";
      if(!isColumnHidden(MEM_COL))
         usableops << "mem";
      if(!isColumnHidden(TYPE_COL))
         usableops << "type";

      usableops << "values";
      usableops << "get";

      foreach(QString usableop, usableops)
         if(ops.contains(usableop))
            AddWork(usableop + " " + dir);

      if(ops.contains("clear")) {
          removeItemWidget(item, CLEAR_COL);
          DataClearButton *clear = new DataClearButton(this, item);
          setItemWidget(item, CLEAR_COL, clear);
      }

      if(ops.contains("get")) {
         item->setCheckState(0, Qt::Unchecked);
         if(ops.contains("set"))
            item->setText(PERMISSIONS_COL, "read/write");
         else
            item->setText(PERMISSIONS_COL, "read only");
      } else
         if(ops.contains("set"))
            item->setText(PERMISSIONS_COL, "write only");
         else
            item->setText(PERMISSIONS_COL, "none");
   } else
   if(op == "mem") {
      item->setText(MEM_COL, reply);
   } else
   if(op == "type") {
      item->setText(TYPE_COL, reply);
   } else
   if(op == "values") {
      removeItemWidget(item, VALUE_COL);
      DataComboBox *combo = new DataComboBox(this, item, reply);
      setItemWidget(item, VALUE_COL, combo);
   }

   StartWork();
}

void
DataTree::populate()
{
   int empty = work.isEmpty();

   stop();
   clear();
   AddWork("ops /");

   if(empty)
      StartWork();
   else
      QTimer::singleShot(500, this, SLOT(StartWork()));
}

void
DataTree::getvalues()
{
   makeGetValuesWork(invisibleRootItem(), "", false);
   StartWork();
}

void
DataTree::getselectedvalues()
{
   makeGetValuesWork(invisibleRootItem(), "", true);
   StartWork();
}

void
DataTree::stop()
{
   work.clear();
   totalwork = 0;
   emit status("Stopped");
   worktimeouttimer->stop();
}

void
DataTree::StartWork()
{
   int done = totalwork - work.count();
   QString stats = QString::number(done) + "/" + QString::number(totalwork);
   if(work.empty()) {
      emit status("Complete: " + stats);
      totalwork = 0;
      worktimeouttimer->stop();
   } else {
      /* start up next in queue */
      emit status("Queries: " + stats);
      emit Command(work.head());
      worktimeouttimer->start(5000);
   }
}

void
DataTree::WorkTimeout()
{
#if 1
   emit status("Queries timed out, retry");
   StartWork();
#else
   emit status("Queries timed out");
   work.clear();
   totalwork = 0;
#endif
}

void
DataTree::ActivateItem(QTreeWidgetItem *i, int col)
{
   if(col != VALUE_COL)
      return;

   if(!i->text(PERMISSIONS_COL).contains("write"))
      return;

   i->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEditable
               | Qt::ItemIsEnabled);
   editingItem = i;
   editItem(i, col);
}

void
DataTree::ChangeItem(QTreeWidgetItem *i, int col)
{
   if(i != editingItem)
      return;

   editingItem = NULL;

   if(col != VALUE_COL)
      return;

   i->setFlags(i->flags() & ~Qt::ItemIsEnabled);
   AddWork("set " + GetPath(i) + " " + i->text(VALUE_COL));
   if(i->text(PERMISSIONS_COL).contains("read"))
      AddWork("get " + GetPath(i));
   StartWork();

   i->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
}

void DataTree::AddWork(QString job)
{
   totalwork++;
   work.enqueue(job);
}

void
DataTree::makeGetValuesWork(QTreeWidgetItem *item, QString dir,
                            bool selected)
{
   for(int i=0; i<item->childCount(); i++) {
      QTreeWidgetItem *child = item->child(i);
      QString name = dir + "/" + child->text(NAME_COL);
      if((!selected || child->checkState(0) == Qt::Checked)
         && child->text(PERMISSIONS_COL).startsWith("read"))
            AddWork("get " + name);
      makeGetValuesWork(child, name, selected);
   }
}

QTreeWidgetItem *DataTree::find(QTreeWidgetItem *item, QStringList dirs)
{
   if(dirs.empty())
      return item; /* must be it */

   QString name = dirs.takeFirst();

   for(int i=0; i<item->childCount(); i++) {
      QTreeWidgetItem *x = item->child(i);
      if(name == x->text(NAME_COL))
         return find(x, dirs);
   }

   return NULL; /* not here */
}

QString
DataTree::GetPath(QTreeWidgetItem *i)
{
   return (i->parent() ? GetPath(i->parent()) : "") + "/" + i->text(NAME_COL);
}
