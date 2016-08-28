/* 
 * File:   main.cpp
 * Author: Rudimar e Wesley
 *
 * IMPORTANTES:
 * REVISAR CONTROLE DE TEMPO PARA OS ROUNDS
 * CRIAR MENSAGENS DE GAME WIN, GAME OVER, FIM DO ROUND.
 * EXIBIR PARA JOGADOR SUA QTDE DE MUNICAO, VIDAS RESTANTES, TEMPO PARA ENCERRAR ROUND, NUM DE SOBREVIVENTES QUE FALTA RESGATAR
 * 
 * MENOS IMPORTANTES:
 * DESENVOLVER TESTES DE COLISAO MAIS SOFISTICADOS (se possivel) - ACHO QUE NÃO PRECISA (SOH CASO MUDE DE FORMA)
 * CALIBRAR PRECISAO DOS TIROS QUE ESTA "AUMENTADA" PARA FACILITAR A COLISAO DO TIRO E DO INIMIGO/ASTEROIDE
 * 
 * 
 * OPCIONIAS:
 * CRIAR UMA MIRA (OPCIONAL, MAS ACHO QUE IA FICAR SHOW)
 * DESENHAR CHAO COMO UMA MALHA GEOMETRICA
 * 
 * Created on 4 de Novembro de 2013, 20:25
 */

#include <windows.h>
#include <gl/glu.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>

#include "Asteroide.h"
#include "Inimigo.h"
#include "Sobrevivente.h"
#include "Tiro.h"

using namespace std;

#define M_PI (3.141592653589793)

//informacoes do jogador
int vidas = 3;
int sobreviventesSalvos = 0;
int numTiros = 60;
Tiro **tiros = new Tiro*[numTiros];
int tirosDados = 0;

//flag para controlar final do round por tempo
int flag = 0;

//dimensoes da tela
int width = 800;
int height = 600;

//posicao da camera no mundo
float eyex = 0;
float eyey = 0;
float eyez = 0;

//direcao em que aponta a camera
float dx = 0;
float dy = 0;
float dz = -1;

//altura
float px = 0;
float py = 1;
float pz = 0;

//angulo
float a = 270;

//tamanho do mundo 400x400 (maxX e maxZ sao as dimensoes maximas do mundo em cada quadrante)
int maxX = 200;
int maxZ = 200;

//personagens
int numAsteroides = 20;
Asteroide **asteroides = new Asteroide*[numAsteroides]; 

int numInimigos = 15;
Inimigo **inimigos = new Inimigo*[numInimigos]; 

int numSobreviventes = 5;
Sobrevivente **sobreviventes = new Sobrevivente*[numSobreviventes];


/*
 * desenha textos
 */
//void drawText(char msg[], float x, float y, float charWidth, char fonte) {
//    for (int i = 0; i < (strlen(msg)); i++) {
//        glRasterPos3d(x, y, 2);
//        // A fonte, neste caso, é uma constante da GLUT.
//        switch (fonte) {
//            case '1':
//                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (char) msg[i]);
//                break;
//            case '2':
//                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (char) msg[i]);
//                break;
//            case '3':
//                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, (char) msg[i]);
//                break;
//            case '4':
//                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (char) msg[i]);
//                break;
//            case '5':
//                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (char) msg[i]);
//                break;
//            case '6':
//                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (char) msg[i]);
//                break;
//            default:
//                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, (char) msg[i]);
//                break;
//        }
//        // inc ponteiro msg
//        x += charWidth;
//    }
//}

/*
 * desenha quadrados, usados para desenhar inimigos, asteroides e sobreviventes no metodo display
 */
void desenhaPersonagem( double x, double y, double z, float r, float g, float b, float dim){
    glColor3f( r, g, b);
    glBegin(GL_QUADS);
        glVertex3d(x, y, z);
        glVertex3d(x+dim, y, z);
        glVertex3d(x+dim, y+dim, z);
        glVertex3d(x, y+dim, z); 
    glEnd();
    
    r-=0.1; g-=0.1; b-=0.1;
    glColor3f( r, g, b);
    
    glBegin(GL_QUADS);
    glVertex3d(x+2, y, z);
        glVertex3d(x+dim, y, z-dim);
        glVertex3d(x+dim, y+dim, z-dim);
        glVertex3d(x+dim, y+dim, z);
    glEnd();
    
    r-=0.1; g-=0.1; b-=0.1;
    glColor3f( r, g, b);
    
    glBegin(GL_QUADS);
        glVertex3d(x+dim, y, z-dim);
        glVertex3d(x, y, z-dim);
        glVertex3d(x, y+dim, z-dim);
        glVertex3d(x+dim, y+dim, z-dim);
    glEnd();
    
    r+=0.1; g+=0.1; b+=0.1;
    glColor3f( r, g, b);
    
    glBegin(GL_QUADS);
        glVertex3d(x, y, z-dim);
        glVertex3d(x, y, z);
        glVertex3d(x, y+dim, z);
        glVertex3d(x, y+dim, z-dim);
    glEnd();
}

/*
 * desenha tiros
 */
void desenhaTiro( double x, double y, double z, float r, float g, float b){
    
    glColor3f( r, g, b);
    glBegin(GL_QUADS);
        glVertex3d(x-0.1, y, z+0.1);
        glVertex3d(x+0.1, y, z+0.1);
        glVertex3d(x+0.1, y+0.2, z+0.1);
        glVertex3d(x-0.1, y+0.2, z+0.1);
    glEnd();
    
    r-=0.1; g-=0.1; b-=0.1;
    glColor3f( r, g, b);
    
    glBegin(GL_QUADS);
        glVertex3d(x+0.1, y, z+0.1);
        glVertex3d(x-0.1, y, z-0.1);
        glVertex3d(x-0.1, y+0.2, z-0.1);
        glVertex3d(x+0.1, y+0.2, z+0.1);
    glEnd();
    
    r-=0.1; g-=0.1; b-=0.1;
    glColor3f( r, g, b);
    
    glBegin(GL_QUADS);
        glVertex3d(x-0.1, y, z-0.1);
        glVertex3d(x+0.1, y, z-0.1);
        glVertex3d(x+0.1, y+0.2, z-0.1);
        glVertex3d(x-0.1, y+0.2, z-0.1);
    glEnd();
    
    r+=0.1; g+=0.1; b+=0.1;
    glColor3f( r, g, b);
    
    glBegin(GL_QUADS);
        glVertex3d(x-0.1, y, z-0.1);
        glVertex3d(x-0.1, y, z+0.1);
        glVertex3d(x+0.1, y+0.2, z+0.1);
        glVertex3d(x+0.1, y+0.2, z-0.1);
    glEnd();
}

/*
 * desenha asteroides, inimigos, sobreviventes e tiros no mundo
 */
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if( vidas == 0){        
        //exibir mensagem de game over
        exit(0);
    }else if( sobreviventesSalvos == numSobreviventes){
        //exibir mensagem de game win
        exit(0);
    }else{
        //desenha quadrante 4 - red
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_QUADS);
            glVertex3d(0, -1, 0);
            glVertex3d(0, -1, -maxZ);
            glVertex3d(maxX, -1, -maxZ);
            glVertex3d(maxX, -1, 0);
        glEnd();

        //desenha quadrante 3 - green
        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_QUADS);
            glVertex3d(0, -1, 0);
            glVertex3d(0, -1, -maxZ);
            glVertex3d(-maxX, -1, -maxZ);
            glVertex3d(-maxX, -1, 0);
        glEnd();

        //desenha quadrante 2 - blue
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_QUADS);
            glVertex3d(0, -1, 0);
            glVertex3d(0, -1, maxZ);
            glVertex3d(-maxX, -1, maxZ);
            glVertex3d(-maxX, -1, 0);
        glEnd();

        //desenha quadrante 1 - yellow
        glColor3f(1.0, 1.0, 0.0);
        glBegin(GL_QUADS);
            glVertex3d(0, -1, 0);
            glVertex3d(maxX, -1, 0);
            glVertex3d(maxX, -1, maxZ);
            glVertex3d(0, -1, maxZ);
        glEnd();

        //desenha asteroides - amarelado 
        for (int i = 0; i < numAsteroides; i++) {
            if(!asteroides[i]->IsDestruido()){
                double x = asteroides[i]->GetX();
                double z = asteroides[i]->GetZ();
                double y = asteroides[i]->GetY();
                desenhaPersonagem( x, y, z, 1.0, 0.9, 0.0, 2.0);
            }
        }

        //desenha inimigos - tons de vermelho
        for (int i = 0; i < numInimigos; i++) {
            if(inimigos[i]->IsAtivo()){
                double x = inimigos[i]->GetX();
                double z = inimigos[i]->GetZ();

                float r, g, b;
                //teste para definir cor do inimigo de acordo com seu numero de danos
                if(inimigos[i]->GetDanos() == 0){
                    r=1.0;
                    g=0.0;
                    b=0.0;
                }else if(inimigos[i]->GetDanos() == 1){
                    r=1.0;
                    g=0.25;
                    b=0.25;
                }else{
                    r=1.0;
                    g=0.5;
                    b=0.5;
                }

                desenhaPersonagem(x,-1.0, z, r, g, b, 2.0);
            }
        }


        //desenha tiros - verde
        for (int i = 0; i < tirosDados; i++) {
            if(tiros[i]->IsAtivo()){
                double x = tiros[i]->GetX();
                double z = tiros[i]->GetZ();
                desenhaTiro( x, -0.1, z, 0.0, 0.7, 0.0);
            }
        }

        //desenha sobreviventes - ciano
        for (int i = 0; i < numSobreviventes; i++) {
            if(!sobreviventes[i]->IsSave()){
                double x = sobreviventes[i]->GetX();
                double z = sobreviventes[i]->GetZ();
                
                desenhaPersonagem( x, -1.0, z, 0.0, 1.0, 1.0, 2.0);
            }
        }
        
        //***************************************TESTE MIRA*********************************************
        // desenho de elipse da mira - verde
//        glColor3f(0.0, 1.0, 0.0);
//            GLfloat circle_points = 50;
//            GLfloat angle=0.0f;
//            GLfloat raioX = 0.1;
//            GLfloat raioY = 0.1;
//	glBegin(GL_LINE_LOOP);
//            for (int i = 0; i < circle_points; i++) {
//                    angle = 2*M_PI*i/circle_points;
//                    glVertex3f(dx +cos(angle)*raioX, sin(angle)*raioY, dz);
//            }
//	glEnd();
//        
//        glBegin(GL_LINES);
//            glVertex3f(dx, -0.1, dz);
//            glVertex3f(dx, 0.1, dz);
//            glVertex3f(dx, 0, dz);
//            glVertex3f(dx+0.1, 0, dz+0.1);
//            glVertex3f(dx, 0, dz);
//            glVertex3f(dx-0.1, 0, dz-0.1);
//            angle = 0;
//            glVertex3f(dx, sin(angle)*raioY, dz);
//            angle = 270;
//            glVertex3f(dx, sin(angle)*raioY, dz);
//        glEnd();
       //***************************************TESTE*********************************************
    }
    
    // Buffer swap
    glutSwapBuffers();
}

/*
 * configura camera
 */
void configView(void) {
    // tamanho da viewport
    glViewport(0, 0, width, height);
    // modo definição de matriz de visualizacao
    glMatrixMode(GL_PROJECTION);
    // limpa matrizes
    glLoadIdentity();
    // define câmera com projeção perspectiva
    gluPerspective(70, (double) width / (double) height, 1, 70);
    
    gluLookAt(eyex, eyey, eyez, dx, dy, dz, px, py, pz);
    //modo transformaçòes e renderização de objetos
    glMatrixMode(GL_MODELVIEW);
    //limpa matrizes
    glLoadIdentity();
}

/*
 * inicializacao do jogo
 */
void init(void) {
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    
    srand((unsigned) time(NULL));

    //criacao dos asteroides
    for(int i=0; i<numAsteroides; i++){
        //soteio da posicao xz inicial
        double x =  pow(-1, 1 + rand() % 2) * (rand()% maxX);
        double z =  pow(-1, 1 + rand() % 2) * (rand()% maxZ);
        //sorteio do angulo de da direcao do asteroide
        double angulo = rand()%360;
        //sorteio da "altura" do asteroide (entre 0 e -2) Obs.: nao eh a altura da forma, mas a altura y do ponto 3d que define o asteroide
        double y = - (rand()% 2);
        Asteroide *ast = new Asteroide( x, z, angulo, y);
        asteroides[i] = ast;
    }
    
    //criacao dos inimigos
    for(int i=0; i<numInimigos; i++){
        //soteio da posicao inicial
        double x =  pow(-1, 1 + rand() % 2) * (rand()% maxX);
        double z =  pow(-1, 1 + rand() % 2) * (rand()% maxX);
        //sorteio do angulo de da direcao do asteroide
        double angulo = rand()%360;
        
        Inimigo *ini = new Inimigo( x, z, angulo );
        inimigos[i] = ini;
    }
    
    //criacao dos sobreviventes
    for(int i=0; i<numSobreviventes; i++){
        //soteio da posicao inicial
        double x =  pow(-1, 1 + rand() % 2) * (rand()% maxX);
        double z =  pow(-1, 1 + rand() % 2) * (rand()% maxX);
        Sobrevivente *sob = new Sobrevivente( x, z );
        sobreviventes[i] = sob;
    }

    //posicao da camera no mundo
    eyex = 0;
    eyey = 0;
    eyez = 0;
    //direcao em que aponta a camera
    dx = 0;
    dy = 0;
    dz = -1;
    //angulo
    a = 270;
    
    //zera dados do jogador (exceto num de vidas)
    sobreviventesSalvos = 0;
    tirosDados = 0;
    
    configView();
    
}

/*
 * movimenta objetos (asteroides, inimigos, tiros) no mundo
 */
void movimentacao(){
    
    //movimentacao dos asteroides
    for (int i = 0; i < numAsteroides; i++) {
        if(!asteroides[i]->IsDestruido()){
            bool moveu = asteroides[i]->move( maxX, maxZ );
            
            while( !moveu ){
                asteroides[i]->SetAngulo( rand()%360 );
                moveu = asteroides[i]->move( maxX, maxZ );
            }
        }
    }
    
    //movimentacao dos inimigos
    for (int i = 0; i < numInimigos; i++) {
        if(inimigos[i]->IsAtivo()){
            
            //calculo de distancia entre o ponto do inimigo e da camera (nave)
            double distancia = sqrt(pow(eyex-inimigos[i]->GetX(), 2) + pow(eyez-inimigos[i]->GetZ(), 2));
            //se a distancia for menor que 25, o inimigo começa a se mover em direcao a camera (nave)
            //atualiza angulo de trajetoria do inimigo
            if( distancia < 25){
                double ang;
                
                if( eyez > inimigos[i]->GetZ()){
                    ang = (acos((eyex-inimigos[i]->GetX())/distancia))*180/M_PI;
                } else {
                    ang = 360 - (acos((eyex-inimigos[i]->GetX())/distancia))*180/M_PI;
                }
                
                inimigos[i]->SetAngulo(ang);
            }
            
            bool moveu = inimigos[i]->move( maxX, maxZ );
            
            while( !moveu ){
                inimigos[i]->SetAngulo( rand()%360 );
                moveu = inimigos[i]->move( maxX, maxZ );
            }
        }
    }
    
    //movimentacao dos tiros
    for (int i = 0; i < tirosDados; i++) {
        if( tiros[i]->IsAtivo()){
            tiros[i]->move( maxX, maxZ);
        }
    }

    
}

/*
 * verifica se algum tiro disparado atingiu algum alvo
 */
void testaTiros(){
    for (int i = 0; i < tirosDados; i++) {
        if(tiros[i]->IsAtivo()){
            
            //tiros em asteroides
            for (int j = 0; j < numAsteroides; j++) {
                if(!asteroides[j]->IsDestruido()){
                    if((asteroides[j]->GetZ() >= tiros[i]->GetZ()-2 && asteroides[j]->GetZ() <= tiros[i]->GetZ()+2) &&
                      ( (asteroides[j]->GetX() >= tiros[i]->GetX()-0.5 && asteroides[j]->GetX() <= tiros[i]->GetX()+0.5) ||
                        (asteroides[j]->GetX()+2 >= tiros[i]->GetX()-0.5 && asteroides[j]->GetX()+2 <= tiros[i]->GetX()+0.5))){
                        asteroides[j]->SetDestruido(true);
                        tiros[i]->SetAtivo(false);
                    }
                }
            }
            
            //tiros em inimigos
            for (int j = 0; j < numInimigos; j++) {
                if(inimigos[j]->IsAtivo()){
                    if((inimigos[j]->GetZ() >= tiros[i]->GetZ()-2 && inimigos[j]->GetZ() <= tiros[i]->GetZ()+2) &&
                      ( (inimigos[j]->GetX() >= tiros[i]->GetX()-0.5 && inimigos[j]->GetX() <= tiros[i]->GetX()+0.5) ||
                        (inimigos[j]->GetX()+2 >= tiros[i]->GetX()-0.5 && inimigos[j]->GetX()+2 <= tiros[i]->GetX()+0.5))){
                        inimigos[j]->recebeTiro();
                        tiros[i]->SetAtivo(false);
                    }
                }
            }


        }
    }

}

/** 
 * Retorna true se colidir com asteroide ou inimigo
 * caso contrário, retorna false
 * caso colida com sobreviventes, incrementa o contador de sobreviventes salvos
 */
bool testaColisoes(void){
    
        
    //testa se algum tiro atingiu algum asteroide ou inimigo ou saiu do mundo
    testaTiros();
    
    //testando colisoes com asteroides
    for (int i = 0; i < numAsteroides; i++) {
        //testa 1º se esta dentro do alcance em z da nave (bound volumm)
        // depois testa se os pontos do asteroide (x ou x+2) se encontram dentro do alcance x da nave
        if(!asteroides[i]->IsDestruido()){
            if( (asteroides[i]->GetZ() >= eyez-1 && asteroides[i]->GetZ() <= eyez+1) &&
              ( (asteroides[i]->GetX() >= eyex-1 && asteroides[i]->GetX() <= eyex+1) ||
                (asteroides[i]->GetX()+2 >= eyex-1 && asteroides[i]->GetX()+2 <= eyex+1))){             
                return true;
            }
        }
    }
    
    //testado colisoes com inimigos (mesma logica dos asteroides)
    for (int i = 0; i < numInimigos; i++) {
        if(inimigos[i]->IsAtivo()){
            if( (inimigos[i]->GetZ() >= eyez-1 && inimigos[i]->GetZ() <= eyez+1) &&
              ( (inimigos[i]->GetX() >= eyex-1 && inimigos[i]->GetX() <= eyex+1) ||
                (inimigos[i]->GetX()+2 >= eyex-1 && inimigos[i]->GetX()+2 <= eyex+1))){
                return true;
            }
        }
    }
    
    //testando colisoes com sobreviventes
    for (int i = 0; i < numSobreviventes; i++) {
        if(!sobreviventes[i]->IsSave()){
            if( (sobreviventes[i]->GetZ() >= eyez-1 && sobreviventes[i]->GetZ() <= eyez+1) &&
              ( (sobreviventes[i]->GetX() >= eyex-1 && sobreviventes[i]->GetX() <= eyex+1) ||
                (sobreviventes[i]->GetX()+2 >= eyex-1 && sobreviventes[i]->GetX()+2 <= eyex+1))){
                
                sobreviventes[i]->SetSave(true);
                sobreviventesSalvos++;
            }
        }
    }
    
    return false;
    
}

/*
 * dispara um tiro
 */
void atirar(){
    if(tirosDados < numTiros){
        tiros[tirosDados] = new Tiro( eyex, eyez, a);
        tirosDados++;
    }
}

/*
 * quando o tempo do contador terminar, decrementa uma vida, se as vidas terminaram, game over.
 */
void acabaTempo( int value ){

    if( flag == 0){
        //se não retira uma vida, reinicia round e reinicia contador de tempo
        vidas--;
        //exibir mensagem de fim do tempo do round e reiniciar round
        init();        
        //reinicia contador de tempo
        glutTimerFunc(60000, acabaTempo, 0);
        
    } else {
        flag--;
    }
}

/*
 * chama a movimentacao dos personagens, testes de colisao. Em caso de colisao, decrementa vidas e reinicia a jogada.
 * verifica também game over e game win
 */
void update(int value){
        
    glLoadIdentity();
    gluLookAt(eyex, eyey, eyez, dx, dy, dz, px, py, pz);
    
    movimentacao();
    
    bool colidiu = testaColisoes();
    //se colidiu, deve exibir uma mensagem e reiniciar o jogo
    if( colidiu ){
        vidas--;
        //incrementa a flag
        flag++;
        if( vidas > 0){
            init();
            //zerar contador de tempo de round
            glutTimerFunc(10000, acabaTempo, 0);
        }
    }

    glutPostRedisplay();
    
    glutTimerFunc(33,update, 0);
    
}

/*
 * redimensionamento da tela
 */
void reshape(int w, int h) {
    if (h < 1)
        h = 1;
    width = w;
    height = h;

    // reestabelece a visualização
    configView();
}

/*
 * tratamento de teclas especiais
 */
void specialKeyboard(int key, int x, int y) {
    switch (key) {
	case GLUT_KEY_LEFT: {
		//correcao angulo
		if (a == 0) {
			a = 360;
		}
		a -= 3;
		//converte para radianos
		dz = eyez + sin((a*M_PI) / 180);
		dx = eyex + cos((a*M_PI) / 180);
		break;

	}
		case GLUT_KEY_RIGHT: {
			//correcao angulo
			if (a == 360) {
				a = 0;
			}
			a += 3;
			//converte para radianos
			dz = eyez + sin((a*M_PI) / 180);
			dx = eyex + cos((a*M_PI) / 180);
			break;
		}
            
		case GLUT_KEY_UP: {
			double novoZ1 = eyez + sin((a*M_PI) / 180);
			double novoX1 = eyex + cos((a*M_PI) / 180);

			//teste para impedir a nave de sair do dominio do mundo
			if (!(novoZ1 >= maxZ || novoZ1 <= -maxZ || novoX1 >= maxX || novoX1 <= -maxX)) {
				eyez = novoZ1;
				eyex = novoX1;
				dz = eyez + sin((a*M_PI) / 180);
				dx = eyex + cos((a*M_PI) / 180);
			}
			break;

		}
		case GLUT_KEY_DOWN: {
			double novoZ2 = eyez - sin((a*M_PI) / 180);
			double novoX2 = eyex - cos((a*M_PI) / 180);

			//teste para impedir a nave de sair do dominio do mundo
			if (!(novoZ2 >= maxZ || novoZ2 <= -maxZ || novoX2 >= maxX || novoX2 <= -maxX)) {
				eyez = novoZ2;
				eyex = novoX2;
				dz = eyez + sin((a*M_PI) / 180);
				dx = eyex + cos((a*M_PI) / 180);
			}
			break;
		}
    }
}

/*
 * tratamento de teclado
 */
void keyboard(unsigned char key, int x, int y) {
	switch(key){
            case 'q':
            case 'Q':
                    exit(0);
            //barra de espaço
            case 32:
                atirar();
	}
}

int main(int argc, char** argv) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    //tamanho da janela
    glutInitWindowSize(width, height);
    
    //posição da janela
    glutInitWindowPosition(0, 0);
    
    glutCreateWindow("Resgate Espacial");
    glutFullScreen();
    init();
    
    glutDisplayFunc(display); //desenho
    glutReshapeFunc(reshape); //tratamento do redimensionamento da tela
    glutSpecialFunc(specialKeyboard); // tratamento das teclas especiais
    glutKeyboardFunc(keyboard); //tratamento do teclado
    
    //controla tempo do update
    glutTimerFunc(33, update, 0);
    //controla tempo do round (60 seg)
    glutTimerFunc(60000, acabaTempo, 0);    
    glutMainLoop();
    
    return 0;
}

