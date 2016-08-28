/* 
 * File:   Sobrevivente.h
 * Author: Rudimar e Wesley
 *
 * Created on 12 de Novembro de 2013, 21:07
 */

#ifndef SOBREVIVENTE_H
#define	SOBREVIVENTE_H

#define M_PI (3.141592653589793)

class Sobrevivente {
public:
    Sobrevivente(double x, double z) {
        this->x = x;
        this->z = z;
        this->save=false;
    }
    
    virtual ~Sobrevivente(){
        
    }
    
    bool IsSave() const {
        return save;
    }

    double GetX() const {
        return x;
    }

    double GetZ() const {
        return z;
    }

    void SetSave(bool save) {
        this->save = save;
    }

    void SetX(double x) {
        this->x = x;
    }

    void SetZ(double z) {
        this->z = z;
    }

private:
    double x;
    double z;
    bool save;

};

#endif	/* SOBREVIVENTE_H */

