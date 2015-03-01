#include <vector>
// #include "material.h"

typedef struct particle {
	float pos[3]; //position coordinates
	float dir[3]; //direction
	float clr[3]; //colour
	// material clr_mat;
	float ang[3]; //rotation angle
	float speed; //particle speed
	float size; //particle size
	int age;
} particle;

typedef std::vector<particle>::iterator vIt; //shorten the vector iterator typename

class particles {
public:
	particles(float origX, float origY, float origZ);
	std::vector<particle> parts;
	float origin[3];
	void update(float g, float windX, float windZ);
	void add(float p[3], float d[3], float a[3], float c[3], float s, float sz) ;
};