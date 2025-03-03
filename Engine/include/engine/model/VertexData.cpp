#include "engine/model/VertexData.h"

VertexData::VertexData()
	: x(0.f), y(0.f), z(0.f), r(0.f), g(0.f), b(0.f), nx(0.f), ny(0.f), nz(0.f), tx(0.f), ty(0.f), a(1.0f)
{
}
VertexData::VertexData(float x, float y, float z, float r, float g, float b, float a, float nx, float ny, float nz,
	float tx, float ty)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
	this->nx = nx;
	this->ny = ny;
	this->nz = nz;
	this->tx = tx;
	this->ty = ty;
}
VertexData::VertexData(Vec3 pos, float r, float g, float b, float a, float nx, float ny, float nz, float tx, float ty)
{
	this->x = pos.x();
	this->y = pos.y();
	this->z = pos.z();
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
	this->nx = nx;
	this->ny = ny;
	this->nz = nz;
	this->tx = tx;
	this->ty = ty;
}
void VertexData::SetPosition(float _x, float _y, float _z)
{
	this->x = _x;
	this->y = _y;
	this->z = _z;
}
void VertexData::SetColor(float _r, float _g, float _b, float _a)
{
	this->r = _r;
	this->g = _g;
	this->b = _b;
	this->a = _a;
}
void VertexData::SetNormal(float _nx, float _ny, float _nz)
{
	this->nx = _nx;
	this->ny = _ny;
	this->nz = _nz;
}
void VertexData::SetTextureCoord(float _tx, float _ty)
{
	this->tx = _tx;
	this->ty = _ty;
}
