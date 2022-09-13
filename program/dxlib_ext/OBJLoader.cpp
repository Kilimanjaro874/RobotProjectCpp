#pragma region File Description
//-----------------------------------------------------------------------
// File : OBJLoader.cpp
// Desc : Alias Wavefront OBJ File Loader
// Date : Mar. 04, 2009
// Version : 2.1
// Author : Pocol
// Memo : 4�p�`�Ή��ς݁B5�p�`�ȏ�̓T�|�[�g���Ȃ��B
//-----------------------------------------------------------------------
#pragma endregion 

#pragma region Includes
//
// Includes
//
#include "OBJLoader.h"
#pragma endregion

#pragma region Defines
//
// Defines
//

// RtlZeroMemory Macro
#ifndef RtlZeroMemory
#define RtlZeroMemory(Destination,Length) memset((Destination),0,(Length))
#endif

// ZeroMemory Macro
#ifndef ZeroMemory
#define ZeroMemory RtlZeroMemory
#endif

// SAFE_DELETE_ARRAY
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) { if (x) { delete [] (x); (x) = NULL; } }
#endif
#pragma endregion


//////////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
// Name : GetDirectionPath()
// Desc : �����񂩂�f�B���N�g�����擾
//------------------------------------------------------------------------
char* GetDirectoryPath( const char* filename )
{
	char *strings = NULL;
	char *dir;
	dir = new char [strlen(filename)+1];
	strcpy(dir, filename);
	if ( strings = strrchr(dir, '/' ) ) strings[1] = '\0';
	else if ( strings = strrchr(dir, '\\') ) strings[1] ='\0';
	else dir[0] = '\0';
	return dir;
}

//-----------------------------------------------------------------------
// Name : CutDirectoryPath()
// Desc : �����񂩂�f�B���N�g�����폜
//-----------------------------------------------------------------------
char* CutDirectoryPath( const char* filename )
{
	char *s=NULL;
	char *base=NULL;
	base = new char [strlen(filename)+1];
	strcpy( base, filename );
	if ( s = strrchr(base, '\\') )
	{
		s++;
		return s;
	}
	else if ( s = strrchr(base, '/') )
	{
		s++;
		return s;
	}
	else
	{ 
		return base;
	}
}

//-----------------------------------------------------------------------
// Name : SetDirectoryPath
// Desc : �f�B���N�g����O�ɕt�����ĕ������Ԃ�
//-----------------------------------------------------------------------
char* SetDirectoryPath( char* dest, char* directory )
{
	char* str;
	str = new char[strlen(directory) + strlen(dest) + 1];
	strcpy( str, directory );
	strcat( str, dest );
	return str;
}

//-----------------------------------------------------------------------
// Name : InitMaterial()
// Desc : �}�e���A���̏�����
//-----------------------------------------------------------------------
void InitMaterial( OBJMATERIAL* pMaterial )
{
	ZeroMemory( pMaterial, sizeof( OBJMATERIAL ) );
	pMaterial->ambient = OBJVEC3( 0.2f, 0.2f, 0.2f );
	pMaterial->diffuse = OBJVEC3( 0.8f, 0.8f, 0.8f );
	pMaterial->specular = OBJVEC3( 1.0f, 1.0f, 1.0f );
	pMaterial->shininess = 0.0f;
	pMaterial->alpha = 1.0f;
}

//-----------------------------------------------------------------------
// Name : SetMaterial()
// Desc : �}�e���A����ݒ肷��
//-----------------------------------------------------------------------
void SetMaterial( OBJMATERIAL* pMaterial )
{
	//glColor4f( pMaterial->diffuse.x, pMaterial->diffuse.y, pMaterial->diffuse.z, pMaterial->alpha );
	//glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, pMaterial->ambient );
	//glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, pMaterial->diffuse );
	//glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, pMaterial->specular );
	//glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, &pMaterial->shininess );
}

//-----------------------------------------------------------------------
// Name : OBJVEC3Scale()
// Desc : �X�P�[���{����
//-----------------------------------------------------------------------
OBJVEC3 OBJVEC3Scale( OBJVEC3 v, float scale )
{ return OBJVEC3( v.x * scale, v.y * scale, v.z * scale ); }

//-----------------------------------------------------------------------
// Name : OBJVEC3Add()
// Desc : �����Z����
//-----------------------------------------------------------------------
OBJVEC3 OBJVEC3Add( OBJVEC3 v1, OBJVEC3 v2 )
{ return OBJVEC3( v1.x + v2.x, v1.y + v2.y, v1.z + v2.z ); }

//-----------------------------------------------------------------------
// Name : OBJVEC3Substract()
// Desc : �����Z����
//-----------------------------------------------------------------------
OBJVEC3 OBJVEC3Substract( OBJVEC3 v1, OBJVEC3 v2 )
{ return OBJVEC3( v1.x - v2.x, v1.y - v2.y, v1.z - v2.z ); }

//-----------------------------------------------------------------------
// Name : OBJVEC3Length()
// Desc : ���������߂�
//-----------------------------------------------------------------------
float OBJVEC3Length( OBJVEC3 v )
{ return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z); }

//-----------------------------------------------------------------------
// Name : OBJVEC3Max()
// Desc : �ő�l�����߂�
//-----------------------------------------------------------------------
OBJVEC3 OBJVEC3Max( OBJVEC3 v, OBJVEC3 _max )
{
	OBJVEC3 result;
	result.x = ( v.x > _max.x ? v.x : _max.x );
	result.y = ( v.y > _max.y ? v.y : _max.y );
	result.z = ( v.z > _max.z ? v.z : _max.z );
	return result;
}

//------------------------------------------------------------------------
// Name : OBJVEC3Min()
// Desc : �ŏ��l�����߂�
//------------------------------------------------------------------------
OBJVEC3 OBJVEC3Min( OBJVEC3 v, OBJVEC3 _min )
{
	OBJVEC3 result;
	result.x = ( v.x < _min.x ? v.x : _min.x );
	result.y = ( v.y < _min.y ? v.y : _min.y );
	result.z = ( v.z < _min.z ? v.z : _min.z );
	return result;
}

//////////////////////////////////////////////////////////////////////////
// OBJVEC2
//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
// Name : OBJVEC2()
// Desc : �R���X�g���N�^
//------------------------------------------------------------------------
OBJVEC2::OBJVEC2( float nx, float ny )
: x(nx), y(ny)
{
}

//-----------------------------------------------------------------------
// operator float*
//-----------------------------------------------------------------------
OBJVEC2::operator float * () 
{ return (float*)&x; }

//-----------------------------------------------------------------------
// operator const float*
//-----------------------------------------------------------------------
OBJVEC2::operator const float *() const 
{ return (const float*)&x; }


//////////////////////////////////////////////////////////////////////////
// OBJVEC3
//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
// Name : OBJVEC3()
// Desc : �R���X�g���N�^
//------------------------------------------------------------------------
OBJVEC3::OBJVEC3( float nx, float ny, float nz )
: x(nx), y(ny), z(nz)
{
}

//------------------------------------------------------------------------
// operator float*
//------------------------------------------------------------------------
OBJVEC3::operator float *() 
{ return (float*)&x; }

//------------------------------------------------------------------------
// operator const float*
//------------------------------------------------------------------------
OBJVEC3::operator const float *() const
{ return (const float*)&x; }

//////////////////////////////////////////////////////////////////////////
// OBJBOUNDINGBOX
/////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
// Name : OBJBOUNDINGBOX()
// Desc : �R���X�g���N�^
//-----------------------------------------------------------------------
OBJBOUNDINGBOX::OBJBOUNDINGBOX( OBJVEC3 value )
: minimum( value ), maximum( value )
{
}

//-----------------------------------------------------------------------
// Name : Merage()
// Desc : 
//-----------------------------------------------------------------------
void OBJBOUNDINGBOX::Merge(OBJVEC3 value)
{
	maximum = OBJVEC3Max( value, maximum );
	minimum = OBJVEC3Min( value, minimum );
	size = OBJVEC3Substract( maximum, minimum );
}

//////////////////////////////////////////////////////////////////////////
// OBJBOUNDINGSPHERE
//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
// Name : Create()
// Desc :
//------------------------------------------------------------------------
void OBJBOUNDINGSPHERE::Create( OBJBOUNDINGBOX box )
{
	center = OBJVEC3Add( box.maximum, box.minimum );
	center = OBJVEC3Scale( center, 0.5f );

	OBJVEC3 half;
	half = OBJVEC3Substract( box.maximum, center );
	radius = OBJVEC3Length( half );
}


/////////////////////////////////////////////////////////////////////////
// OBJMESH
/////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
// Name : OBJMESH()
// Desc : �R���X�g���N�^
//-----------------------------------------------------------------------
OBJMESH::OBJMESH()
{
	m_NumVertices = 0;
	m_NumMaterials = 0;
	m_NumSubsets = 0;
	m_NumIndices = 0;

	m_Vertices = 0;
	m_Materials = 0;
	m_Subsets = 0;
	m_Indices = 0;
}

//-----------------------------------------------------------------------
// Name : ~OBJMESH()
// Desc : �f�X�g���N�^
//-----------------------------------------------------------------------
OBJMESH::~OBJMESH()
{
	Release();
};

//-----------------------------------------------------------------------
// Name : Release()
// Desc : ��������j��
//-----------------------------------------------------------------------
void OBJMESH::Release()
{
	SAFE_DELETE_ARRAY( m_Vertices );
	SAFE_DELETE_ARRAY( m_Materials );
	SAFE_DELETE_ARRAY( m_Subsets );
	SAFE_DELETE_ARRAY( m_Indices );
	
	m_NumVertices = 0;
	m_NumMaterials = 0;
	m_NumSubsets = 0;
	m_NumIndices = 0;
}

//-----------------------------------------------------------------------
// Name : LoadOBJFile()
// Desc : OBJ�t�@�C���̓ǂݍ���
//-----------------------------------------------------------------------
bool OBJMESH::LoadOBJFile(const char *filename)
{
	std::ifstream file;

	char mtlFileName[OBJ_NAME_LENGTH] = {0};
	char buf[OBJ_BUFFER_LENGTH] = {0};
	std::vector<OBJVEC3> positions;
	std::vector<OBJVEC3> normals;
	std::vector<OBJVEC2> texcoords;
	std::vector<OBJVERTEX> t_vertices;
	std::vector<OBJSUBSET> t_subsets;
	std::vector<unsigned int> t_indices;
	bool initBox = false;
	int prevSize = 0;

	unsigned long total = 0;

	OBJMATERIAL material;
	unsigned int dwFaceIndex = 0;
	unsigned int dwFaceCount = 0;
	unsigned int dwCurSubset = 0;

	//�@�f�B���N�g����؂���
	strcpy( m_directoryPath, GetDirectoryPath( filename ) );

	//�@�t�@�C�����J��
	file.open( filename, std::ios::in );

	//�@�`�F�b�N
	if ( !file.is_open() )
	{
		//cerr << "Error : �t�@�C���I�[�v���Ɏ��s\n";
		//cerr << "File Name : " << filename << endl;
		std::string str = filename;
		return false;
	}

	//�@���[�v
	for( ;; )
	{
		file >> buf;
		if ( !file )
			break;

		//�@�R�����g
		if ( 0 == strcmp( buf, "#" ) )
		{
		}

		//�@���_���W
		else if ( 0 == strcmp( buf, "v" ) )
		{
			float x, y, z;
			file >> x >> y >> z;
			OBJVEC3 v( x, y, z );
			positions.push_back( v );

			//�@�o�E���f�B���O�{�b�N�X�̏�����
			if ( !initBox )
			{
				m_Box = OBJBOUNDINGBOX( v );
				initBox = true;
			}

			//�@�o�E���f�B���O�{�b�N�X�̎Z�o
			m_Box.Merge( v );
		}

		//�@�e�N�X�`�����W
		else if ( 0 == strcmp( buf, "vt" ) )
		{
			float u, v;
			file >> u >> v;
			texcoords.push_back( OBJVEC2( u, v ) );
		}

		//�@�@���x�N�g��
		else if ( 0 == strcmp( buf, "vn" ) )
		{
			float x, y, z;
			file >> x >> y >> z;
			normals.push_back( OBJVEC3( x, y, z) );
		}

		//�@��
		else if ( 0 == strcmp( buf, "f" ) )
		{
			unsigned int iPosition, iTexCoord, iNormal;
			unsigned int p[4] = {-1}, t[4] = {-1}, n[4] = {-1};	
			OBJVERTEX vertex;
			dwFaceIndex++;
			dwFaceCount++;
			int count = 0;
			unsigned int index = 0;
		
			//�@�O�p�`�E�l�p�`�̂ݑΉ�
			for ( int iFace = 0; iFace < 4; iFace++ )
			{
				count++;	//�@���_���𐔂���
				ZeroMemory( &vertex, sizeof( OBJVERTEX ) );

				file >> iPosition;
				vertex.position = positions[ iPosition - 1 ];
				p[iFace] = iPosition -1;
				
				if ( '/' == file.peek() )
				{
					file.ignore();

					//�@�e�N�X�`�����W�C���f�b�N�X
					if ( '/' != file.peek() )
					{
						file >> iTexCoord;
						vertex.texcoord = texcoords[ iTexCoord - 1 ];
						t[iFace] = iTexCoord -1;
					}

					//�@�@���x�N�g���C���f�b�N�X
					if ( '/' == file.peek() )
					{
						file.ignore();

						file >> iNormal;
						vertex.normal = normals[ iNormal - 1 ];
						n[iFace] = iNormal -1;
					}
				}

				//�@�J�E���g��3����
				if ( iFace < 3 )
				{
					t_vertices.push_back( vertex );
					index = t_vertices.size()-1;
					t_indices.push_back( index );
				}

				//�@�������s��������I��
				if ( '\n' == file.peek() )
				{
					break;
				}

			}

			//�@�l�p�`�|���S���̏ꍇ�C�O�p�`��ǉ�����
			if ( count > 3 )
			{
				//�@�J�E���g
				dwFaceIndex++;
				dwFaceCount++;

				//�@���_�ƃC���f�b�N�X��ǉ�
				for ( int iFace = 1; iFace < 4; iFace++ )
				{
					int j = (iFace+1)%4;
					ZeroMemory( &vertex, sizeof( OBJVERTEX ) );

					if ( p[j] != -1 ) vertex.position = positions[ p[j] ];
					if ( t[j] != -1 ) vertex.texcoord = texcoords[ t[j] ];
					if ( n[j] != -1 ) vertex.normal = normals[ n[j] ];

					t_vertices.push_back( vertex );
					index = t_vertices.size() - 1;
					t_indices.push_back( index );
				}

			}
		}

		//�@�}�e���A���t�@�C��
		else if ( 0 == strcmp( buf, "mtllib" ) )
		{
			file >> mtlFileName;
			//�@�}�e���A���t�@�C���̓ǂݍ���
			if ( mtlFileName[0] )
			{
				if ( !LoadMTLFile( SetDirectoryPath(mtlFileName, m_directoryPath) ) )
				{
					//cerr << "Error : �}�e���A���̃��[�h�Ɏ��s\n";
					std::string str = mtlFileName;

					return false;
				}
			}
		}

		//�@�}�e���A��
		else if ( 0 == strcmp( buf, "usemtl" ) )
		{
			char strName[OBJ_NAME_LENGTH] = {0};
			file >> strName;
			OBJSUBSET subset;
			subset.faceCount = 1;

			for ( unsigned int i = 0; i < m_NumMaterials; i++ )
			{
				if ( 0 == strcmp( m_Materials[i].name, strName ) )
				{
					dwCurSubset = i;
					break;
				}
			}

			subset.materialIndex = dwCurSubset;
			subset.faceStart = dwFaceIndex*3;
			prevSize = t_subsets.size();
			t_subsets.push_back( subset );
			if ( t_subsets.size() > 1 )
			{
				t_subsets[prevSize-1].faceCount = dwFaceCount*3;
				dwFaceCount = 0;
			}
		}

		file.ignore( OBJ_BUFFER_LENGTH, '\n' );
	}

	//�@�T�u�Z�b�g
	if ( t_subsets.size() > 0 )
	{
		int maxSize = t_subsets.size();
		t_subsets[maxSize-1].faceCount = dwFaceCount*3;
	}

	//�@�t�@�C�������
	file.close();

	//�@���_�f�[�^���R�s�[
	m_NumVertices = t_vertices.size();
	m_Vertices = new OBJVERTEX[ m_NumVertices ];
	for ( unsigned int i = 0; i<m_NumVertices; i++ )
		m_Vertices[i] = t_vertices[i];

	//�@�T�u�Z�b�g�f�[�^���R�s�[
	m_NumSubsets = t_subsets.size();
	m_Subsets = new OBJSUBSET[ m_NumSubsets ];
	for ( unsigned int i =0; i<m_NumSubsets; i++ )
		m_Subsets[i] = t_subsets[i];

	//�@�C���f�b�N�X�f�[�^���R�s�[
	m_NumIndices = t_indices.size();
	m_Indices = new unsigned int [ m_NumIndices ];
	for ( unsigned int i = 0; i<m_NumIndices; i++ )
		m_Indices[i] = t_indices[i];

	//�@�o�E���f�B���O�X�t�B�A�̍쐬
	m_Sphere.Create( m_Box );

	//�@�������j��
	positions.clear();
	normals.clear();
	texcoords.clear();
	t_vertices.clear();
	t_subsets.clear();
	t_indices.clear();

	//�@����I��
	return true;
}


void OBJMESH::LoadInfoMaterialSubset(const char* filename)
{
	std::ifstream file;

	char mtlFileName[OBJ_NAME_LENGTH] = { 0 };
	char buf[OBJ_BUFFER_LENGTH] = { 0 };
	std::vector<OBJSUBSET> t_subsets;
	int prevSize = 0;

	unsigned int dwFaceIndex = 0;
	unsigned int dwFaceCount = 0;
	unsigned int dwCurSubset = 0;

	//�@�f�B���N�g����؂���
	strcpy(m_directoryPath, GetDirectoryPath(filename));

	//�@�t�@�C�����J��
	file.open(filename, std::ios::in);

	//�@�`�F�b�N
	if (!file.is_open())
	{
		//_ASSERT(0, "obj file open error");
	}

	//�@���[�v
	for (;; )
	{
		file >> buf;
		if (!file)
			break;

		//�@�R�����g
		if (0 == strcmp(buf, "#"))
		{
		}

		//�@�}�e���A���t�@�C��
		else if (0 == strcmp(buf, "mtllib"))
		{
			file >> mtlFileName;
			//�@�}�e���A���t�@�C���̓ǂݍ���
			if (mtlFileName[0])
			{
				if (!LoadMTLFile(SetDirectoryPath(mtlFileName, m_directoryPath)))
				{
					//cerr << "Error : �}�e���A���̃��[�h�Ɏ��s\n";
					std::string str = mtlFileName;
					//_ASSERT(0, "mtl file open error");
				}
			}
		}

		//�@�}�e���A��
		else if (0 == strcmp(buf, "usemtl"))
		{
			char strName[OBJ_NAME_LENGTH] = { 0 };
			file >> strName;
			OBJSUBSET subset;
			subset.faceCount = 1;

			for (unsigned int i = 0; i < m_NumMaterials; i++)
			{
				if (0 == strcmp(m_Materials[i].name, strName))
				{
					dwCurSubset = i;
					break;
				}
			}

			subset.materialIndex = dwCurSubset;
			subset.faceStart = dwFaceIndex * 3;
			prevSize = t_subsets.size();
			t_subsets.push_back(subset);
			if (t_subsets.size() > 1)
			{
				t_subsets[prevSize - 1].faceCount = dwFaceCount * 3;
				dwFaceCount = 0;
			}
		}

		file.ignore(OBJ_BUFFER_LENGTH, '\n');
	}

	//�@�T�u�Z�b�g
	if (t_subsets.size() > 0)
	{
		int maxSize = t_subsets.size();
		t_subsets[maxSize - 1].faceCount = dwFaceCount * 3;
	}

	//�@�t�@�C�������
	file.close();

	//�@�T�u�Z�b�g�f�[�^���R�s�[
	m_NumSubsets = t_subsets.size();
	m_Subsets = new OBJSUBSET[m_NumSubsets];
	for (unsigned int i = 0; i < m_NumSubsets; i++)
		m_Subsets[i] = t_subsets[i];
	t_subsets.clear();

	//�@����I��
	return ;
}


//-----------------------------------------------------------------------
// Name : LoadMTLFile()
// Desc : MTL�t�@�C���̓ǂݍ���
//-----------------------------------------------------------------------
bool OBJMESH::LoadMTLFile( const char* filename )
{
	char buf[OBJ_BUFFER_LENGTH] = {0};
	int iMtlCount = -1;
	std::ifstream file;
	std::vector<OBJMATERIAL> t_materials;
	OBJMATERIAL material;
	InitMaterial( &material );

	//�@�t�@�C�����J��
	file.open( filename, std::ios::in );

	//�@�`�F�b�N
	if ( !file.is_open() )
	{
		//cerr << "Error : �t�@�C���I�[�v���Ɏ��s���܂���\n";
		//cerr << "File Name : " << filename << endl;
		std::string str = filename;

		return false;
	}

	//�@���[�v
	for( ;; )
	{
		file >> buf;
		if ( !file )
			break;

		// New Material
		if ( 0 == strcmp( buf, "newmtl" ) )
		{
			iMtlCount++;
			t_materials.push_back( material );
			char strName[OBJ_NAME_LENGTH] = {0};
			file >> strName;
			strcpy( t_materials[iMtlCount].name, strName );
		}
		// Ambient Color
		else if ( 0 == strcmp( buf, "Ka" ) )
		{
			float r, g, b;
			file >> r >> g >> b;
			t_materials[iMtlCount].ambient = OBJVEC3( r, g, b );
		}
		// Diffuse Color
		else if ( 0 == strcmp( buf, "Kd" ) )
		{
			float r, g, b;
			file >> r >> g >> b;
			t_materials[iMtlCount].diffuse = OBJVEC3( r, g, b );
		}
		// Specular Color
		else if ( 0 == strcmp( buf, "Ks" ) )
		{
			float r, g, b;
			file >> r >> g >> b;
			t_materials[iMtlCount].specular = OBJVEC3( r, g, b );
		}
		// Alpha
		else if ( 0 == strcmp( buf, "d" ) ||
				  0 == strcmp( buf, "Tr" ) )
		{
			file >> t_materials[iMtlCount].alpha;
		}
		// Shininess
		else if ( 0 == strcmp( buf, "Ns" ) )
		{
			file >> t_materials[iMtlCount].shininess;
		}
		// Ambient Map
		else if ( 0 == strcmp( buf, "map_Ka" ) )
		{
			char mapKaName[OBJ_NAME_LENGTH];
			file >> mapKaName;
			SetDirectoryPath( mapKaName, m_directoryPath );
			strcpy( t_materials[iMtlCount].ambientMapName, mapKaName );
		}
		// Diffuse Map
		else if ( 0 == strcmp( buf, "map_Kd" ) )
		{
			char mapKdName[OBJ_NAME_LENGTH];
			file >> mapKdName;
			SetDirectoryPath( mapKdName, m_directoryPath );
			strcpy( t_materials[iMtlCount].diffuseMapName, mapKdName );
		}
		// Specular Map
		else if ( 0 == strcmp( buf, "map_Ks" ) )
		{
			char mapKsName[OBJ_NAME_LENGTH];
			file >> mapKsName;
			SetDirectoryPath( mapKsName, m_directoryPath );
			strcpy( t_materials[iMtlCount].specularMapName, mapKsName );
		}
		// Bump Map
		else if ( 0 == strcmp( buf, "map_Bump" ) )
		{
			char mapBumpName[OBJ_NAME_LENGTH];
			file >> mapBumpName;
			SetDirectoryPath( mapBumpName, m_directoryPath );
			strcpy( t_materials[iMtlCount].bumpMapName, mapBumpName );
		}

		file.ignore( OBJ_BUFFER_LENGTH, '\n' );
	}

	//�@�t�@�C�������
	file.close();

	//�@�}�e���A���f�[�^���R�s�[
	m_NumMaterials = t_materials.size();
	m_Materials = new OBJMATERIAL[ m_NumMaterials ];
	for ( unsigned int i = 0; i<m_NumMaterials; i++ )
		m_Materials[i] = t_materials[i];

	//�@����I��
	return true;
}

//-----------------------------------------------------------------------
// Name : LoadFile()
// Desc : ���b�V���t�@�C���̓ǂݍ���
//-----------------------------------------------------------------------
bool OBJMESH::LoadFile( const char* filename )
{
	//�@OBJ, MTL�t�@�C����ǂݍ���
	if ( !LoadOBJFile( filename ) )
	{
		//cerr << "Error : ���b�V���t�@�C���̓ǂݍ��݂Ɏ��s���܂���\n";
		return false;
	}

	//�@����I��
	return true;
}

			
//-----------------------------------------------------------------------
// Name : Draw()
// Desc : �`�揈��
//-----------------------------------------------------------------------
void Draw()
{
	//for ( unsigned int i = 0; i<m_NumSubsets; i++ )
	//{
	//	//�@�}�e���A��
	//	OBJMATERIAL* pMat = &m_Materials[m_Subsets[i].materialIndex];
	//	SetMaterial( pMat );	

	//	//�@�O�p�`�`��
	//	glInterleavedArrays( GL_T2F_N3F_V3F, 0, m_Vertices );
	//	glDrawElements( GL_TRIANGLES, m_Subsets[i].faceCount, GL_UNSIGNED_INT, &m_Indices[m_Subsets[i].faceStart] );
	//}
}


//------------------------------------------------------------------------
// Name : GetNumVertices()
// Desc : ���_�����擾
//------------------------------------------------------------------------
unsigned int OBJMESH::GetNumVertices()
{ return m_NumVertices; }

//-----------------------------------------------------------------------
// Name : GetNumSubsets()
// Desc : �T�u�Z�b�g�����擾
//-----------------------------------------------------------------------
unsigned int OBJMESH::GetNumSubsets()
{ 
	return m_NumSubsets; 
}

//-----------------------------------------------------------------------
// Name : GetNumMaterials()
// Desc : �}�e���A�������擾
//-----------------------------------------------------------------------
unsigned int OBJMESH::GetNumMaterials()
{ return m_NumMaterials; }

//-----------------------------------------------------------------------
// Name : GetNumIndices()
// Desc : �C���f�b�N�X�����擾
//-----------------------------------------------------------------------
unsigned int OBJMESH::GetNumIndices()
{ return m_NumIndices; }

//-----------------------------------------------------------------------
// Name : GetIndexData()
// Desc : �w�肳�ꂽ�C���f�b�N�X�f�[�^���擾
//-----------------------------------------------------------------------
unsigned int OBJMESH::GetIndexData( unsigned int index )
{ return m_Indices[index]; }

//-----------------------------------------------------------------------
// Name : GetVertex()
// Desc : �w�肳�ꂽ���_�f�[�^���擾
//-----------------------------------------------------------------------
OBJVERTEX OBJMESH::GetVertex( unsigned int index )
{ return m_Vertices[index]; }

//-----------------------------------------------------------------------
// Name : GetSubsets()
// Desc : �w�肳�ꂽ�T�u�Z�b�g�f�[�^���擾
//-----------------------------------------------------------------------
OBJSUBSET OBJMESH::GetSubset( unsigned int index )
{ return m_Subsets[index]; }

//-----------------------------------------------------------------------
// Name : GetMaterial()
// Desc : �w�肳�ꂽ�}�e���A���f�[�^���擾
//-----------------------------------------------------------------------
OBJMATERIAL OBJMESH::GetMaterial( unsigned int index )
{ return m_Materials[index]; }

//-----------------------------------------------------------------------
// Name : GetVertices()
// Desc : ���_�f�[�^���擾
//-----------------------------------------------------------------------
OBJVERTEX* OBJMESH::GetVertices()
{ return m_Vertices; }

//-----------------------------------------------------------------------
// Name : GetSubsets()
// Desc : �T�u�Z�b�g�f�[�^���擾
//-----------------------------------------------------------------------
OBJSUBSET* OBJMESH::GetSubsets()
{ return m_Subsets; }

//-----------------------------------------------------------------------
// Name : GetMaterails()
// Desc : �}�e���A���f�[�^���擾
//-----------------------------------------------------------------------
OBJMATERIAL* OBJMESH::GetMaterials()
{ return m_Materials; }

//-----------------------------------------------------------------------
// Name : GetIndices()
// Desc : �C���f�b�N�X�f�[�^���擾
//-----------------------------------------------------------------------
unsigned int* OBJMESH::GetIndices()
{ return m_Indices; }
		
//-----------------------------------------------------------------------
// Name : GetBox()
// Desc : �o�E���f�B���O�{�b�N�X���擾
//-----------------------------------------------------------------------
OBJBOUNDINGBOX OBJMESH::GetBox()
{ return m_Box; }

//-----------------------------------------------------------------------
// Name : GetSphere()
// Desc : �o�E���f�B���O�X�t�B�A���擾
//-----------------------------------------------------------------------
OBJBOUNDINGSPHERE OBJMESH::GetSphere()
{ return m_Sphere; }



					


			




