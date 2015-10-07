#ifndef _pyglm_wrapper_H_
#define _pyglm_wrapper_H_

//#include <wrapper/WrapHelper.h>

#include <Python.h>
#include <boost/python.hpp>
#include <glm/glm.hpp>

#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

#include <string>

using namespace boost::python;
using namespace std;


template<class VEC3, class ATTR>
struct vec3_from_python
{
    vec3_from_python()
    {
        boost::python::converter::registry::push_back(
                &convertible, &construct, boost::python::type_id<VEC3>());
    }

    static void* convertible(PyObject* obj_ptr)
    {
        if (!PySequence_Check(obj_ptr)) {
            return 0;
        }
        if (PySequence_Size(obj_ptr) != 3) {
            return 0;
        }
        if (PyString_Check(obj_ptr)) {
            return 0;
        }

        return obj_ptr;
    }

    static void construct(PyObject* obj_ptr,
            boost::python::converter::rvalue_from_python_stage1_data* data)
    {
        VEC3 t;
        PyObject * pEntry = PySequence_GetItem(obj_ptr, 0);
        t.x = (ATTR)PyFloat_AsDouble(pEntry);
        Py_DECREF(pEntry);
        pEntry = PySequence_GetItem(obj_ptr, 1);
        t.y = (ATTR)PyFloat_AsDouble(pEntry);
        Py_DECREF(pEntry);
        pEntry = PySequence_GetItem(obj_ptr, 2);
        t.z = (ATTR)PyFloat_AsDouble(pEntry);
        Py_DECREF(pEntry);
        void* storage = (
                (boost::python::converter::rvalue_from_python_storage<VEC3>*)
                        data)->storage.bytes;
        new (storage) VEC3(t);
        data->convertible = storage;
    }
};


namespace Vec3Helper
{
    int len(const glm::vec3&) 
    {
        return 3;
    }

    float getX(const glm::vec3& pt)
    {
        return pt.x;
    }

    float getY(const glm::vec3& pt)
    {
        return pt.y;
    }

    float getZ(const glm::vec3& pt)
    {
        return pt.z;
    }

    void setX(glm::vec3& pt, float val)
    {
        pt.x = val;
    }

    void setY(glm::vec3& pt, float val)
    {
        pt.y = val;
    }

    void setZ(glm::vec3& pt, float val)
    {
        pt.z = val;
    }

    void checkItemRange(int i) {
        if (i < 0 || i > 2) {
            throw std::out_of_range("Index out of range for vec3. Must be [0..2].");
        }
    }

    float getItem(const glm::vec3& pt, int i)
    {
        checkItemRange(i);
        switch (i) {
            case 0:
                return pt.x;
            case 1:
                return pt.y;
            case 2:
                return pt.z;
            default:
                assert(false);
                return 0;
        }
    }

    void setItem(glm::vec3& pt, int i, float val)
    {
        checkItemRange(i);
        switch (i) {
            case 0:
                pt.x = val;
            case 1:
                pt.y = val;
            case 2:
                pt.z = val;
            default:
                assert(false);
        }
    }

    string str(const glm::vec3& pt)
    {
        stringstream st;
        st << "(" << pt.x << "," << pt.y << "," << pt.z << ")";
        return st.str();
    }

    string repr(const glm::vec3& pt)
    {
        stringstream st;
        st << "pyglm.vec3(" << pt.x << "," << pt.y << "," << pt.z << ")";
        return st.str();
    }

    long getHash(const glm::vec3& pt)
    {
        // Wild guess at what could constitute a good hash function.
        // Will generate very bad hashes if most values are in a range < 0.1,
        // but this is meant for pixel values anyway, right? ;-).
        return long(pt.x*42+pt.y*23+pt.z*17);
    }
    
    glm::vec3 safeGetNormalized(const glm::vec3& pt)
    {
        if (pt.x==0 && pt.y==0 && pt.z==0) {
            throw range_error("Can't normalize (0,0,0).");
        } else {
            float invNorm = 1/sqrt(pt.x*pt.x+pt.y*pt.y+pt.z*pt.z);
            return glm::vec3(pt.x*invNorm, pt.y*invNorm, pt.z*invNorm);
        }
    }

    float getNorm(const glm::vec3& pt)
    {
        return glm::length(pt);
    }

    float dot(const glm::vec3& pt1, const glm::vec3& pt2)
    {
        return glm::dot(pt1, pt2);
    }
}

glm::vec3* createVec3()
{
    return new glm::vec3(0,0,0);
}

namespace QuatHelper
{
    int len(const glm::quat&) 
    {
        return 4;
    }

    float getX(const glm::quat& q)
    {
        return q.x;
    }

    float getY(const glm::quat& q)
    {
        return q.y;
    }

    float getZ(const glm::quat& q)
    {
        return q.z;
    }

    float getW(const glm::quat& q)
    {
        return q.w;
    }

    void setX(glm::quat& q, float val)
    {
        q.x = val;
    }

    void setY(glm::quat& q, float val)
    {
        q.y = val;
    }

    void setZ(glm::quat& q, float val)
    {
        q.z = val;
    }

    void setW(glm::quat& q, float val)
    {
        q.w = val;
    }

    glm::vec3 getEuler(const glm::quat& q)
    {
        return glm::eulerAngles(q);
    }

    glm::quat getInverse(const glm::quat& q)
    {
        return glm::inverse(q);
    }

    glm::quat slerp(const glm::quat& q1, const glm::quat& q2, float part)
    {
        return glm::slerp(q1, q2, part);
    }

    bool almostEqual(const glm::quat& q1, const glm::quat& q2, float epsilon=0.00001)
    {
        return (fabs(q1.w-q2.w) < epsilon) && (fabs(q1.x-q2.x) < epsilon) &&
                (fabs(q1.y-q2.y) < epsilon) && (fabs(q1.z-q2.z) < epsilon);
    }

    string str(const glm::quat& q)
    {
        stringstream st;
        st << "(" << q.w << "," << q.x << "," << q.y << "," << q.z << ")";
        return st.str();
    }

    string repr(const glm::quat& q)
    {
        stringstream st;
        st << "pyglm.quat(" << q.w << "," << q.x << "," << q.y << "," << q.z << ")";
        return st.str();
    }

    void checkItemRange(int i) {
        if (i < 0 || i > 3) {
            throw std::out_of_range("Index out of range for quat. Must be [0..3].");
        }
    }

    float getItem(const glm::quat& q, int i)
    {
        checkItemRange(i);
        switch (i) {
            case 0:
                return q.w;
            case 1:
                return q.x;
            case 2:
                return q.y;
            case 3:
                return q.z;
            default:
                assert(false);
                return 0;
        }
    }

    void setItem(glm::quat& q, int i, float val)
    {
        checkItemRange(i);
        switch (i) {
            case 0:
                q.w = val;
            case 1:
                q.x = val;
            case 2:
                q.y = val;
            case 3:
                q.z = val;
            default:
                assert(false);
        }
    }

}

glm::quat* createQuat()
{
    return new glm::quat(0,0,0,0);
}

BOOST_PYTHON_FUNCTION_OVERLOADS(quat_almostEqual_overloads, QuatHelper::almostEqual, 2, 3);

BOOST_PYTHON_MODULE(pyglm)
{
    vec3_from_python<glm::vec3, float>();

    class_<glm::vec3>("vec3", no_init)
        .def("__init__", make_constructor(createVec3))
        .def(init<float, float, float>())
        .def(init<const glm::vec3&>())
        .def("__setitem__", &Vec3Helper::setItem)
        .add_property("x", &Vec3Helper::getX, &Vec3Helper::setX,"")
        .add_property("y", &Vec3Helper::getY, &Vec3Helper::setY,"")
        .add_property("z", &Vec3Helper::getZ, &Vec3Helper::setZ,"")
        .def("__len__", &Vec3Helper::len)
        .def("__getitem__", &Vec3Helper::getItem)
        .def("__str__", &Vec3Helper::str)
        .def("__repr__", &Vec3Helper::repr)
        .def("__hash__", &Vec3Helper::getHash)
        .def("getNormalized", &Vec3Helper::safeGetNormalized)
        .def("getNorm", &Vec3Helper::getNorm)
//        .def("getRotated", &getRotated)
//        .def("getRotated", &getRotatedPivot)
        .def(self == self)
        .def(self != self)
        .def(-self)
        .def(self + self)
        .def(self - self)
        .def(float() * self)
        .def(self * float())
        .def(self / float())
        .def("dot", &Vec3Helper::dot)
        .staticmethod("dot")
    ;    
    
    class_<glm::quat>("quat", no_init)
        .def("__init__", make_constructor(createQuat))
        .def(init<float, float, float, float>())
        .def(init<const glm::vec3&>())
        .def("__setitem__", &QuatHelper::setItem)
        .add_property("x", &QuatHelper::getX, &QuatHelper::setX,"")
        .add_property("y", &QuatHelper::getY, &QuatHelper::setY,"")
        .add_property("z", &QuatHelper::getZ, &QuatHelper::setZ,"")
        .add_property("w", &QuatHelper::getW, &QuatHelper::setW,"")
        .def("__len__", &QuatHelper::len)
        .def("__getitem__", &QuatHelper::getItem)
        .def("__str__", &QuatHelper::str)
        .def("__repr__", &QuatHelper::repr)
        .def("toEuler", &QuatHelper::getEuler)
        .def("getInverse", &QuatHelper::getInverse)
        .def("slerp", &QuatHelper::slerp)
        .staticmethod("slerp")
        .def("almostEqual", &QuatHelper::almostEqual, quat_almostEqual_overloads())
        .staticmethod("almostEqual")
        .def(self * glm::vec3())
    ;
}

#endif
