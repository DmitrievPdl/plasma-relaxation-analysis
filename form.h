#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QTextEdit>
#include <QTimer>
#include "system.h"
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

class Form : public QWidget
{
    Q_OBJECT

public:
    Form(QWidget *parent = 0);
    ~Form();
private:
    QLabel * labelTotalAmount;
    QLabel * labelAlpha;
    QLabel * labelK1;
    QLabel * labelK2;
    QLabel * labelK3;

    QDoubleSpinBox * doubleSpinBoxTotalAmount;
    QDoubleSpinBox * doubleSpinBoxAlpha;
    QDoubleSpinBox * doubleSpinBoxK1;
    QDoubleSpinBox * doubleSpinBoxK2;
    QDoubleSpinBox * doubleSpinBoxK3;

    QPushButton * pushButtonStart;
    QPushButton * pushButtonStop;
    QPushButton * pushButtonImMaxCalculation;

    QTextEdit * textEditLog;
    System * sys;
    QTimer *timer, *timer2, *timer3;

    QwtPlot * plot,*plotMaxIm;
    QwtPlotCurve *curveIp, *curveIm, *curveN,*curveE, *curveMaxIm;
    QVector<QPointF> dataIp, dataIm, dataN,dataE, dataMaxIm;
public slots:
    void startCalculation();
    void stopCalculation();
    void nextStep();
    void nextStep2();
    void startCalculationMaxIm();
};

#endif // FORM_H


