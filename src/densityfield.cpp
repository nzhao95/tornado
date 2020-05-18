
#include <openvdb/openvdb.h>
#include <iostream>
#include "densityfield.h"

void densityfield::saveVDB () {
	openvdb::initialize();
	openvdb::FloatGrid::Ptr vdbGrid =
		openvdb::FloatGrid::create(	      /*background value=*/0.0);

	// Get an accessor for coordinate-based access to voxels.
	openvdb::FloatGrid::Accessor accessor = vdbGrid->getAccessor();

	for (unsigned int i = 0; i < grid_size; ++i) {
		for (unsigned int j = 0; j < grid_size; ++j) {
			for (unsigned int k = 0; k < grid_size; ++k) {
				openvdb::Coord xyz(i, j, k);
                accessor.setValue(xyz, grid[i][k][j]);
			}
		}
	}
	vdbGrid->setName("Density Field");
    openvdb::io::File file("saves/smoke" + std::to_string((int) (current_time / dt)) +  ".vdb");
	openvdb::GridPtrVec grids;
	grids.push_back(vdbGrid);
	file.write(grids);
	file.close();
}
