/***************************************************************************
 *   Copyright (C) 2005-08 by the Quassel Project                          *
 *   devel@quassel-irc.org                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) version 3.                                           *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QDebug>
#include <QtGui>
#include "chatwidget.h"

ChatWidget::ChatWidget(QWidget *parent) : QTextEdit(parent) {
  setStyleSheet("background-color: rgba(255, 255, 255, 60%)");
  setTextInteractionFlags(Qt::TextBrowserInteraction);
}

void ChatWidget::setContents(QList<ChatLineOld *> lines) {
  clear();
  appendChatLines(lines);

}

void ChatWidget::prependMsg(AbstractUiMsg *msg) {
  ChatLineOld *line = static_cast<ChatLineOld*>(msg);
  Q_ASSERT(line);
  prependChatLine(line);
}

void ChatWidget::appendMsg(AbstractUiMsg *msg) {
  ChatLineOld *line = static_cast<ChatLineOld*>(msg);
  Q_ASSERT(line);
  appendChatLine(line);
}

void ChatWidget::appendChatLine(ChatLineOld *line) {
  QTextCursor cursor = textCursor();
  moveCursor(QTextCursor::End);
  if(!document()->isEmpty()) insertPlainText("\n");
  insertStyledText(line->styledSender());
  insertPlainText(" ");
  insertStyledText(line->styledText());
  setTextCursor(cursor);
}

void ChatWidget::appendChatLines(QList<ChatLineOld *> list) {
  foreach(ChatLineOld *line, list) {
    appendChatLine(line);
  }
}

void ChatWidget::prependChatLine(ChatLineOld *line) {
  QTextCursor cursor = textCursor();
  moveCursor(QTextCursor::Start);
  bool flg = document()->isEmpty();
  insertStyledText(line->styledSender());
  insertPlainText(" ");
  insertStyledText(line->styledText());
  if(!flg) insertPlainText("\n");
  setTextCursor(cursor);
}

void ChatWidget::prependChatLines(QList<ChatLineOld *> list) {
  foreach(ChatLineOld *line, list) {
    prependChatLine(line);
  }
}

void ChatWidget::insertChatLine(ChatLineOld *line) {
  if(!document()->isEmpty()) insertPlainText("\n");
  insertStyledText(line->styledSender());
  insertPlainText(" ");
  insertStyledText(line->styledText());
}

void ChatWidget::insertStyledText(const QtopiaUiStyle::StyledText &stext) {
  QTextCursor cursor = textCursor();
  foreach(QTextLayout::FormatRange format, stext.formats) {
    cursor.setCharFormat(format.format);
    setTextCursor(cursor);
    insertPlainText(stext.text.mid(format.start, format.length));
  }
}
