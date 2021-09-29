#version 330

uniform sampler2D textureMap0;
uniform sampler2D textureMap1;
uniform int chosen;
uniform int specular;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 ic; 
in vec4 n;
in vec4 n2;
in vec4 l;
in vec4 l2;
in vec4 v;
in vec4 v2;
in vec2 iTexCoord0;

void main(void) {

	//Znormalizowane interpolowane wektory
	vec4 ml = normalize(l);
	vec4 ml2 = normalize(l2);
	vec4 mn = normalize(n);
	vec4 mn2 = normalize(n2);
	vec4 mv = normalize(v);
	vec4 mv2 = normalize(v2);
	//Wektor odbity
	vec4 mr = reflect(-ml, mn);
	vec4 mr2 = reflect(-ml2, mn2);

	//Parametry powierzchni
	vec4 kd = texture(textureMap0, iTexCoord0);
	vec4 ks = vec4(1,1,1,1);
	vec4 ks2 = vec4(1,1,1,1);

	// Uwzglednienie tekstury odbic (jezeli zostala podana)
	if (specular == 1){
		ks = texture(textureMap1, iTexCoord0);
	}

	//Obliczenie modelu oświetlenia
	float nl = clamp(dot(mn, ml), 0, 1);
	float nl2 = clamp(dot(mn, ml2), 0, 1);
	float rv = pow(clamp(dot(mr, mv), 0, 1), 20);
	float rv2 = pow(clamp(dot(mr2, mv2), 0, 1), 20);
	pixelColor = vec4(kd.rgb * nl, kd.a) + vec4(ks.rgb*rv, 0) + vec4(kd.rgb * nl2, kd.a) + vec4(ks2.rgb*rv2, 0);

	//Podswietlanie wybranego obiektu
	if (chosen == 1){
		pixelColor = mix(pixelColor, vec4(0.070,0.119,0.912, 1.0), 1.0);
	}
}
