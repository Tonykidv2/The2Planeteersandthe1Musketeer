#include "stdafx.h"
#include "Utilities.h"


FbxAMatrix Utilities::GetGeometryTransformation(FbxNode* m_Node)
{
	if (!m_Node)
	{
		throw std::exception("Null for mesh Geometry");
	}

	const FbxVector4 translation = m_Node->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 rotation = m_Node->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 scaling = m_Node->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(translation, rotation, scaling);
}