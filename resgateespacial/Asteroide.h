/* 
 * File:   Asteroide.h
 * Author: Rudimar e Wesley
 *
 * Created on 12 de Novembro de 2013, 21:07
 */

#ifndef ASTEROIDE_H
#define	ASTEROIDE_H

#include <math.h>

#define M_PI (3.141592653589793)

class Asteroide {
public:
    Asteroide(double x, double z, double angulo, double y){
        this->x=x;
        this->z=z;
        this->angulo=angulo;
        this->destruido=false;
        this->y=y;
        this->sobe=true;
    }
    
    bool IsDestruido() const {
        return destruido;
    }

    double GetX() const {
        return x;
    }

    double GetZ() const {
        return z;
    }
    
    double GetAngulo() const {
        return angulo;
    }
    
    double GetY() const {
        return y;
    }
    
    void SetDestruido(bool destruido) {
        this->destruido = destruido;
    }

    void SetX(double x) {
        this->x = x;
    }

    void SetZ(double z) {
        this->z = z;
    }
    
    void SetAngulo(double angulo) {
        this->angulo = angulo;
    }
    
    void SetY(double y) {
        this->y = y;
    }

    bool move( float maxX, float maxZ){
        //divisao por 4 reduz a "velocidade" do asteroide
        double novoX = x + cos((angulo*M_PI)/180)/4;
        double novoZ = z + sin((angulo*M_PI)/180)/4;
        
        //teste para verificar se esta dentro dos limites do mundo
        if( !(novoX >= maxX || novoX <= -maxX || novoZ >= maxZ || novoZ <= -maxZ)){
            this->x=novoX;
            this->z=novoZ;
            //ajuste de direcao "vertical" do movimento do asteroide
            if(y>=0)
                sobe=false;
            else if(y<=-1)
                sobe=true;
            //movimento "vertical
            if(sobe)
                y+=0.06;
            else
                y-=0.06;
            
            return true;
        }
        
        return false;
    }
    
    virtual ~Asteroide(){}
    
private:
    double x;
    double z;
    double angulo;
    bool destruido;
    double y;
    bool sobe;
};

#endif	/* ASTEROIDE_H */

