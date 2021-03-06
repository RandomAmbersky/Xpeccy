#ifndef _DEBUGER_H
#define _DEBUGER_H

#include <QDialog>
#include <QLineEdit>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QKeyEvent>
#include <QTimer>
#include <QItemDelegate>
#include <QMenu>
#include <QTableWidget>

#include "libxpeccy/spectrum.h"
#include "dbg_sprscan.h"
#include "dbg_memfill.h"
#include "dbg_finder.h"

class xTableWidget : public QTableWidget {
	Q_OBJECT
	public:
		xTableWidget(QWidget*);
		Computer* comp;
		int blockStart;
		int blockEnd;
	private:
		int markAdr;
	signals:
		void rqRefill();
	protected:
		void keyPressEvent(QKeyEvent*);
		void mousePressEvent(QMouseEvent*);
		void mouseReleaseEvent(QMouseEvent*);
		void mouseMoveEvent(QMouseEvent*);
};

#include "ui_dumpdial.h"
#include "ui_openDump.h"
#include "ui_debuger.h"

enum {
	XTYPE_NONE = -1,
	XTYPE_ADR = 0,
	XTYPE_LABEL,
	XTYPE_DUMP,
	XTYPE_BYTE
};

struct DasmRow {
	unsigned ispc:1;	// adr=PC
	unsigned cond:1;	// if there is condition command (JR, JP, CALL, RET) and condition met
	unsigned mem:1;		// memory reading
	unsigned short adr;
	unsigned char type;
	unsigned char mop;
	QByteArray bytes;
	QString com;
};

class xItemDelegate : public QItemDelegate {
	public:
		xItemDelegate(int);
		int type;
		QWidget* createEditor (QWidget*, const QStyleOptionViewItem&, const QModelIndex&) const;

};

class DebugWin : public QDialog {
	Q_OBJECT
	public:
		DebugWin(QWidget* = NULL);
		~DebugWin();

		void reject();
		void start(Computer*);
		void stop();

		QMap<QString, xAdr> labels;
	public slots:
		bool fillAll();
	signals:
		void closed();
	private:
		unsigned block:1;
		// tracer
		unsigned trace:1;
		int traceType;
		int traceAdr;

		Ui::Debuger ui;
		QPoint winPos;
		QImage scrImg;
		QList<unsigned short> jumpHistory;

		Computer* comp;
		long tCount;

		QDialog* dumpwin;
		Ui::DumpDial dui;
		QByteArray getDumpData();

		QDialog* openDumpDialog;
		Ui::oDumpDial oui;
		QString dumpPath;

		xMemFiller* memFiller;
		xMemFinder* memFinder;
		MemViewer* memViewer;

		QMenu* cellMenu;
		unsigned short bpAdr;
		void doBreakPoint(unsigned short);
		int getAdr();
		void switchBP(unsigned char);

		QString findLabel(int, int, int);
		void placeLabel(DasmRow&);

		unsigned short disasmAdr;
		unsigned short dumpAdr;

		void fillZ80();
		void fillFlags();
		void fillMem();
		void fillStack();
		void fillFDC();
		void fillRZX();
		void fillAY();

		void fillBrkTable();

		void setFlagNames(const char*);
		void chLayout();

		unsigned short getPrevAdr(unsigned short);
		void scrollDown();
		void scrollUp();

	public slots:
		void loadLabels(QString = QString());
	private slots:
		void setShowLabels(bool);
		void setShowSegment(bool);

		void loadMap();
		void saveMap();
		void saveDasm();
		void saveLabels();

		int fillDisasm();
		void fillDump();
		void fillGBoy();

		void setZ80();
		void setFlags();
		void updateScreen();

		void dasmEdited(int, int);
		void dumpEdited(int, int);

		void putBreakPoint();
		void chaCellProperty(QAction*);
		void goToBrk(QModelIndex);

		void doMemView();
		void doFill();

		void doFind();
		void onFound(int);

		void doTrace(QAction*);
		void doTraceHere();
		void doStep();

		void doOpenDump();
		void doSaveDump();
		void loadDump();
		void chDumpFile();
		void dmpStartOpen();
		void dmpLimChanged();
		void dmpLenChanged();
		void saveDumpBin();
		void saveDumpHobeta();
		void saveDumpToDisk(int);
		void saveDumpToA();
		void saveDumpToB();
		void saveDumpToC();
		void saveDumpToD();
	protected:
		void keyPressEvent(QKeyEvent*);
		void wheelEvent(QWheelEvent*);
};

#endif
