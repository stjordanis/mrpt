#pragma once
#include<string>

#include "mrpt/maps/CSimpleMap.h"
#include "mrpt/maps/CMultiMetricMap.h"
#include "mrpt/opengl/CRenderizable.h"
#include "mrpt/utils/CConfigFile.h"


class CDocument
{
public:
	CDocument(const std::string& fileName, const std::string& config);
	~CDocument();

	const std::map<std::string, mrpt::opengl::CSetOfObjects::Ptr> renderizableMaps() const;

	const mrpt::maps::CSimpleMap &simplemap() const;
private:
	mrpt::maps::CSimpleMap m_simplemap;
	mrpt::maps::CMultiMetricMap m_metricmap;
};
