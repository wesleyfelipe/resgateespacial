/* 
 * File:   Tiro.h
 * Author: Admin
 *
 * Created on 15 de Novembro de 2013, 19:12
 */

#ifndef TIRO_H
#define	TIRO_H

#include <math.h>

#define M_PI (3.141592653589793)

class Tiro {
public:
    Tiro(double x, double z, double angulo){
        this->x = x;
        this->z = z;
        this->angulo = angulo;
        this->ativo = true;
    }
    
    bool IsAtivo() const {
        return ativo;
    }

    double GetX() const {
        return x;
    }

    double GetZ() const {
        return z;
    }

    void SetAtivo(bool ativo) {
        this->ativo = ativo;
    }

    void SetX(double x) {
        this->x = x;
    }

    void SetZ(double z) {
        this->z = z;
    }

    void move( float maxX, float maxZ){
        //divisao por 0.7 aumentar a "velocidade" do tiro
        double novoX = x + cos((angulo * M_PI) / 180)/0.7;
        double novoZ = z + sin((angulo * M_PI) / 180)/0.7;

        //teste para verificar se esta dentro dos limites do mundo
        if (!(novoX >= maxX || novoX <= -maxX || novoZ >= maxZ || novoZ <= -maxZ)) {
            this->x = novoX;
            this->z = novoZ;;
        } else {
            ativo = false;
        }
    }
    
    virtual ~Tiro(){}
    
private:
    double x;
    double z;
    double angulo;
    bool ativo;
};

#endif	/* TIRO_H */

