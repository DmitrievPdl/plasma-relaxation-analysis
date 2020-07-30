#include "system.h"
#include<iostream>
#include<cmath>
#define ERR 1.0e-5
#define MAX 1.0e-2
#define MAXDT 1
#define MINDT 1.0e-10


System::System(double Ip0,double E0, double N0,double Im0, double k1, double k2,double k3,double t, double Alpha)
    : Ip_(Ip0),E_(E0),N_(N0),Im_(Im0),k1_(k1),k2_(k2),k3_(k3),t_(t), maxIm_(0.0), Alpha_(Alpha)
{}

System::System(const std::map<std::string, double> &data)
    : System (data.at("Ip0"),data.at("E0"),data.at("N0"),data.at("Im0"),data.at("k1"),data.at("k2"),data.at("k3"),data.at("t"), data.at("Alpha"))
{}
double System::t() const
{
    return t_;
}

double System::E() const
{
    return E_;
}
double System::N() const
{
    return N_;
}
double System::Ip() const
{
    return Ip_;
}
double System::Im() const
{
    return Im_;
}
double System::maxIm() const
{
    return maxIm_;
}
double System::Alpha()
{
    return Alpha_;
}
void System::step(double dt0){
    double K[4][4],Ip2,Ip1,Im2,Im1,N2,N1,E1,E2,mm,dt,dN,dIp,dIm,dE;
    int q;
    dt = dt0;
    do{
        q=1;

        dt*=2;// ШАГ 2dt
        K[0][0] = this->f1(k1_,k2_,k3_,N_,Ip_,Im_,E_);
        K[1][0] = this->f2(k1_,k2_,k3_,N_,Ip_,Im_,E_);
        K[2][0] = this->f3(k1_,k2_,k3_,N_,Ip_,Im_,E_);
        K[3][0] = this->f4(k1_,k2_,k3_,N_,Ip_,Im_,E_);

        K[0][1] = this->f1(k1_,k2_,k3_,N_ + K[0][0]*dt/2,Ip_ + K[1][0]*dt/2,Im_ + K[2][0]*dt/2,E_ + K[3][0]*dt/2);
        K[1][1] = this->f2(k1_,k2_,k3_,N_ + K[0][0]*dt/2,Ip_ + K[1][0]*dt/2,Im_ + K[2][0]*dt/2,E_ + K[3][0]*dt/2);
        K[2][1] = this->f3(k1_,k2_,k3_,N_ + K[0][0]*dt/2,Ip_ + K[1][0]*dt/2,Im_ + K[2][0]*dt/2,E_ + K[3][0]*dt/2);
        K[3][1] = this->f4(k1_,k2_,k3_,N_ + K[0][0]*dt/2,Ip_ + K[1][0]*dt/2,Im_ + K[2][0]*dt/2,E_ + K[3][0]*dt/2);

        K[0][2] = this->f1(k1_,k2_,k3_,N_ + K[0][1]*dt/2,Ip_ + K[1][1]*dt/2,Im_ + K[2][1]*dt/2,E_ + K[3][1]*dt/2);
        K[1][2] = this->f2(k1_,k2_,k3_,N_ + K[0][1]*dt/2,Ip_ + K[1][1]*dt/2,Im_ + K[2][1]*dt/2,E_ + K[3][1]*dt/2);
        K[2][2] = this->f3(k1_,k2_,k3_,N_ + K[0][1]*dt/2,Ip_ + K[1][1]*dt/2,Im_ + K[2][1]*dt/2,E_ + K[3][1]*dt/2);
        K[3][2] = this->f4(k1_,k2_,k3_,N_ + K[0][1]*dt/2,Ip_ + K[1][1]*dt/2,Im_ + K[2][1]*dt/2,E_ + K[3][1]*dt/2);

        K[0][3] = this->f1(k1_,k2_,k3_,N_ + K[0][2]*dt/2,Ip_ + K[1][2]*dt/2,Im_ + K[2][2]*dt/2,E_ + K[3][2]*dt/2);
        K[1][3] = this->f2(k1_,k2_,k3_,N_ + K[0][2]*dt/2,Ip_ + K[1][2]*dt/2,Im_ + K[2][2]*dt/2,E_ + K[3][2]*dt/2);
        K[2][3] = this->f3(k1_,k2_,k3_,N_ + K[0][2]*dt/2,Ip_ + K[1][2]*dt/2,Im_ + K[2][2]*dt/2,E_ + K[3][2]*dt/2);
        K[3][3] = this->f4(k1_,k2_,k3_,N_ + K[0][2]*dt/2,Ip_ + K[1][2]*dt/2,Im_ + K[2][2]*dt/2,E_ + K[3][2]*dt/2);

        N2 = N_ +dt/6*(K[0][0] + 2*K[0][1] + 2*K[0][2] + K[0][3]);
        Ip2 = Ip_ +dt/6*(K[1][0] + 2*K[1][1] + 2*K[1][2] + K[1][3]);
        Im2 = Im_ +dt/6*(K[2][0] + 2*K[2][1] + 2*K[2][2] + K[2][3]);
        E2 = E_ +dt/6*(K[3][0] + 2*K[3][1] + 2*K[3][2] + K[3][3]);


        dt/=2;// ШАГ dt

        K[0][0] = this->f1(k1_,k2_,k3_,N_,Ip_,Im_,E_);
        K[1][0] = this->f2(k1_,k2_,k3_,N_,Ip_,Im_,E_);
        K[2][0] = this->f3(k1_,k2_,k3_,N_,Ip_,Im_,E_);
        K[3][0] = this->f4(k1_,k2_,k3_,N_,Ip_,Im_,E_);

        K[0][1] = this->f1(k1_,k2_,k3_,N_ + K[0][0]*dt/2,Ip_ + K[1][0]*dt/2,Im_ + K[2][0]*dt/2,E_ + K[3][0]*dt/2);
        K[1][1] = this->f2(k1_,k2_,k3_,N_ + K[0][0]*dt/2,Ip_ + K[1][0]*dt/2,Im_ + K[2][0]*dt/2,E_ + K[3][0]*dt/2);
        K[2][1] = this->f3(k1_,k2_,k3_,N_ + K[0][0]*dt/2,Ip_ + K[1][0]*dt/2,Im_ + K[2][0]*dt/2,E_ + K[3][0]*dt/2);
        K[3][1] = this->f4(k1_,k2_,k3_,N_ + K[0][0]*dt/2,Ip_ + K[1][0]*dt/2,Im_ + K[2][0]*dt/2,E_ + K[3][0]*dt/2);

        K[0][2] = this->f1(k1_,k2_,k3_,N_ + K[0][1]*dt/2,Ip_ + K[1][1]*dt/2,Im_ + K[2][1]*dt/2,E_ + K[3][1]*dt/2);
        K[1][2] = this->f2(k1_,k2_,k3_,N_ + K[0][1]*dt/2,Ip_ + K[1][1]*dt/2,Im_ + K[2][1]*dt/2,E_ + K[3][1]*dt/2);
        K[2][2] = this->f3(k1_,k2_,k3_,N_ + K[0][1]*dt/2,Ip_ + K[1][1]*dt/2,Im_ + K[2][1]*dt/2,E_ + K[3][1]*dt/2);
        K[3][2] = this->f4(k1_,k2_,k3_,N_ + K[0][1]*dt/2,Ip_ + K[1][1]*dt/2,Im_ + K[2][1]*dt/2,E_ + K[3][1]*dt/2);

        K[0][3] = this->f1(k1_,k2_,k3_,N_ + K[0][2]*dt/2,Ip_ + K[1][2]*dt/2,Im_ + K[2][2]*dt/2,E_ + K[3][2]*dt/2);
        K[1][3] = this->f2(k1_,k2_,k3_,N_ + K[0][2]*dt/2,Ip_ + K[1][2]*dt/2,Im_ + K[2][2]*dt/2,E_ + K[3][2]*dt/2);
        K[2][3] = this->f3(k1_,k2_,k3_,N_ + K[0][2]*dt/2,Ip_ + K[1][2]*dt/2,Im_ + K[2][2]*dt/2,E_ + K[3][2]*dt/2);
        K[3][3] = this->f4(k1_,k2_,k3_,N_ + K[0][2]*dt/2,Ip_ + K[1][2]*dt/2,Im_ + K[2][2]*dt/2,E_ + K[3][2]*dt/2);

        N1 = N_ +dt/6*(K[0][0] + 2*K[0][1] + 2*K[0][2] + K[0][3]);
        Ip1 = Ip_ +dt/6*(K[1][0] + 2*K[1][1] + 2*K[1][2] + K[1][3]);
        Im1 = Im_ +dt/6*(K[2][0] + 2*K[2][1] + 2*K[2][2] + K[2][3]);
        E1 = E_ +dt/6*(K[3][0] + 2*K[3][1] + 2*K[3][2] + K[3][3]);

        //

        K[0][0] = this->f1(k1_,k2_,k3_,N1,Ip1,Im1,E1);
        K[1][0] = this->f2(k1_,k2_,k3_,N1,Ip1,Im1,E1);
        K[2][0] = this->f3(k1_,k2_,k3_,N1,Ip1,Im1,E1);
        K[3][0] = this->f4(k1_,k2_,k3_,N1,Ip1,Im1,E1);

        K[0][1] = this->f1(k1_,k2_,k3_,N1 + K[0][0]*dt/2,Ip1 + K[1][0]*dt/2,Im1 + K[2][0]*dt/2,E1 + K[3][0]*dt/2);
        K[1][1] = this->f2(k1_,k2_,k3_,N1 + K[0][0]*dt/2,Ip1 + K[1][0]*dt/2,Im1 + K[2][0]*dt/2,E1 + K[3][0]*dt/2);
        K[2][1] = this->f3(k1_,k2_,k3_,N1 + K[0][0]*dt/2,Ip1 + K[1][0]*dt/2,Im1 + K[2][0]*dt/2,E1 + K[3][0]*dt/2);
        K[3][1] = this->f4(k1_,k2_,k3_,N1 + K[0][0]*dt/2,Ip1 + K[1][0]*dt/2,Im1 + K[2][0]*dt/2,E1 + K[3][0]*dt/2);

        K[0][2] = this->f1(k1_,k2_,k3_,N1 + K[0][1]*dt/2,Ip1 + K[1][1]*dt/2,Im1 + K[2][1]*dt/2,E1 + K[3][1]*dt/2);
        K[1][2] = this->f2(k1_,k2_,k3_,N1 + K[0][1]*dt/2,Ip1 + K[1][1]*dt/2,Im1 + K[2][1]*dt/2,E1 + K[3][1]*dt/2);
        K[2][2] = this->f3(k1_,k2_,k3_,N1 + K[0][1]*dt/2,Ip1 + K[1][1]*dt/2,Im1 + K[2][1]*dt/2,E1 + K[3][1]*dt/2);
        K[3][2] = this->f4(k1_,k2_,k3_,N1 + K[0][1]*dt/2,Ip1 + K[1][1]*dt/2,Im1 + K[2][1]*dt/2,E1 + K[3][1]*dt/2);

        K[0][3] = this->f1(k1_,k2_,k3_,N1 + K[0][2]*dt/2,Ip1 + K[1][2]*dt/2,Im1 + K[2][2]*dt/2,E1 + K[3][2]*dt/2);
        K[1][3] = this->f2(k1_,k2_,k3_,N1 + K[0][2]*dt/2,Ip1 + K[1][2]*dt/2,Im1 + K[2][2]*dt/2,E1 + K[3][2]*dt/2);
        K[2][3] = this->f3(k1_,k2_,k3_,N1 + K[0][2]*dt/2,Ip1 + K[1][2]*dt/2,Im1 + K[2][2]*dt/2,E1 + K[3][2]*dt/2);
        K[3][3] = this->f4(k1_,k2_,k3_,N1 + K[0][2]*dt/2,Ip1 + K[1][2]*dt/2,Im1 + K[2][2]*dt/2,E1 + K[3][2]*dt/2);

        N1 += dt/6*(K[0][0] + 2*K[0][1] + 2*K[0][2] + K[0][3]);
        Ip1 += dt/6*(K[1][0] + 2*K[1][1] + 2*K[1][2] + K[1][3]);
        Im1 += dt/6*(K[2][0] + 2*K[2][1] + 2*K[2][2] + K[2][3]);
        E1 += dt/6*(K[3][0] + 2*K[3][1] + 2*K[3][2] + K[3][3]);

        //step selection:

        dN = fabs(N1-N2);
        dIp = fabs(Ip1-Ip2);
        dIm = fabs(Im1-Im2);
        dE = fabs(E1-E2);
        mm = std::max( std::max( std::max( dN,dIp ),dIm ),dE );
        if (mm>ERR){
            if (dt<MINDT){
                q=0;
                dt=MINDT;
            }
            else
                dt/=2;
        }
        if((mm<ERR)&&(mm>ERR*1.0e-1))
            q=0;
        if((mm<ERR)&&(mm<ERR*1.0e-1)){
            if (dt>MAXDT){
                dt=MAXDT;
                q=0;
            }
           else dt*=2;
        }
    } while(q==1);

    Ip_ = Ip1;
    Im_ = Im1;
    N_ = N1;
    E_ = E1;
    t_ += dt;
    if (maxIm_ < Im1)
        maxIm_ = Im1;

}

double System::f1(double k1,double k2,double k3,double N,double Ip,double Im,double E)
{
    return k1*Ip*E - k2*N*E + 2.0*k3*Ip*Im;
}

double System::f2(double k1,double k2,double k3,double N,double Ip,double Im,double E)
{
    return -k1*Ip*E-k3*Ip*Im;
}

double System::f3(double k1,double k2,double k3,double N,double Ip,double Im,double E)
{
    return k2*N*E-k3*Ip*Im;
}

double System::f4(double k1,double k2,double k3,double N,double Ip,double Im,double E)
{
    return -k1*Ip*E-k2*N*E;
}



