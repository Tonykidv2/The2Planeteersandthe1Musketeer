#pragma once
#include <fstream>

namespace LoadModel
{
	bool LoadObj(const char* path, std::vector<DirectX::XMFLOAT4>& out_vertex, 
		std::vector<DirectX::XMFLOAT3>& out_UVs, std::vector<DirectX::XMFLOAT3>& out_Normals, 
		std::vector<unsigned int>& vertex_indices, std::vector<unsigned int>& uv_indices,
		std::vector<unsigned int>& normal_indices)
	{
		
		char input, input2;
		bool checked = false;
		

		std::fstream fin;

		fin.open(path, std::ios_base::in);

		if (fin.fail())
			return false;

		if (fin.is_open())
		{

			while (!fin.eof())
			{
				fin.get(input);
				if (input == '2')
				{
					input = 'd';
				}
				if (input == 'v')
				{
					fin.get(input);
					if (input == ' ')
					{
						DirectX::XMFLOAT4 temp;
						fin >> temp.x >> temp.y >> temp.z;
						temp.x = -temp.x;
						out_vertex.push_back(temp);
					}
					else if (input == 't')
					{
						DirectX::XMFLOAT3 temp;
						fin >> temp.x >> temp.y;
						temp.y = 1.0f - temp.y;
						temp.z = 0;
						out_UVs.push_back(temp);
					}
					else if (input == 'n')
					{
						DirectX::XMFLOAT3 temp;
						fin >> temp.x >> temp.y >> temp.z;
						temp.x = -temp.x;
						out_Normals.push_back(temp);
					}

					while (input != '\n')
					{
						fin.get(input);
					}
				}
				else if (input == 'f')
				{
					DirectX::XMINT3 vIndex, uvIndex, norIndex;

					fin.get(input);
					fin >> vIndex.x >> input2 >> uvIndex.x >> input2 >> norIndex.x
						>> vIndex.y >> input2 >> uvIndex.y >> input2 >> norIndex.y
						>> vIndex.z >> input2 >> uvIndex.z >> input2 >> norIndex.z;

					vertex_indices.push_back(vIndex.x - 1);
					vertex_indices.push_back(vIndex.y - 1);
					vertex_indices.push_back(vIndex.z - 1);

					uv_indices.push_back(uvIndex.x - 1);
					uv_indices.push_back(uvIndex.y - 1);
					uv_indices.push_back(uvIndex.z - 1);

					normal_indices.push_back(norIndex.x - 1);
					normal_indices.push_back(norIndex.y - 1);
					normal_indices.push_back(norIndex.z - 1);
				}
			}
			fin.close();
			return true;
		}
		else
			return false;

		return false;
	}


}