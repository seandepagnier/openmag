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

#include "../data/data.h"
#include "DataProcess.h"

DataProcess::DataProcess(QObject *parent)
   : QProcess(parent)
{
   QSettings settings;
   device = settings.value("device", "").toString();

   connect(this, SIGNAL(readyReadStandardOutput()),
           SLOT(readStdout()));

   connect(this, SIGNAL(readyReadStandardError()),
           SLOT(readStderr()));
   connect(this, SIGNAL(error(QProcess::ProcessError)),
           SLOT(error(QProcess::ProcessError)));
   connect(this, SIGNAL(finished(int, QProcess::ExitStatus)),
           SLOT(finished(int, QProcess::ExitStatus)));
   connect(this, SIGNAL(started()),
           SLOT(started()));

   /* when we exit, kill off the process */
   connect(this, SIGNAL(destroyed()),
           SLOT(kill()));
}

DataProcess::~DataProcess()
{
    terminate();
    waitForFinished(100);
    kill();
}

void DataProcess::Command(QString cmd)
{
   if(!query.isEmpty())
      return;

   query = cmd;
   
   /* now write the command */
   write(cmd.toAscii());
   write("\n", 1);
}

void DataProcess::respawn()
{
   if(state() != QProcess::NotRunning) {
      terminate();
      if(!waitForFinished(300))
         kill();
   }

   QStringList args;

   args << "-r";
   if(!devicename.isEmpty())
       args << devicename;

   emit update("./dataclient " + args.join(" ") + "\n");
   start("./dataclient", args);
}

void DataProcess::readStdout()
{
   QByteArray data = readAllStandardOutput();

   const char prompt[] = DATA_PROMPT;
   int ppos = 0;
   foreach(QChar c, data) {
      if(prompt[ppos] == c) {
         if(!prompt[++ppos]) {
            QString q = query, r = result;
            emit update(" " + q + "\n" + r);

            /* get rid of prompt */
            r.chop((sizeof prompt) / (sizeof *prompt) - 1);

            query.clear();
            result.clear();
            emit QueryResult(q, r);
            continue;
         }
      } else
         ppos = 0;

      result += c;
   }
}

/* stderr has async data */
void DataProcess::readStderr()
{
   emit asyncUpdate(readAllStandardError());
}

void DataProcess::error(QProcess::ProcessError error)
{
   switch(error) {
   case QProcess::FailedToStart:
      emit update("dataclient failed to start\n");
      break;
   default:
      break;
   }
}

void DataProcess::finished(int exitcode, QProcess::ExitStatus exitstatus)
{
   const char prompt[] = DATA_PROMPT;
   QString queryresult = prompt + query;
   if(exitstatus == QProcess::CrashExit)
      queryresult += "\nProcess terminated\n";
   else
   if(exitcode == 0)
      queryresult += "\nProcess finished\n";
   else {
      queryresult += "\nProcess exited abnormally with code ";
      queryresult += QString::number(exitcode) + "\n";
   }
   emit update(queryresult);
}

void DataProcess::started()
{
   emit update("Process spawned");
}

void DataProcess::setDeviceName(QString name)
{
   devicename = name;
   if(state() != QProcess::NotRunning)
      respawn();
}
