struct Collision{
	vec3 position;
	vec3 normal;
};

class CollisionDetector{
protected:
	list<Mesh*> &meshs;

public:
	CollisionDetector(list<Mesh*> &ms) : meshs(ms) {}

	bool getCollision(vec3 position, vec3 direction, Collision &collision, GLfloat raySize){
		vec3 colPosition, colNormal;

		for (list<Mesh*>::iterator m=meshs.begin(); m != meshs.end(); ++m){
			list<vector<vec3>> triangles = (*m)->getTriangles();
			vec3 intersect;
			vec3 p1,p2,p3;

			for (list<vector<vec3>>::iterator t=triangles.begin(); t != triangles.end(); ++t){
				//los pongo asi para invertir la normal
				p1 = (*t).at(2);
				p2 = (*t).at(1);
				p3 = (*t).at(0);

				if(glm::intersectRayTriangle(position,direction,p1,p2,p3,intersect)){
					float z = 1.0 - intersect.x - intersect.y;
					collision.position = p1 * z + p2 * intersect.x + p3 * intersect.y;

					if(glm::distance(collision.position,position) > raySize) continue;

					collision.normal = glm::normalize(glm::cross(p2 - p1,p3 - p1));
					//cout<<"-- p1 "<<p1.x<<","<<p1.y<<","<<p1.z<<endl;
					//cout<<"-- p2 "<<p2.x<<","<<p2.y<<","<<p2.z<<endl;
					//cout<<"-- p3 "<<p3.x<<","<<p3.y<<","<<p3.z<<endl;
					return true;
				}
			}
		}

		return false;
	}
};