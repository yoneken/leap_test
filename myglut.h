#ifndef __MY_GLUT_H__
#define __MY_GLUT_H__

#include <Eigen/Dense>
#include <vector>
#include <deque>

typedef std::vector<Eigen::Vector3f, Eigen::aligned_allocator<Eigen::Vector3f> > Vector3fv;
typedef std::deque<Eigen::Vector3f, Eigen::aligned_allocator<Eigen::Vector3f> > Vector3fd;
typedef std::vector<Eigen::Matrix3f, Eigen::aligned_allocator<Eigen::Matrix3f> > Matrix3fv;
typedef std::deque<Eigen::Matrix3f, Eigen::aligned_allocator<Eigen::Matrix3f> > Matrix3fd;

void glPrint(float x, float y, void *font, char *string);
void glDrawSpherer(float x, float y, float z, float radius);
void glDrawSpherer(Eigen::Vector3f, float);
void glDrawPiped(double x0, double y0, double z0, double x1, double y1, double z1, double r);
void glDrawPiped(Eigen::Vector3d start, Eigen::Vector3d finish, double thickness);
void glDrawPipedr(Eigen::Vector3d start, Eigen::Vector3d finish, double thickness);
void glDrawArrowd(double x0, double y0, double z0, double x1, double y1, double z1);
void glDrawArrowd(Eigen::Vector3d start, Eigen::Vector3d finish);
void glDrawArrowdr(Eigen::Vector3d start, Eigen::Vector3d finish);
void glDrawAxis(float length, void *font);
void glDrawAxisr(float length, void *font);
void glDrawMatrix3fr(Eigen::Matrix3f, Eigen::Vector3f);
void glDrawMatrix3fr(Eigen::Matrix3f);

void gluLookAtr(float , float, float, float, float, float, float, float, float);

#endif /* __MYGLUT_H__ */
