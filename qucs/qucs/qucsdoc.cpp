/***************************************************************************
                                qucsdoc.cpp
                               -------------
    copyright            : (C) 2003, 2004 by Michael Margraf
                               2019 Felix Salfelder / QUCS team
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QFileInfo>
#include <QAction>

#include "qucsdoc.h"
#include "qucs.h"
#include <QUndoStack>
#include "io.h" // tmp?


QucsDoc::QucsDoc(QucsApp &App_, const QString& Name_)
	: App(&App_),
     _app(App_)
{
  undoStack = new QUndoStack;

  GridOn = true;
  DocName = Name_;
  QFileInfo Info(DocName);
  if(!DocName.isEmpty()) {
    DocName = Info.absoluteFilePath();
    QString base = Info.completeBaseName();
    QString ext = Info.suffix();

    if(ext == "m" || ext == "oct")
      SimTime = "1";

    DataSet = base + ".dat";       // name of the default dataset
    Script = base + ".m";          // name of the default script
    if(ext != "dpl")
      DataDisplay = base + ".dpl"; // name of default data display
    else {
      DataDisplay = base + ".sch"; // name of default schematic
      GridOn = false;              // data display without grid (per default)
    }
  }
  SimOpenDpl = true;
  SimRunScript = false;

  DocChanged = false;
  showBias = -1;  // don't show DC bias (currently for "Schematic" only)
  Scale = 1.0;
}

// vtable here?
QucsDoc::~QucsDoc()
{
	delete undoStack;
}

// really?!
void QucsDoc::undo(){
  assert(undoStack);
  return undoStack->undo();
}

void QucsDoc::redo(){
  assert(undoStack);
  return undoStack->redo();
}

QString QucsDoc::fileSuffix (const QString& Name) {
  QFileInfo Info (Name);
  return Info.suffix();
}

QString QucsDoc::fileSuffix (void) {
  return fileSuffix (DocName);
}

QString QucsDoc::fileBase (const QString& Name) {
  QFileInfo Info (Name);
  return Info.completeBaseName();
}

QString QucsDoc::fileBase (void) {
  return fileBase (DocName);
}

// cleanup debris
QAction* QucsDoc::selectAction()
{
	assert(App);
  	return App->select;
}

void QucsDoc::uncheckActive(){
	if(App->activeAction) {
		App->activeAction->blockSignals(true); // do not call toggle slot
		App->activeAction->setChecked(false);       // set last toolbar button off
		App->activeAction->blockSignals(false);
	}else{
	}
}

MouseActions* QucsDoc::mouseActions()
{
	assert(App);
	return App->view;
}
