/* 
 * File:   Inimigo.h
 * Author: Rudimar e Wesley
 *
 * Created on 12 de Novembro de 2013, 21:07
 */

#ifndef INIMIGO_H
#define	INIMIGO_H

#include <math.h>

#define M_PI (3.141592653589793)

class Inimigo {
public:

    Inimigo(double x, double z, double angulo) {
        this->x = x;
        this->z = z;
        this->angulo = angulo;
        this->danos = 3;
        this->ativo = true;
    }

    virtual ~Inimigo() {

    }

    bool IsAtivo() const {
        return ativo;
    }

    int GetDanos() const {
        return danos;
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

    void SetAtivo(bool ativo) {
        this->ativo = ativo;
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

    void recebeTiro() {
        danos--;
        if (danos < 0) {
            ativo = false;
        }
    }

    bool move(float maxX, float maxZ) {
        //divisao por 6 reduz a "velocidade" do inimigo
        double novoX = x + cos((angulo * M_PI) / 180)/6;
        double novoZ = z + sin((angulo * M_PI) / 180)/6;

        //teste para verificar se esta dentro dos limites do mundo
        if (!(novoX >= maxX || novoX <= -maxX || novoZ >= maxZ || novoZ <= -maxZ)) {
            this->x = novoX;
            this->z = novoZ;
            return true;
        }

        return false;
    }

private:
    int danos;
    double x;
    double z;
    double angulo;
    bool ativo;
};

#endif	/* INIMIGO_H */

