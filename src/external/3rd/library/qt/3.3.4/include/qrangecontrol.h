/****************************************************************************
** $Id: qt/qrangecontrol.h   3.3.4   edited May 27 2003 $
**
** Definition of QRangeControl class
**
** Created : 940427
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of the widgets module of the Qt GUI Toolkit.
**
** This file may be distributed under the terms of the Q Public License
** as defined by Trolltech AS of Norway and appearing in the file
** LICENSE.QPL included in the packaging of this file.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid Qt Enterprise Edition or Qt Professional Edition
** licenses may use this file in accordance with the Qt Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.trolltech.com/pricing.html or email sales@trolltech.com for
**   information about Qt Commercial License Agreements.
** See http://www.trolltech.com/qpl/ for QPL licensing information.
** See http://www.trolltech.com/gpl/ for GPL licensing information.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifndef QRANGECONTROL_H
#define QRANGECONTROL_H

#ifndef QT_H
#include "qglobal.h"
#include "qframe.h"
#endif // QT_H

#ifndef QT_NO_RANGECONTROL


class QRangeControlPrivate;


class Q_EXPORT QRangeControl
{
public:
    QRangeControl();
    QRangeControl( int minValue, int maxValue,
		   int lineStep, int pageStep, int value );
    virtual ~QRangeControl();
    int		value()		const;
    void	setValue( int );
    void	addPage();
    void	subtractPage();
    void	addLine();
    void	subtractLine();

    int		minValue()	const;
    int		maxValue()	const;
    void	setRange( int minValue, int maxValue );
    void	setMinValue( int minVal );
    void	setMaxValue( int minVal );

    int		lineStep()	const;
    int		pageStep()	const;
    void	setSteps( int line, int page );

    int		bound( int ) const;

protected:
    int		positionFromValue( int val, int space ) const;
    int		valueFromPosition( int pos, int space ) const;
    void	directSetValue( int val );
    int		prevValue()	const;

    virtual void valueChange();
    virtual void rangeChange();
    virtual void stepChange();

private:
    int		minVal, maxVal;
    int		line, page;
    int		val, prevVal;

    QRangeControlPrivate * d;

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QRangeControl( const QRangeControl & );
    QRangeControl &operator=( const QRangeControl & );
#endif
};


inline int QRangeControl::value() const
{ return val; }

inline int QRangeControl::prevValue() const
{ return prevVal; }

inline int QRangeControl::minValue() const
{ return minVal; }

inline int QRangeControl::maxValue() const
{ return maxVal; }

inline int QRangeControl::lineStep() const
{ return line; }

inline int QRangeControl::pageStep() const
{ return page; }


#endif // QT_NO_RANGECONTROL

#ifndef QT_NO_SPINWIDGET

class QSpinWidgetPrivate;
class Q_EXPORT QSpinWidget : public QWidget
{
    Q_OBJECT
public:
    QSpinWidget( QWidget* parent=0, const char* name=0 );
    ~QSpinWidget();

    void 	setEditWidget( QWidget * widget );
    QWidget * 	editWidget();

    QRect upRect() const;
    QRect downRect() const;

    void setUpEnabled( bool on );
    void setDownEnabled( bool on );

    bool isUpEnabled() const;
    bool isDownEnabled() const;

    enum ButtonSymbols { UpDownArrows, PlusMinus };
    virtual void	setButtonSymbols( ButtonSymbols bs );
    ButtonSymbols	buttonSymbols() const;

    void arrange();

signals:
    void stepUpPressed();
    void stepDownPressed();

public slots:
    void stepUp();
    void stepDown();
    
protected:
    void mousePressEvent( QMouseEvent *e );
    void resizeEvent( QResizeEvent* ev );
    void mouseReleaseEvent( QMouseEvent *e );
    void mouseMoveEvent( QMouseEvent *e );
#ifndef QT_NO_WHEELEVENT
    void wheelEvent( QWheelEvent * );
#endif
    void styleChange( QStyle& );
    void paintEvent( QPaintEvent * );
    void enableChanged( bool old );
    void windowActivationChange( bool );

private slots:
    void timerDone();
    void timerDoneEx();

private:
    QSpinWidgetPrivate * d;

    void updateDisplay();

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QSpinWidget( const QSpinWidget& );
    QSpinWidget& operator=( const QSpinWidget& );
#endif
};

#endif // QT_NO_SPINWIDGET

#endif // QRANGECONTROL_H
