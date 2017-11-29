#ifndef _REGION_HPP_
#define _REGION_HPP_

#include <atomic>
#include <vector>
#include "../math.hpp"

struct Chunk_Textured_Mesh
{
	// TODO(Xavier): (2017.11.29)
	// This needs to be implemented.
	// It should also be moved to its own file.
	unsigned int vao;
	unsigned int vbo;
	unsigned int ibo;
};

enum class Floor
{
	NONE = 0,
};

enum class Wall
{
	NONE = 0,
};

enum class Object
{
	NONE = 0,
};

enum class Direction
{
	NONE = 0,
	N = 1,
	S = 2,
	E = 3,
	W = 4
};

enum class Selection_Type
{
	NONE,
	SINGLE_TILE, 
	MULTI_TILE, 
	ENTITY,
};

struct Region
{
	///////////////////////////////
	// SIMULATION DATA & METHODS
	struct
	{
		Floor *floor;
		Wall *wall;
		unsigned char *water;
		Object *object;
		Direction *direction;
	} tiles;

	std::vector<vec3> meshesNeedingUpdate_floor;
	std::vector<vec3> meshesNeedingUpdate_wall;
	std::vector<vec3> meshesNeedingUpdate_water;
	std::vector<vec3> meshesNeedingUpdate_object;

	void simulate();

		Floor get_floor ( int x, int y, int z );
		Wall get_wall ( int x, int y, int z );
		unsigned char get_water ( int x, int y, int z );
		Object get_object ( int x, int y, int z );
		Direction get_direction ( int x, int y, int z );

		void build_floor_mesh();
		void build_wall_mesh();
		void build_water_mesh();
		void build_object_mesh();


	///////////////////////////
	// SHARED DATA & METHODS
	struct Chunk_Mesh_Data
	{
		std::size_t ageIdentifier;
		vec3 position;
		std::vector<float> floorData;
		std::vector<float> wallData;
		std::vector<float> waterData;
		std::vector<float> objectData;
	};
	std::atomic<bool> simulationUsingUploadQue_1;
	std::atomic<bool> renderingUsingUploadQue_1;
	std::vector<Chunk_Mesh_Data> chunkMeshesToBeUploaded_1;
	std::atomic<bool> simulationUsingUploadQue_2;
	std::atomic<bool> renderingUsingUploadQue_2;
	std::vector<Chunk_Mesh_Data> chunkMeshesToBeUploaded_2;

	struct Command
	{
		// TODO(Xavier): (2017.11.29)
		// This needs more details.
		unsigned int type;
	};
	std::atomic<bool> simulationUsingCommandQue_1;
	std::atomic<bool> renderingUsingCommandQue_1;
	std::vector<Command> commandQue_1;
	std::atomic<bool> simulationUsingCommandQue_2;
	std::atomic<bool> renderingUsingCommandQue_2;
	std::vector<Command> commandQue_2;

	std::atomic<bool> simulationWritingToSelection;
	std::atomic<bool> renderingWritingToSelection;
	std::atomic<bool> renderingReadingFromSelection;
	std::atomic<bool> selectionActive;
	struct
	{
		Selection_Type type;
		struct
		{
			vec3 position;
		} info;
	} selection;

	unsigned int length;
	unsigned int width;
	unsigned int height;

	// NOTE(Xavier): (2017.11.29)
	// These methods are a special case as they will be 
	// called from the main thread but access data for the simulation thread.
	// Init will probably have to stay like this, but generate, load & save could
	// probably be moved to the simulation thread and be called from an issued command.
	void init( unsigned int l, unsigned int w, unsigned int h );
	void generate();
	void load();
	void save();
	void cleanup();


	//////////////////////////////
	// RENDERING DATA & METHODS
	struct Chunk_Mesh
	{
		vec3 position;
		Chunk_Textured_Mesh floor;
		std::size_t ageIdentifier_floor;
		Chunk_Textured_Mesh wall;
		std::size_t ageIdentifier_wall;
		Chunk_Textured_Mesh water;
		std::size_t ageIdentifier_water;
		Chunk_Textured_Mesh object;
		std::size_t ageIdentifier_object;
	};
	std::vector<Chunk_Mesh> chunkMeshes;

	unsigned int shader;

	void render();
	void issue_command();
	
		// These methods will build their own selection meshes.
		void select_position( int x, int y, int z );
		void select_area( int x, int y, int z );
		void cancel_selection();
		
		void upload_floor_mesh( Chunk_Mesh_Data& meshData );
		void upload_wall_mesh( Chunk_Mesh_Data& meshData );
		void upload_water_mesh( Chunk_Mesh_Data& meshData );
		void upload_object_mesh( Chunk_Mesh_Data& meshData );
};

#endif