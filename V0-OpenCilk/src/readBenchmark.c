#include <../include/knn.h>
#include <hdf5.h>

int load_hdf5(const char* filename, const char* matname, Matrix* matrix) {

  hid_t file_id = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
  if(file_id < 0) {
    printf("Error opening HDF5 file: %s\n", filename);
    return -1;
  }

  hid_t dataset_id = H5Dopen(file_id, matname, H5P_DEFAULT);
  if(dataset_id < 0) {
    printf("Error opening dataset: %s\n", matname);
    H5Fclose(file_id);
    return -1;
  }

  hid_t   space_id = H5Dget_space(dataset_id);
  int     ndims    = H5Sget_simple_extent_ndims(space_id);
  hsize_t dims[2];
  H5Sget_simple_extent_dims(space_id, dims, NULL);

  matrix->rows = dims[0];
  matrix->cols = dims[1];

  matrix->data = (double*)malloc(matrix->rows*matrix->cols*sizeof(double));
  if(matrix->data == NULL) {
    printf("Memory allocation failed\n");
    H5Sclose(space_id);
    H5Dclose(dataset_id);
    H5Fclose(file_id);
    return -1;
  }

  H5Dread(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, matrix->data);

  H5Sclose(space_id);
  H5Dclose(dataset_id);
  H5Fclose(file_id);

  printf("Matrix %s loaded successfully!\n", matname);
  return 0;
}