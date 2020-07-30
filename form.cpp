#include "form.h"
#include <QLayout>
#include<QString>
#include <qwt_plot_grid.h>
#include <qwt_legend.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>

Form::Form(QWidget *parent)
    : QWidget(parent), sys(nullptr)
{
        labelTotalAmount = new QLabel("Total Amount");
        labelAlpha = new QLabel("Degree of ionization");
        labelK1 = new QLabel("Velocity of reaction:I(+)+e(-) -> N");
        labelK2 = new QLabel("Velocity of reaction:N+e(-) -> I(-)");
        labelK3 = new QLabel("Velocity of reaction:I(+)+I(-) -> 2N");

        doubleSpinBoxTotalAmount = new QDoubleSpinBox;
        doubleSpinBoxAlpha = new QDoubleSpinBox;
        doubleSpinBoxK1 = new QDoubleSpinBox;
        doubleSpinBoxK2 = new QDoubleSpinBox;
        doubleSpinBoxK3 = new QDoubleSpinBox;

        doubleSpinBoxTotalAmount->setMaximum(1);
        doubleSpinBoxTotalAmount->setSingleStep(0.01);
        doubleSpinBoxAlpha->setMaximum(1);
        doubleSpinBoxAlpha->setSingleStep(0.01);
        doubleSpinBoxK1->setSingleStep(0.01);
        doubleSpinBoxK2->setSingleStep(1.0);
        doubleSpinBoxK3->setSingleStep(0.01);
        doubleSpinBoxK2->setMaximum(100);

        //параметры по умолчанию
        doubleSpinBoxTotalAmount->setValue(0.5);
        doubleSpinBoxAlpha->setValue(0.5);
        doubleSpinBoxK1->setValue(0.1);
        doubleSpinBoxK2->setValue(0.1);
        doubleSpinBoxK3->setValue(0.1);

        pushButtonStart = new QPushButton("Start");
        pushButtonStop = new QPushButton("Stop");
        pushButtonImMaxCalculation = new QPushButton("Im Max Calculation");

        textEditLog = new QTextEdit;
        textEditLog->setReadOnly(true);

        //PLOT
        plot = new QwtPlot;
        plot->setTitle("Time plots");
        plot->setAxisTitle(QwtPlot::xBottom, "Time (s)");
        plot->setAxisTitle(QwtPlot::yLeft, "Ip, Im, N, E");
        plot->setCanvasBackground(QBrush(Qt::white));

        plotMaxIm = new QwtPlot;
        plotMaxIm->setTitle("Maximum negative ions");
        plotMaxIm->setAxisTitle(QwtPlot::xBottom, "Total Amount");
        plotMaxIm->setAxisTitle(QwtPlot::yLeft, "MaxIm");
        plotMaxIm->setCanvasBackground(QBrush(Qt::white));

        QwtPlotMagnifier *magnifier = new QwtPlotMagnifier(plot->canvas());
        magnifier->setMouseButton(Qt::MiddleButton);
        QwtPlotPanner *panner = new QwtPlotPanner(plot->canvas());
        panner->setMouseButton(Qt::LeftButton);

        QwtPlotGrid *grid = new QwtPlotGrid;
        grid->attach(plot);
        QwtPlotGrid *grid2 = new QwtPlotGrid;
        grid2->attach(plotMaxIm);

        curveMaxIm = new QwtPlotCurve;
        curveMaxIm->setPen(Qt::black, 4.0);
        curveMaxIm->attach(plotMaxIm);

        curveIp = new QwtPlotCurve;
        curveIm = new QwtPlotCurve;
        curveN = new QwtPlotCurve;
        curveE = new QwtPlotCurve;
        curveIp->setPen(Qt::red, 4.0);
        curveIm->setPen(Qt::blue, 4.0, Qt::DashLine);
        curveN->setPen(Qt::black, 4.0);
        curveE->setPen(Qt::green, 4.0, Qt::DashLine);
        curveIp->setTitle("Ip");
        curveIm->setTitle("Im");
        curveN->setTitle("N");
        curveE->setTitle("E");
        curveIp->attach(plot);
        curveIm->attach(plot);
        curveN->attach(plot);
        curveE->attach(plot);


        QwtLegend *legend = new QwtLegend;
        plot->insertLegend(legend);


        QGridLayout * layoutParameters = new QGridLayout;
        layoutParameters->addWidget(labelTotalAmount, 0, 0);
        layoutParameters->addWidget(labelAlpha, 1, 0);
        layoutParameters->addWidget(labelK1, 2, 0);
        layoutParameters->addWidget(labelK2, 3, 0);
        layoutParameters->addWidget(labelK3, 4, 0);

        layoutParameters->addWidget(doubleSpinBoxTotalAmount, 0, 1);
        layoutParameters->addWidget(doubleSpinBoxAlpha, 1, 1);
        layoutParameters->addWidget(doubleSpinBoxK1, 2, 1);
        layoutParameters->addWidget(doubleSpinBoxK2, 3, 1);
        layoutParameters->addWidget(doubleSpinBoxK3, 4, 1);

        QHBoxLayout * layoutButtons = new QHBoxLayout;
            layoutButtons->addWidget(pushButtonStart);
            layoutButtons->addWidget(pushButtonStop);
            layoutButtons->addWidget(pushButtonImMaxCalculation);

            QVBoxLayout * layoutLeft = new QVBoxLayout;
            layoutLeft->addLayout(layoutParameters);
            layoutLeft->addLayout(layoutButtons);
            layoutLeft->addWidget(textEditLog);

            QVBoxLayout *layoutRight = new QVBoxLayout;
            layoutRight->addWidget(plot);
            layoutRight->addWidget(plotMaxIm);

            QHBoxLayout * layoutMain = new QHBoxLayout;
            layoutMain->addLayout(layoutLeft);
            layoutMain->addLayout(layoutRight);

            this->setLayout(layoutMain);

        timer = new QTimer;
        timer->setInterval(0);
        timer2 = new QTimer;
        timer2->setInterval(0);


        this->setLayout(layoutMain);
        connect(pushButtonStart, SIGNAL(clicked()), this, SLOT(startCalculation()));
        connect(pushButtonStop, SIGNAL(clicked()), this, SLOT(stopCalculation()));
        connect(pushButtonImMaxCalculation, SIGNAL(clicked()), this, SLOT(startCalculationMaxIm()));
        connect(timer, SIGNAL(timeout()), this, SLOT(nextStep()));
        connect(timer2, SIGNAL(timeout()), this, SLOT(nextStep2()));
}

Form::~Form()
{

}

void Form::startCalculation()
{
    textEditLog->clear();
    delete sys;
    dataIp.clear();
    dataIm.clear();
    dataN.clear();
    dataE.clear();
    sys = new System({
            {"Ip0", doubleSpinBoxTotalAmount->value() * doubleSpinBoxAlpha->value()},
            {"E0", doubleSpinBoxTotalAmount->value() * doubleSpinBoxAlpha->value()},
            {"N0", (1.0-doubleSpinBoxAlpha->value()) * doubleSpinBoxTotalAmount->value()},
            {"Im0", 0.0},
            {"k1", doubleSpinBoxK1->value()},
            {"k2", doubleSpinBoxK2->value()},
            {"k3", doubleSpinBoxK3->value()},
            {"t", 0},
            {"Alpha", doubleSpinBoxAlpha->value()}
        });
    textEditLog->append(QString::number(sys->t())+'\t'+
                        QString::number(sys->Ip())+'\t'+
                        QString::number(sys->Im())+'\t'+
                        QString::number(sys->N())+'\t'+
                        QString::number(sys->E()));

    dataIp.push_back(QPointF(sys->t(), sys->Ip()));
    dataIm.push_back(QPointF(sys->t(), sys->Im()));
    dataN.push_back(QPointF(sys->t(), sys->N()));
    dataE.push_back(QPointF(sys->t(), sys->E()));
    curveIp->setSamples(dataIp);
    curveIm->setSamples(dataIm);
    curveN->setSamples(dataN);
    curveE->setSamples(dataE);
    plot->replot();
    timer->start();
}

void Form::stopCalculation()
{
    timer->stop();
    timer2->stop();
}

void Form::nextStep()
{
    double dt = 0.001;
    sys->step(dt);

    textEditLog->append(QString::number(sys->t())+'\t'+
                            QString::number(sys->Ip())+'\t'+
                            QString::number(sys->Im())+'\t'+
                           QString::number(sys->N())+'\t'+
                           QString::number(sys->E()));

    dataIp.push_back(QPointF(sys->t(), sys->Ip()));
    dataIm.push_back(QPointF(sys->t(), sys->Im()));

    dataN.push_back(QPointF(sys->t(), sys->N()));
    dataE.push_back(QPointF(sys->t(), sys->E()));

    curveIp->setSamples(dataIp);
    curveIm->setSamples(dataIm);
    curveN->setSamples(dataN);
    curveE->setSamples(dataE);
    plot->replot();
    if (sys->t() > 200.0)
        timer->stop();
}
void Form::startCalculationMaxIm(){
    textEditLog->clear();
    delete sys;
    dataIp.clear();
    dataIm.clear();
    dataN.clear();
    dataE.clear();
    double InitialAlpha = 0.1, InitialK2 = 0.1;
    doubleSpinBoxAlpha->setValue(InitialAlpha);
    doubleSpinBoxK2 ->setValue(InitialK2);
    sys = new System({
                    {"Ip0", doubleSpinBoxTotalAmount->value() * doubleSpinBoxAlpha->value()},
                    {"E0", doubleSpinBoxTotalAmount->value() * doubleSpinBoxAlpha->value()},
                    {"N0", (1.0-doubleSpinBoxAlpha->value()) * doubleSpinBoxTotalAmount->value() },
                    {"Im0", 0.0},
                    {"k1", doubleSpinBoxK1->value()},
                    {"k2", doubleSpinBoxK2->value()},
                    {"k3", doubleSpinBoxK3->value()},
                    {"t", 0},
                    {"Alpha", doubleSpinBoxAlpha->value()}
                });
    timer2->start();
}
void Form::nextStep2()
{
    double dt = 0.001;
    sys->step(dt);

    dataIp.push_back(QPointF(sys->t(), sys->Ip()));
    dataIm.push_back(QPointF(sys->t(), sys->Im()));

    dataN.push_back(QPointF(sys->t(), sys->N()));
    dataE.push_back(QPointF(sys->t(), sys->E()));

    curveIp->setSamples(dataIp);
    curveIm->setSamples(dataIm);
    curveN->setSamples(dataN);
    curveE->setSamples(dataE);
    plot->replot();
    if (sys->t() > 200.0){
        textEditLog->append('('+ QString::number(doubleSpinBoxK2->value())+','+'\t' + QString::number(doubleSpinBoxAlpha->value())+','+'\t'+
                         QString::number(sys->maxIm()) + ')');
        dataMaxIm.push_back(QPointF(doubleSpinBoxAlpha->value(), sys->maxIm()));
        curveMaxIm->setSamples(dataMaxIm);
        plotMaxIm->replot();
        dataIp.clear();
        dataIm.clear();
        dataN.clear();
        dataE.clear();
        doubleSpinBoxAlpha->setValue(doubleSpinBoxAlpha->value() + 0.1);
        delete sys;
        sys = new System({
                        {"Ip0", doubleSpinBoxTotalAmount->value() * doubleSpinBoxAlpha->value()},
                        {"E0", doubleSpinBoxTotalAmount->value() * doubleSpinBoxAlpha->value()},
                        {"N0", (1.0-doubleSpinBoxAlpha->value()) * doubleSpinBoxTotalAmount->value() },
                        {"Im0", 0.0},
                        {"k1", doubleSpinBoxK1->value()},
                        {"k2", doubleSpinBoxK2->value()},
                        {"k3", doubleSpinBoxK3->value()},
                        {"t", 0},
                        {"Alpha", doubleSpinBoxAlpha->value()},
                        {"maxIm",0.0}
                    });
    }

   //
    if (doubleSpinBoxAlpha->value() == 0.8){
        //textEditLog->append(QString::number(doubleSpinBoxK1->value())+'\t' + QString::number(doubleSpinBoxAlpha->value())+'\t'+
        //                 QString::number(sys->maxIm()));
        //dataMaxIm.push_back(QPointF(doubleSpinBoxAlpha->value(), sys->maxIm()));
        //curveMaxIm->setSamples(dataMaxIm);
        //plotMaxIm->replot();
        dataIp.clear();
        dataIm.clear();
        dataN.clear();
        dataE.clear();
        dataMaxIm.clear();
        doubleSpinBoxAlpha->setValue(0.1);
        doubleSpinBoxK2->setValue(doubleSpinBoxK2->value() + 0.5);
        delete sys;
        sys = new System({
                        {"Ip0", doubleSpinBoxTotalAmount->value() * doubleSpinBoxAlpha->value()},
                        {"E0", doubleSpinBoxTotalAmount->value() * doubleSpinBoxAlpha->value()},
                        {"N0", (1.0-doubleSpinBoxAlpha->value()) * doubleSpinBoxTotalAmount->value() },
                        {"Im0", 0.0},
                        {"k1", doubleSpinBoxK1->value()},
                        {"k2", doubleSpinBoxK2->value()},
                        {"k3", doubleSpinBoxK3->value()},
                        {"t", 0},
                        {"Alpha", doubleSpinBoxAlpha->value()},
                        {"maxIm",0.0}
                    });
        }
}
