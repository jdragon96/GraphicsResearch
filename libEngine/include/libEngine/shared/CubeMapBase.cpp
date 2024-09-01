#include "libEngine/shared/CubeMapBase.h"

namespace libEngine
{
CubeMapBase::CubeMapBase()
{
  m_textureType = CubeTextureType::NONE;
}
CubeMapBase::~CubeMapBase()
{
}
void CubeMapBase::LoadDDS(std::vector<std::string> ddsPath)
{
  m_textureType = CubeTextureType::DDS;
  m_ddsList     = ddsPath;
}
void CubeMapBase::LoadPictures(std::vector<std::vector<std::string>> files)
{
  m_textureType = CubeTextureType::FILES;
  m_singleFiles = files;
}
}  // namespace libEngine
