
/*
 * Составление карт гарей лесных массивов
 *
 * Акинин М.В. (verzhak@gmail.com), студент группы 740М, кафедра ЭВМ, РГРТУ (Рязань, Россия)
 *
 * Научный руководитель: доцент, к.т.н., Конкин Ю.В.
 *
 * 12 февраля 2012-го года
 */

#include "various.h"

GDALDriverH drv_tiff = NULL, drv_r_mem = NULL;
OGRSFDriverH drv_shp = NULL;

int init()
{
	try;

		int ret = 0;

		GDALAllRegister();
		OGRRegisterAll();
		
		throw_null(drv_tiff = GDALGetDriverByName("GTiff"));
		throw_null(drv_r_mem = GDALGetDriverByName("MEM"));

		throw_null(drv_shp = OGRGetDriverByName("ESRI Shapefile"));

	catch;

		destroy();

		ret = -1;

	finally;

	return ret;
}

void destroy()
{
	OGRCleanupAll();
	GDALDestroyDriverManager();
}

s_sat * sat_init(unsigned ch_num)
{
	try;

		unsigned u;
		s_sat * sat = NULL;

		// throw_null((sat = sfire_alloc(sizeof(s_sat), 1, 1)));
		throw_null((sat = malloc(sizeof(s_sat))));

		sat->ch_num = ch_num;
		sat->proj_ref = NULL;

		// throw_null((at->pixel = sfire_alloc(sizeof(uint8_t *), 1, ch_num)));
		throw_null((sat->pixel = malloc(sizeof(uint8_t *) * ch_num)));

		for(u = 0; u < ch_num; u++)
			sat->pixel[u] = NULL;
				
	catch;

		sat_destroy(sat);

		sat = NULL;

	finally;

	return sat;
}

s_sat * sat_rasterize_copy(s_sat * c_sat, OGRGeometryH geometry)
{
	try;

		int band = 1;
		unsigned height, width, height_width;
		double burn_value = 255;
		GDALDatasetH ds = NULL;
		s_sat * sat = NULL;

		throw_null((sat = sat_init(1)));

		height = sat->height = c_sat->height;
		width = sat->width = c_sat->width;
		height_width = height * width;
		sat->proj_ref = strdup(c_sat->proj_ref);
		sat->ch_num = 0;

		memcpy(sat->gt_coef, c_sat->gt_coef, sizeof(double) * 6);
		throw_null((sat->pixel[0] = sfire_alloc(sizeof(uint8_t), 1, height_width)));

		sat->ch_num = 1;

		throw_null((ds = GDALCreate(drv_r_mem, "", width, height, 1, GDT_Byte, NULL)));
		throw((GDALSetProjection(ds, sat->proj_ref) == CE_Failure));
		throw((GDALSetGeoTransform(ds, sat->gt_coef) == CE_Failure));

		throw((GDALRasterizeGeometries(ds, 1, & band, 1, & geometry, NULL, NULL, & burn_value, NULL, NULL, NULL) == CE_Failure));

		throw(GDALDatasetRasterIO(ds, GF_Read, 0, 0, width, height, sat->pixel[0], width, height, GDT_Byte, 1, NULL, 0, 0, 0) == CE_Failure);

	catch;

		sat_destroy(sat);

		sat = NULL;

	finally;

		if(ds != NULL)
			GDALClose(ds);

	return sat;
}

void sat_destroy(s_sat * sat)
{
	if(sat != NULL)
	{
		unsigned u, ch_num = sat->ch_num;

		// sfire_free(sat->proj_ref);
		if(sat->proj_ref != NULL)
			free(sat->proj_ref); // Не munmap() !!!

		for(u = 0; u < ch_num; u++)
			sfire_free(sat->pixel[u]);

		free(sat->pixel);
		free(sat);
	}
}

s_sat * sat_load(const char * dname)
{
	try;

		bool is_first = true;
		unsigned v, height, width, height_width, raster_num = 0;
		char str[BUF_SIZE], * sstr;
		DIR * dir = NULL;
		struct dirent * entry;
		GDALDatasetH ds = NULL;
		s_sat * sat = NULL;
		
		throw_null((sat = sat_init(DIM)));
		throw_null((dir = opendir(dname)));

		while((entry = readdir(dir)) != NULL)
			for(v = 0; v < DIM; v++)
			{
				sprintf(str, "B%u0.tif", v + 1);
				sstr = strstr(entry->d_name, str);

				if((sstr != NULL) && (sstr[7] == '\0'))
				{
					sprintf(str, "%s/%s", dname, entry->d_name);

					throw_null(ds = GDALOpen(str, GA_ReadOnly));

					if(is_first)
					{
						height = sat->height = GDALGetRasterYSize(ds);
						width = sat->width = GDALGetRasterXSize(ds);
						throw((GDALGetGeoTransform(ds, sat->gt_coef) == CE_Failure));
						throw_null((sat->proj_ref = strdup(GDALGetProjectionRef(ds))));

						height_width = height * width;

						is_first = false;
					}

					throw_null((sat->pixel[v] = sfire_alloc(sizeof(uint8_t), 1, height_width)));
					throw((GDALDatasetRasterIO(ds, GF_Read, 0, 0, width, height, sat->pixel[v], width, height, GDT_Byte, 1, NULL, 0, 0, 0) == CE_Failure));
					GDALClose(ds);

					ds = NULL;

					raster_num++;

					break;
				}
			}

		throw((raster_num != DIM));

	catch;

		sat_destroy(sat);

		sat = NULL;

	finally;

		if(ds != NULL)
			GDALClose(ds);

		if(dir != NULL)
			closedir(dir);

	return sat;
}

int sat_save_ch(s_sat * sat, const char * fname, unsigned ch)
{
	try;

		int ret = 0;
		unsigned height = sat->height, width = sat->width;
		GDALDatasetH ds = NULL;

		throw_null((ds = GDALCreate(drv_tiff, fname, width, height, 1, GDT_Byte, NULL)));

		throw((GDALSetProjection(ds, sat->proj_ref) == CE_Failure));
		throw((GDALSetGeoTransform(ds, sat->gt_coef) == CE_Failure));

		throw(GDALDatasetRasterIO(ds, GF_Write, 0, 0, width, height, sat->pixel[ch], width, height, GDT_Byte, 1, NULL, 0, 0, 0) == CE_Failure);

	catch;

		ret = -1;

	finally;

		if(ds != NULL)
			GDALClose(ds);

	return ret;
}

s_sat * sat_aoi(s_sat * sat, const char * shp_aoi_fname, const char * shp_forest_fname)
{
	try;

		OGRDataSourceH a_ds = NULL, f_ds = NULL;
		OGRLayerH a_layer = NULL, f_layer = NULL;
		OGRGeometryH a_geometry = NULL, fa_geometry = NULL, union_geometry = NULL, intersection_geometry = NULL, simplify_geometry = NULL;
		OGRFeatureH next_feature = NULL;
		s_sat * fa_img = NULL;

		throw_null((a_ds = OGR_Dr_Open(drv_shp, shp_aoi_fname, FALSE)));
		throw_null((f_ds = OGR_Dr_Open(drv_shp, shp_forest_fname, FALSE)));

		throw_null((a_layer = OGR_DS_GetLayer(a_ds, 0)));
		throw_null((f_layer = OGR_DS_GetLayer(f_ds, 0)));

		throw_null((a_geometry = OGR_G_CreateGeometry(wkbPolygon)));
		throw_null((fa_geometry = OGR_G_CreateGeometry(wkbPolygon)));

		OGR_L_ResetReading(a_layer);
		
		while((next_feature = OGR_L_GetNextFeature(a_layer)) != NULL)
		{
			throw_null((union_geometry = OGR_G_Union(a_geometry, OGR_F_GetGeometryRef(next_feature))));

			OGR_G_DestroyGeometry(a_geometry);

			a_geometry = union_geometry;

			union_geometry = NULL;
		}

		OGR_L_SetSpatialFilter(f_layer, a_geometry);
		OGR_L_ResetReading(f_layer);

		while((next_feature = OGR_L_GetNextFeature(f_layer)) != NULL)
		{
			throw_null((intersection_geometry = OGR_G_Intersection(a_geometry, OGR_F_GetGeometryRef(next_feature))));
			throw_null((union_geometry = OGR_G_Union(fa_geometry, intersection_geometry)));

			OGR_G_DestroyGeometry(intersection_geometry);
			OGR_G_DestroyGeometry(fa_geometry);

			fa_geometry = union_geometry;

			union_geometry = intersection_geometry = NULL;
		}

		throw_null((simplify_geometry = OGR_G_Simplify(fa_geometry, 0)));
		
		OGR_G_DestroyGeometry(fa_geometry);

		fa_geometry = simplify_geometry;

		simplify_geometry = NULL;

		throw_null((fa_img = sat_rasterize_copy(sat, fa_geometry)));

	catch;

		sat_destroy(fa_img);

		fa_img = NULL;

	finally;

		if(next_feature != NULL)
			OGR_F_Destroy(next_feature);

		if(a_geometry != NULL)
			OGR_G_DestroyGeometry(a_geometry);

		if(fa_geometry != NULL)
			OGR_G_DestroyGeometry(fa_geometry);

		if(union_geometry != NULL)
			OGR_G_DestroyGeometry(union_geometry);

		if(intersection_geometry != NULL)
			OGR_G_DestroyGeometry(intersection_geometry);

		if(simplify_geometry != NULL)
			OGR_G_DestroyGeometry(simplify_geometry);
		
		if(a_ds != NULL)
			OGRReleaseDataSource(a_ds);

		if(f_ds != NULL)
			OGRReleaseDataSource(f_ds);

	return fa_img;
}

