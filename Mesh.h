class Mesh{
protected:
	vec3 position;
	GLfloat height;
	vec3 color;
	char type; //W:wall, O:obstacle1, H:hole
	
public:	GLfloat width;

	Mesh(vec3 p, GLfloat h, GLfloat w, vec3 c,char t) 
		: position(p),height(h),width(w),color(c),type(t) {}

	/*
		Vertex  Triangle    Face
		------+-----------+-----
		0 0 0
		0 1 0
		1 0 0  000 010 100  **0
		1 1 0  100 010 110  **0
		1 1 1  100 110 111  1**
		0 1 0  111 110 010  *1*
		0 1 1  111 010 011  *1*
		0 0 1  011 010 001  0**
		1 1 1  011 001 111  **1
		1 0 1  111 001 101  **1
		1 0 0  111 101 100  1**
		0 0 1  100 101 001  *0*
		0 0 0  100 001 000  *0*
		0 1 0  000 001 010  0**
	*/

	void draw(){	
		GLfloat iX,iY,iZ,fX,fY,fZ;
		iX = position.x-(width/2);
		iZ = position.z-(height/2);
		fX = position.x+(width/2);
		fZ = position.z+(height/2);
		iY = 0;
		fY = 1;

		switch (type){       	
	        case 'W': 
	        case 'H':
	        case 'O':
	        	glColor3f(color.x,color.y,color.z);
				glBegin(GL_TRIANGLE_STRIP);
					// a,b,c,d,e,b,f,g,e,h,c,g,a,b
					glVertex3f(iX, iY, iZ);
					glVertex3f(iX, fY, iZ);
					glVertex3f(fX, iY, iZ);
					glVertex3f(fX, fY, iZ);
					glVertex3f(fX, fY, fZ);
					glVertex3f(iX, fY, iZ);
					glVertex3f(iX, fY, fZ);
					glVertex3f(iX, iY, fZ);
					glVertex3f(fX, fY, fZ);
					glVertex3f(fX, iY, fZ);
					glVertex3f(fX, iY, iZ);
					glVertex3f(iX, iY, fZ);
					glVertex3f(iX, iY, iZ);
					glVertex3f(iX, fY, iZ);
				glEnd();
	        break;	
	        default: break;
	    } 
	}	

	list<vector<vec3>> getTriangles(){
		GLfloat iX,iY,iZ,fX,fY,fZ;
		iX = position.x-(width/2);
		iZ = position.z-(height/2);
		fX = position.x+(width/2);
		fZ = position.z+(height/2);
		iY = 0;
		fY = 1;

		/* POINTS */
		vec3 a = {iX, fY, iZ};
		vec3 b = {iX, iY, iZ};
		vec3 c = {fX, fY, iZ};
		vec3 d = {fX, iY, iZ};
		vec3 e = {fX, iY, fZ};
		vec3 f = {iX, iY, fZ};
		vec3 g = {iX, fY, fZ};
		vec3 h = {fX, fY, fZ};

		/* TRIANGLES */
		return list<vector<vec3>> { 
			  vector<vec3> {a, b, c}
			, vector<vec3> {c, b, d}
			, vector<vec3> {c, d, e}
			, vector<vec3> {e, d, b}
			, vector<vec3> {e, b, f}
			, vector<vec3> {f, b, g}
			, vector<vec3> {f, g, e}
			, vector<vec3> {e, g, h}
			, vector<vec3> {e, h, c}
			, vector<vec3> {c, h, g}
			, vector<vec3> {c, g, a}
			, vector<vec3> {a, g, b} };
	}
};