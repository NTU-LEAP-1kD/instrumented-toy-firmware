#include <cmath>
#include "imu.h"
// heading, roll, pitch in degrees

const double RAD2DEG = (180.0 / M_PI);
const double DEG2RAD = (M_PI / 180.0);

Vector3D toEuler( double w, double x, double y, double z)
{
    Vector3D ret;
    double sqw = w*w;
    double sqx = x*x;
    double sqy = y*y;
    double sqz = z*z;

    ret.x = atan2(2.0*(x*y+z*w),(sqx-sqy-sqz+sqw)) * RAD2DEG;
    ret.z = atan2(2.0*(y*z+x*w),(-sqx-sqy+sqz+sqw)) * RAD2DEG;
    double siny = -2.0*(x*z-y*w)/(sqx+sqy+sqz+sqw);

    // prevent NaN and use 90 deg when out of range
    if (fabs(siny) >= 1.0)
        ret.y = copysign(90.0, siny);
    else
        ret.y = asin(siny) * RAD2DEG;

    return ret;
}
