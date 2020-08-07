#include "mesh.h"

bufferobject<uint>* mesh::GenerateIndiceBuffer(cuint size)
{
	uint* elements = new uint[size];
	uint* const endptr = elements + size;
	int i = 0;
	for (uint* ptr = elements; ptr < endptr; ptr++)
	{
		*ptr = i++;
	}
	return new bufferobject<uint>(elements, size, 3, size / 3);
}

//https://en.wikipedia.org/wiki/Wavefront_.obj_file
mesh::mesh(std::wstring path)
{
	std::wstring text = StringToWString(readalltext(path));
	wstringcontainer lines = split_string(text, L"\n");
	fastlist<vec3> positions = fastlist<vec3>();
	fastlist<vec2> texcoords = fastlist<vec2>();
	fastlist<uint> indices = fastlist<uint>();
	fastlist<vec3> lightlevels = fastlist<vec3>();
	for (int i = 0; i < lines.size(); i++)
	{
		std::wstring line = lines[i];
		trim(line);
		wstringcontainer words = split_string(line, L" ");
		if (words[0] == L"v")//vertice
		{
			vec3 v;
			strtod(words[1], v.x);
			strtod(words[2], v.y);
			strtod(words[3], v.z);
			positions.push_back(v);
		}
		else if (words[0] == L"vt")//texture coord
		{	
			vec2 v;
			strtod(words[1], v.x);
			strtod(words[2], v.y);
			texcoords.push_back(v);
		}
		else if (words[0] == L"f")//polygonal face element
		{
			//-1 because it starts at 1
			//check for normal indices
			int off0 = words[1].find(L'/');
			if (words[1].find(L'/') != std::wstring::npos)
			{
				//normal, do not read them
				int off1 = words[2].find(L'/');
				int off2 = words[3].find(L'/');
				indices.push_back(std::stoi(words[1].substr(0, off0)) - 1);
				indices.push_back(std::stoi(words[2].substr(0, off1)) - 1);
				indices.push_back(std::stoi(words[3].substr(0, off2)) - 1);
			}
			else 
			{
				indices.push_back(std::stoi(words[1]) - 1);
				indices.push_back(std::stoi(words[2]) - 1);
				indices.push_back(std::stoi(words[3]) - 1);
			}
			


		}
	}
	positions.update();
	texcoords.update();
	indices.update();
	this->vertices = new bufferobject<fp>((fp*)positions.basearray, positions.size * 3, 3, positions.size);
	this->textureCoordinates = new bufferobject<fp>((fp*)texcoords.basearray, texcoords.size * 2, 2, texcoords.size);
	this->indices = new bufferobject<uint>((uint*)indices.basearray, indices.size, 3, indices.size / 3);
}

void mesh::save(std::wstring path)
{
}

mesh::mesh(bufferobject<fp>* vertices, bufferobject<uint>* indices, bufferobject<fp>* textureCoordinates, Texture* tex, bufferobject<fp>* lightLevels)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textureCoordinates = textureCoordinates;
	this->tex = tex;
	this->lightLevels = lightLevels;
}

mesh::mesh(bufferobject<fp>* vertices, bufferobject<uint>* indices, bufferobject<color>* colors, bufferobject<fp>* lightLevels)
{
	this->vertices = vertices;
	this->indices = indices;
	this->colors = colors;
	this->lightLevels = lightLevels;
}

void mesh::CalculateLightLevels()
{
	vec3* lightLevels = new vec3[indices->stepcount * 3];
	cuint* indPtr = indices->buffer;
	for (int i = 0; i < indices->stepcount * 3; i+= 3, indPtr += indices->stride)
	{
		cuint indice0 = *indPtr;
		cuint indice1 = *(indPtr + 1);
		cuint indice2 = *(indPtr + 2);
		cvec3* vert0 = (cvec3*)&(*vertices)[indice0];
		cvec3* vert1 = (cvec3*)&(*vertices)[indice1];
		cvec3* vert2 = (cvec3*)&(*vertices)[indice2];
		cvec3 normal = vec3::cross(*vert1 - *vert0, *vert2 - *vert0).normalized();
		cvec3 lightlevel = CalculateLightlevelZ(normal);
		lightLevels[i] = lightlevel;
		lightLevels[i + 1] = lightlevel;
		lightLevels[i + 2] = lightlevel;
	}
	this->lightLevels = new bufferobject<fp>((cfp*)lightLevels, indices->stepcount * 9, 9, indices->stepcount);
}
//the normal vector has to be normalized
vec3 mesh::CalculateLightlevelZ(cvec3& normal)
{
	return vec3(normal.z * 0.4 + 0.6);
}

void mesh::ApplyMatrix(mat4x4 mat)
{
	{
		for (int i = 0; i < vertices->stepcount; i++)
		{
			vec3* cur = (vec3*)&(*vertices)[i];
			*cur = mat.multPointMatrix(*cur);
		}
	}
}
