#ifndef SYSTEM_H
#define SYSTEM_H


#include <string>
#include <map>

class System
{
public:
    System(double Ip0, double E0, double N0, double Im0, double k1, double k2, double k3, double t, double Alpha);
    System(const std::map<std::string, double >& data);
    double Ip() const;
    double E() const;
    double N() const;
    double Im() const;
    double t() const;
    double maxIm() const;
    void step(double dt0);
    double Alpha();
    double changeTotalAmount(double dTotalAmount);
private:
    double Ip_;
    double E_;
    double N_;
    double Im_;
    double k1_;
    double k2_;
    double k3_;
    double t_;
    double maxIm_;
    double Alpha_;
    double f1(double k1,double k2,double k3,double N,double Ip,double Im,double E);
    double f2(double k1,double k2,double k3,double N,double Ip,double Im,double E);
    double f3(double k1,double k2,double k3,double N,double Ip,double Im,double E);
    double f4(double k1,double k2,double k3,double N,double Ip,double Im,double E);
};
#endif // SYSTEM_H

