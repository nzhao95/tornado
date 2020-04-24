
#include <openvdb/openvdb.h>

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
				accessor.setValue(xyz, grid[i][j][k]);
			}
		}
	}
	vdbGrid->setName("Density Field");
	openvdb::io::File file("mygrids.vdb");
	openvdb::GridPtrVec grids;
	grids.push_back(vdbGrid);
	file.write(grids);
	file.close();
}
