#pragma once

namespace libEngine
{
class Vertex
{
public:
  Vertex();
  Vertex(float x, float y, float z, float r, float g, float b, float a, float nx, float ny, float nz, float tx,
         float ty);
  void SetPosition(float _x, float _y, float _z);
  void SetColor(float _r, float _g, float _b, float _a);
  void SetNormal(float _nx, float _ny, float _nz);
  void SetTextureCoord(float _tx, float _ty);

  float x;
  float y;
  float z;
  float r;
  float g;
  float b;
  float a;
  float tx;
  float ty;
  float nx;
  float ny;
  float nz;
};
}  // namespace libEngine