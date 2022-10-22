#pragma once

#include "Barrier.h"
#include "Scene.h"

namespace pr {

int findClosestInter(const Scene& scene, const Rayon& ray);
Color computeColor(const Sphere& obj, const Rayon& ray, const Vec3D& camera,
                   std::vector<Vec3D>& lights);

class Job {
 public:
  virtual void run() = 0;
  virtual ~Job(){};
};

class PixelJob : public Job {
  int x;
  const Scene::screen_t& screen;
  Scene* scene;
  std::vector<Vec3D> lights;
  Color* pixels;
  Barrier& b;
  void calcul(int x, const Scene::screen_t& screen, Scene& scene,
              std::vector<Vec3D>& lights, Color* pixels);

 public:
  PixelJob(int x, const Scene::screen_t& screen, Scene* scene,
           std::vector<Vec3D> lights, Color* pixels, Barrier& b);
  ~PixelJob();
  void run();
};

}  // namespace pr
