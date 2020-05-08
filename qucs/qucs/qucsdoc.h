/***************************************************************************
                                 qucsdoc.h
                                -----------
    begin                : Wed Sep 3 2003
    copyright            : (C) 2003 by Michael Margraf
    email                : michael.margraf@alumni.tu-berlin.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QUCSDOC_H
#define QUCSDOC_H

#include "object.h"
#include <QString>
#include <QDateTime>
#include <QTextStream> // tmp: used as baseclass, should be member.
#include <assert.h>

#include "object.h"
#include "actions.h"
#include "trace.h"

class QucsApp;
class QAction;
class QPrinter;
class QPainter;
class MouseActions;
class QUndoStack;

class QucsDoc {
protected:
	QucsDoc(const QucsDoc&);
public:
  explicit QucsDoc(QucsApp&, const QString&);
  virtual ~QucsDoc();

public:
  virtual void  setName(const QString&) {};
  virtual bool  load() { return true; };
  virtual int   save() { return 0; };
  virtual void  print(QPrinter*, QPainter*, bool, bool) {};
  virtual void  becomeCurrent(bool) {};
  virtual float zoomBy(float) { return 1.0; };
  virtual void  showAll() {};
  virtual void  showNoZoom() {};

  static QString fileSuffix (const QString&);
  QString fileSuffix (void);
  static QString fileBase (const QString&);
  QString fileBase (void);

private:
  QString DocName;

public:
  void setDocName(QString x){ DocName=x; }
  QString docName() const{ return DocName; }

  QString DataSet;     // name of the default dataset
  QString DataDisplay; // name of the default data display
  QString Script;
  QString SimTime;     // used for VHDL simulation, but stored in datadisplay
  QDateTime lastSaved;

  float Scale;
  QucsApp &_app;
  QucsApp* App; // TODO: delete.
  bool DocChanged;
  bool SimOpenDpl;   // open data display after simulation ?
  bool SimRunScript; // run script after simulation ?
  int  showBias;     // -1=no, 0=calculation running, >0=show DC bias points
  bool GridOn;
  int  tmpPosX, tmpPosY;

protected: // why not directly connect to undostack slots?!
  virtual void undo();
  virtual void redo();

public: // actions: These somehow correspond to buttons.
        // needs cleanup...
	virtual void actionSelect(bool on) = 0;
	virtual void actionCopy() = 0;
	virtual void actionCut() = 0;
	virtual void actionEditActivate(bool) = 0;
	virtual void actionEditUndo() = 0;
	virtual void actionEditRedo() = 0;
	virtual void actionSelectAll() = 0;
	virtual void actionChangeProps() = 0;

	// these are not implemented for qucstext, not called perhaps?
	virtual void actionApplyCompText() { unreachable(); }
	virtual void actionAlign(int) {unreachable();}
	virtual void actionDistrib(int) {unreachable();}
	virtual void actionSelectMarker() {unreachable();}
	virtual void actionCursor(arrow_dir_t) { unreachable();}
	virtual void actionExportGraphAsCsv(){ unreachable();}

	virtual void actionOnGrid(bool) {unreachable();}
	virtual void actionEditRotate(bool) {unreachable();}
	virtual void actionEditMirrorX(bool) {unreachable();}
	virtual void actionEditMirrorY(bool) {unreachable();}
	virtual void actionEditDelete(bool) {unreachable();}
	virtual void actionEditPaste(bool) {unreachable();}
	virtual void actionSetWire(bool) {unreachable();}
	virtual void actionInsertLabel(bool) {unreachable();}
	virtual void actionInsertEquation(bool) {unreachable();}
	virtual void actionInsertEntity() {unreachable();}
	virtual void actionInsertPort(bool) {unreachable();}
	virtual void actionInsertGround(bool) {unreachable();}
	virtual void actionSetMarker(bool) {unreachable();}
	virtual void actionMoveText(bool) {unreachable();}
	virtual void actionZoomIn(bool) = 0;

	void uncheckActive();

protected: // cleaning up debris
	QAction* selectAction();
	MouseActions* mouseActions();

private:
	QUndoStack* undoStack;
};

#endif
