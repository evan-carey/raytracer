#ifndef CSE168_SCENE_H_INCLUDED
#define CSE168_SCENE_H_INCLUDED

#include "Miro.h"
#include "Object.h"
#include "PointLight.h"
#include "BVH.h"
#include "PhotonMap.h"


class Camera;
class Image;

class Scene
{
public:
	Scene(); 
    void addObject(Object* pObj)        {m_objects.push_back(pObj);}
    const Objects* objects() const      {return &m_objects;}

    void addLight(PointLight* pObj)     {m_lights.push_back(pObj);}
    const Lights* lights() const        {return &m_lights;}

    void preCalc();
    void openGL(Camera *cam);

    void raytraceImage(Camera *cam, Image *img);
    bool trace(HitInfo& minHit, const Ray& ray,
               float tMin = 0.0f, float tMax = MIRO_TMAX) const;

	// recursive trace to include reflection and refraction
	bool trace(const Ray& ray, int numCalls, Vector3& res);


	void calcPhotonMap(Photon_map& map, bool isCaustic);


	int tracePhoton(const Vector3& flux, const Vector3& origin, const Vector3& dir, int numCalls, bool isCaustic);

protected:
    Objects m_objects;
    BVH m_bvh;
    Lights m_lights;

	// Photom mapping
	Photon_map m_photonMap;
	// Caustics photon mapping
	Photon_map m_causticMap;

};

extern Scene * g_scene;

#endif // CSE168_SCENE_H_INCLUDED
