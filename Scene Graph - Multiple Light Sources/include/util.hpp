#ifndef UTILITIES_HPP
#define UTILITIES_HPP


#include<iostream>
#include <math.h>
#include <fstream>
#include <vector>
#include <cmath>
#define PI 3.14159265358979323846


class Vector
{
public:
	float x;
	float y;
	float z;
	float length;
	Vector crossProduct(Vector v);
	void normalize();
	void calculateLength();
	Vector(float x1,float y1,float z1);
	
};

class Quaternion{
	public:	
		Quaternion();
		~Quaternion();
		void CreateMatrix(float *pMatrix);
		void CreateFromAxisAngle(const float &in_x,
		 const float &in_y,
		 const float &in_z,
		 const float &in_degrees);
		Quaternion operator *(const Quaternion &q);
	
		float x, y, z, w;
};


Quaternion::Quaternion(){
	x = y = z = 0.0f;
	w = 1.0f;
	}

Quaternion::~Quaternion(){

	}

void Quaternion::CreateFromAxisAngle(const float &in_x, const float &in_y, const float &in_z, const float &in_degrees){

	float angle = float((in_degrees / 180.0f) * PI);
	float result = float(sin(angle/2.0f));
	w = float(cos(angle/2.0f));

	// Calculate the x, y and z of the quaternion
	x = float(in_x * result);
	y = float(in_y * result);
	z = float(in_z * result);
	}

void Quaternion::CreateMatrix(float *pMatrix){
	if(pMatrix){
		// First row
	    pMatrix[ 0] = 1.0f - 2.0f * ( y * y + z * z );
	    pMatrix[ 1] = 2.0f * ( x * y - w * z );
	    pMatrix[ 2] = 2.0f * ( x * z + w * y );
	    pMatrix[ 3] = 0.0f;

	    // Second row
	    pMatrix[ 4] = 2.0f * ( x * y + w * z );
	    pMatrix[ 5] = 1.0f - 2.0f * ( x * x + z * z );
	    pMatrix[ 6] = 2.0f * ( y * z - w * x );
	    pMatrix[ 7] = 0.0f;

	    // Third row
	    pMatrix[ 8] = 2.0f * ( x * z - w * y );
	    pMatrix[ 9] = 2.0f * ( y * z + w * x );
	    pMatrix[10] = 1.0f - 2.0f * ( x * x + y * y );
	    pMatrix[11] = 0.0f;

	    // Fourth row
	    pMatrix[12] = 0;
	    pMatrix[13] = 0;
	    pMatrix[14] = 0;
	    pMatrix[15] = 1.0f;
		}
	}

Quaternion Quaternion::operator *(const Quaternion &q){
	Quaternion r;

	r.w = w*q.w - x*q.x - y*q.y - z*q.z;
	r.x = w*q.x + x*q.w + y*q.z - z*q.y;
	r.y = w*q.y + y*q.w + z*q.x - x*q.z;
	r.z = w*q.z + z*q.w + x*q.y - y*q.x;

	return r;
	}

Vector::Vector(float x1, float y1, float z1)
{
	x=x1;
	y=y1;
	z=z1;
	calculateLength();
}


void Vector::normalize()
{
	x = x/length;
	y = y/length;
	z = z/length;
	calculateLength();
}

void Vector::calculateLength()
{
	length = sqrt(x*x + y*y +z*z);
}


Vector Vector::crossProduct(Vector v)
{
	Vector v1( (y * v.z) - (z * v.y), (z * v.x) - (x * v.z), (x * v.y) - (y * v.x));
	v1.normalize();
	return v1;
}
#endif