#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 color; //kolor związany z wierzchołkiem
in vec4 normal; //wektor normalny w przestrzeni modelu
in vec2 texCoord0;

//Zmienne interpolowane
out vec4 ic;
out vec4 l;
out vec4 l2;
out vec4 n;
out vec4 n2;
out vec4 v;
out vec4 v2;
out vec2 iTexCoord0;

void main(void) {
    vec4 lp = vec4(0, 5, 0, 1); //pozcyja światła, przestrzeń świata
    vec4 lp2 = vec4(5, 5, 5, 1);
    l = normalize(V * lp - V*M*vertex); //wektor do światła w przestrzeni oka
    l2 = normalize(V * lp2 - V*M*vertex);
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex); //wektor do obserwatora w przestrzeni oka
    v2 = normalize(vec4(0, 0, 0, 1) - V * M * vertex); //wektor do obserwatora w przestrzeni oka
    n = normalize(V * M * normal); //wektor normalny w przestrzeni oka
    n2 = normalize(V * M * normal); //wektor normalny w przestrzeni oka
    iTexCoord0 = texCoord0; 
    ic = color;

    gl_Position=P*V*M*vertex;
}
