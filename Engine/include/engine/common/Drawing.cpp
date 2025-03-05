#include "engine/common/Drawing.h"

MeshData<VertexData> MakeCube(float size, float r, float g, float b, bool flipTexture)
{
  MeshData<VertexData> mesh;

  mesh.vertices.push_back(VertexData(-size, size, -size, r, g, b, 1.f, 0.f, 1.f, 0.f, 0.f, 0.f));
  mesh.vertices.push_back(VertexData(-size, size, size, r, g, b, 1.f, 0.f, 1.f, 0.f, 1.f, 0.f));
  mesh.vertices.push_back(VertexData(size, size, size, r, g, b, 1.f, 0.f, 1.f, 0.f, 1.f, 1.f));
  mesh.vertices.push_back(VertexData(size, size, -size, r, g, b, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f));

  mesh.vertices.push_back(VertexData(-size, -size, -size, r, g, b, 1.f, 0.f, -1.f, 0.f, 0.f, 0.f));
  mesh.vertices.push_back(VertexData(size, -size, -size, r, g, b, 1.f, 0.f, -1.f, 0.f, 1.f, 0.f));
  mesh.vertices.push_back(VertexData(size, -size, size, r, g, b, 1.f, 0.f, -1.f, 0.f, 1.f, 1.f));
  mesh.vertices.push_back(VertexData(-size, -size, size, r, g, b, 1.f, 0.f, -1.f, 0.f, 0.f, 1.f));

  mesh.vertices.push_back(VertexData(-size, -size, -size, r, g, b, 1.f, 0.f, 0.f, -1.f, 0.f, 0.f));
  mesh.vertices.push_back(VertexData(-size, size, -size, r, g, b, 1.f, 0.f, 0.f, -1.f, 1.f, 0.f));
  mesh.vertices.push_back(VertexData(size, size, -size, r, g, b, 1.f, 0.f, 0.f, -1.f, 1.f, 1.f));
  mesh.vertices.push_back(VertexData(size, -size, -size, r, g, b, 1.f, 0.f, 0.f, -1.f, 0.f, 1.f));

  mesh.vertices.push_back(VertexData(-size, -size, size, r, g, b, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f));
  mesh.vertices.push_back(VertexData(size, -size, size, r, g, b, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f));
  mesh.vertices.push_back(VertexData(size, size, size, r, g, b, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f));
  mesh.vertices.push_back(VertexData(-size, size, size, r, g, b, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f));

  mesh.vertices.push_back(VertexData(-size, -size, size, r, g, b, 1.f, -1.f, 0.f, 0.f, 1.f, 0.f));
  mesh.vertices.push_back(VertexData(-size, size, size, r, g, b, 1.f, -1.f, 0.f, 0.f, 1.f, 0.f));
  mesh.vertices.push_back(VertexData(-size, size, -size, r, g, b, 1.f, -1.f, 0.f, 0.f, 1.f, 1.f));
  mesh.vertices.push_back(VertexData(-size, -size, -size, r, g, b, 1.f, -1.f, 0.f, 0.f, 1.f, 1.f));

  mesh.vertices.push_back(VertexData(size, -size, size, r, g, b, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f));
  mesh.vertices.push_back(VertexData(size, -size, -size, r, g, b, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f));
  mesh.vertices.push_back(VertexData(size, size, -size, r, g, b, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f));
  mesh.vertices.push_back(VertexData(size, size, size, r, g, b, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f));

  if (flipTexture)
  {
    for (auto& vtx : mesh.vertices)
    {
      vtx.ty = 1.f - vtx.ty;
    }
  }

  mesh.indices = {
    0,  1,  2,  0,  2,  3,   // À­¸é
    4,  5,  6,  4,  6,  7,   // ¾Æ·§¸é
    8,  9,  10, 8,  10, 11,  // ¾Õ¸é
    12, 13, 14, 12, 14, 15,  // µÞ¸é
    16, 17, 18, 16, 18, 19,  // ¿ÞÂÊ
    20, 21, 22, 20, 22, 23   // ¿À¸¥ÂÊ
  };

  return mesh;
}

MeshData<VertexData> MakeCube(Vec3 mmax, Vec3 mmin)
{
  MeshData<VertexData> mesh;
  float                r = 0.f;
  float                g = 1.f;
  float                b = 1.f;
  float                a = 0.1f;

  Vec3 u1 = mmax;
  Vec3 u2 = mmax - (mmax - Vec3(mmin.x(), mmax.y(), mmax.z()));
  Vec3 u3 = mmax - (mmax - Vec3(mmin.x(), mmin.y(), mmax.z()));
  Vec3 u4 = mmax - (mmax - Vec3(mmax.x(), mmin.y(), mmax.z()));
  Vec3 d1 = mmax - (mmax - Vec3(mmax.x(), mmax.y(), mmin.z()));
  Vec3 d2 = mmax - (mmax - Vec3(mmin.x(), mmax.y(), mmin.z()));
  Vec3 d3 = mmax - (mmax - Vec3(mmin.x(), mmin.y(), mmin.z()));
  Vec3 d4 = mmax - (mmax - Vec3(mmax.x(), mmin.y(), mmin.z()));

  {
    mesh.vertices.push_back(VertexData(u1, r, g, b, a, 0.f, 1.f, 0.f, 0.f, 0.f));
    mesh.vertices.push_back(VertexData(u2, r, g, b, a, 0.f, 1.f, 0.f, 0.f, 0.f));
    mesh.vertices.push_back(VertexData(u3, r, g, b, a, 0.f, 1.f, 0.f, 0.f, 0.f));
    mesh.vertices.push_back(VertexData(u4, r, g, b, a, 0.f, 1.f, 0.f, 0.f, 0.f));
  }
  {
    mesh.vertices.push_back(VertexData(d1, r, g, b, a, 0.f, 1.f, 0.f, 0.f, 0.f));
    mesh.vertices.push_back(VertexData(d2, r, g, b, a, 0.f, 1.f, 0.f, 0.f, 0.f));
    mesh.vertices.push_back(VertexData(d3, r, g, b, a, 0.f, 1.f, 0.f, 0.f, 0.f));
    mesh.vertices.push_back(VertexData(d4, r, g, b, a, 0.f, 1.f, 0.f, 0.f, 0.f));
  }

  mesh.indices = { 0, 1, 2, 0, 2, 3, 0, 4, 1, 1, 4, 5, 2, 1, 5, 5, 6, 2,
                   6, 3, 2, 6, 7, 3, 0, 3, 4, 3, 7, 4, 5, 4, 6, 6, 4, 7 };

  return mesh;
}

MeshData<VertexData> MakeSphere(Vec3 color, float radius, int numSlice, int numStack)
{
  const float          dTheta = -(3.141592 * 2) / float(numSlice);
  const float          dPhi   = -3.141592 / float(numStack);
  MeshData<VertexData> object;

  for (int i = 0; i <= numStack; ++i)
  {
    VertexData stackStartPoint;
    stackStartPoint.x = (-radius) * (-sin(dPhi * i));
    stackStartPoint.y = (-radius) * cos(dPhi * i);
    stackStartPoint.z = 0;

    for (int j = 0; j <= numSlice; ++j)
    {
      VertexData vtx;
      vtx.x         = stackStartPoint.x * cos(dTheta * j);
      vtx.y         = stackStartPoint.y;
      vtx.z         = stackStartPoint.x * (-sin(dTheta * j));
      float nFactor = sqrt(pow(vtx.x, 2) + pow(vtx.y, 2) + pow(vtx.z, 2));
      vtx.nx        = vtx.x / nFactor;
      vtx.ny        = vtx.y / nFactor;
      vtx.nz        = vtx.z / nFactor;
      vtx.tx        = float(j) / numSlice;
      vtx.ty        = 1.0f - float(i) / numStack;
      vtx.SetColor(color.x(), color.y(), color.z(), 1.f);
      object.vertices.push_back(vtx);
    }
  }

  for (int j = 0; j < numStack; j++)
  {
    const int offset = (numSlice + 1) * j;
    for (int i = 0; i < numSlice; i++)
    {
      object.indices.push_back(offset + i);
      object.indices.push_back(offset + i + numSlice + 1);
      object.indices.push_back(offset + i + 1 + numSlice + 1);

      object.indices.push_back(offset + i);
      object.indices.push_back(offset + i + 1 + numSlice + 1);
      object.indices.push_back(offset + i + 1);
    }
  }

  return object;
}

MeshData<VertexData> MakeSquare(float scale)
{
  std::vector<Vec3> positions;
  std::vector<Vec3> colors;
  std::vector<Vec3> normals;
  std::vector<Vec2> texcoords;  // ÅØ½ºÃç ÁÂÇ¥

  // ¾Õ¸é
  positions.push_back(Vec3(-1.0f, 1.0f, 0.0f) * scale);
  positions.push_back(Vec3(1.0f, 1.0f, 0.0f) * scale);
  positions.push_back(Vec3(1.0f, -1.0f, 0.0f) * scale);
  positions.push_back(Vec3(-1.0f, -1.0f, 0.0f) * scale);
  colors.push_back(Vec3(0.0f, 0.0f, 1.0f));
  colors.push_back(Vec3(0.0f, 0.0f, 1.0f));
  colors.push_back(Vec3(0.0f, 0.0f, 1.0f));
  colors.push_back(Vec3(0.0f, 0.0f, 1.0f));
  normals.push_back(Vec3(0.0f, 0.0f, -1.0f));
  normals.push_back(Vec3(0.0f, 0.0f, -1.0f));
  normals.push_back(Vec3(0.0f, 0.0f, -1.0f));
  normals.push_back(Vec3(0.0f, 0.0f, -1.0f));

  // Texture Coordinates (Direct3D 9)
  // https://learn.microsoft.com/en-us/windows/win32/direct3d9/texture-coordinates
  texcoords.push_back(Vec2(0.0f, 0.0f));
  texcoords.push_back(Vec2(1.0f, 0.0f));
  texcoords.push_back(Vec2(1.0f, 1.0f));
  texcoords.push_back(Vec2(0.0f, 1.0f));

  MeshData<VertexData> meshData;

  for (size_t i = 0; i < positions.size(); i++)
  {
    VertexData v;
    v.SetPosition(positions[i].x(), positions[i].y(), positions[i].z());
    v.SetColor(colors[i].x(), colors[i].y(), colors[i].z(), 1);
    v.SetNormal(normals[i].x(), normals[i].y(), normals[i].z());
    v.SetTextureCoord(texcoords[i].x(), texcoords[i].y());
    meshData.vertices.push_back(v);
  }
  meshData.indices = {
    0, 1, 2, 0, 2, 3,
  };

  return meshData;
}