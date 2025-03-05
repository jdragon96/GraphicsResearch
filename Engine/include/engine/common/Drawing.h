#pragma once

#include "engine/model/VertexData.h"
#include "engine/model/MeshData.h"
#include "engine/Macro.h"

MeshData<VertexData> MakeCube(float size, float r, float g, float b, bool flipTexture);

MeshData<VertexData> MakeCube(Vec3 mmax, Vec3 mmin);

MeshData<VertexData> MakeSphere(Vec3 color, float radius, int numSlice, int numStack);

MeshData<VertexData> MakeSquare(float scale);
