#include "stdio.h"
#include "math.h"

const float pi = 3.141592;

float calculaVelocidade(float D, float Q);
void calculaVazaoTeorica();
float calculaHc(float p, float v, float D, float u, float e, float g, float L, float Ks);

int main(){
    calculaVazaoTeorica();
    return 0;
}

float calculaVelocidade(float D, float Q){
    float r = D / 2;
    float A = pi * r * r;
    float v2 = Q / A;
    return v2;
}

void calculaVazaoTeorica(){
    // Constantes
    float z2 = -0.49; // altura em 2 - referencial: ponto 1 (m)
    float g = 9.81; // gravidade (m/s²)
    float p = 997.5; // densidade (kg/m³)
    float u = 0.0009321; // viscosidade dinâmica (Pa.s)
    float e = 0.0000015; // rugosidade absoluta (m)
    float L = 1.63; // comprimento da tubulação (m) -> 11 + 92 + 60    
    float Ks = 2 * 0.9 + 0.5 + 1; // duas curvas de 90º, uma entrada em canalização e entrada de borda

    // Preencher
    float Q = 0.0002632; // vazão (m3/s) -> vazao para 1L
    float D = 0.01905; // diâmetro (m)
    float v2 = calculaVelocidade(D, Q);
    printf("Valor experimental de v2 = %f\n", v2);

    float adicaoVelocidade = v2* 0.01;

    for (int i = 0; i < 1000; i++){
        float hc = calculaHc(p, v2, D, u, e, g, L, Ks);
        float bernoulli = (v2*v2) / (2*g) + z2 + hc;
        
        printf("Valor da bernoulli = %.10f\n", bernoulli);
        
        if (bernoulli > -0.00001 && bernoulli < 0.00001){
            printf("Valor de v2 = %f esta proximo\n", v2);
            printf("Numero de iteracoes = %d, Ultima adicao de velocidade = %f\n", i, adicaoVelocidade);
            float Qteorica = v2 * pi * pow(D, 2) / 4;
            printf("Vazao Volumetrica Teorica: %f", Qteorica);
            break;
        }

        if (i < 100){
            adicaoVelocidade = v2 * 0.01;
        }
        else if(i < 250){
            adicaoVelocidade = v2 * 0.001;
        }
        else if(i < 500){
            adicaoVelocidade = v2 * 0.0001;
        }
        else if(i < 750){
            adicaoVelocidade = v2 * 0.00001;
        }
        else{
            adicaoVelocidade = v2 * 0.000001;
        }

        if(bernoulli < 0){
            v2 += adicaoVelocidade;
            printf("Novo v2: %f\n", v2);
        }
        else {
            v2 -= adicaoVelocidade;
            printf("Novo v2: %f\n", v2);
        }
    }
}

float calculaHc(float p, float v, float D, float u, float e, float g, float L, float Ks){
    float Re = (p * v * D) / u;
    float f, hcd, hcl;

    f = 0.25 / (pow(log10( ( (e / D) / 3.7 ) + ( 5.74 / pow(Re, 0.9) )), 2));
    printf("f: %f\n", f);
    hcd = (f * L * pow(v, 2)) / (D * 2 * g); // perda de carga distribuída
    hcl = Ks * pow(v, 2) / (2 * g); // perda de carga localizada
    printf("hc: %.10f\n", hcd+hcl);
    return hcd + hcl;
}
