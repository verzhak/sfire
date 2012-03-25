
// Программный код, находящийся в данном файле, распространяется по лицензии GPLv3

#ifndef VARIOUS_H
#define VARIOUS_H

#include "all.h"
#include "exception.h"
#include "memory.h"
#include "print.h"

extern GDALDriverH drv_tiff, drv_r_mem;
extern OGRSFDriverH drv_shp;

typedef struct t_s_sat
{
	
	uint8_t ** pixel;
	unsigned ch_num, height, width;
	double gt_coef[6];
	char * proj_ref;

} s_sat;

int init();
void destroy();

s_sat * sat_init(unsigned ch_num);
void sat_destroy(s_sat * sat);
s_sat * sat_rasterize_copy(s_sat * c_sat, OGRGeometryH geometry);
s_sat * sat_load(const char * dname);
int sat_save_ch(s_sat * sat, const char * fname, unsigned ch);
s_sat * sat_aoi(s_sat * sat, const char * shp_aoi_fname, const char * shp_forest_fname);

#endif

