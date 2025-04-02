#pragma once

#include <vector>
#include <chrono>
#include <numeric>
#include <random>

#include "engine/dx11/Dx11EngineManager.h"
#include "engine/dx11/Dx11GraphicsPSO.h"
#include "engine/dx11/Dx11MeshBuffer.h"
#include "engine/dx11/Dx11ConstantBuffer.h"
#include "engine/dx11/Dx11Filter.h"
#include "engine/common/CameraBuffer.h"
#include "engine/common/Drawing.h"
#include "engine/model/CBlinnPhong.h"
#include "engine/model/CCommon.h"
#include "engine/model/CBillboardPoint.h"
#include "engine/model/CBillboardCube.h"
#include "engine/model/CGlobalPixel.h"
#include "engine/model/CTextures.h"
#include "engine/dx11/Dx11EnginePSO.h"
#include "engine/dx11/Dx11ComputeBuffer.h"
#include "engine/dx11/Dx11StructedBuffer.h"

struct Particle
{
  Vec3 Position;
  Vec3 Color;
};

void ParticleExample();