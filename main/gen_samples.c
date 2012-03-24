
#include "gen_samples.h"

double ndvi(double red, double nred)
{
	if(fabs(red + nred) > 0.5)
		return (nred - red) / (nred + red);

	return -1;
}

int gen_samples(char * sat_first_dname, char * sat_second_dname, char * shp_aoi_fname, char * shp_forest_fname, char ** out_fname)
{
	try;

		int notused, ret = 0;
		void * transform = NULL;
		uint8_t out_buf[DIM], * fa_buf, ** buf, * red_buf[2], * nred_buf[2];
		unsigned v, u, t, width_first, height_second, width_second, pi_first, pi_second, index;
		double x, y, z;
		FILE * fl[3] = { NULL, NULL, NULL };
		s_sat * fa_img = NULL, * sat[2] = { NULL, NULL };

		my_printf("Begin");

		init();

		throw_null((sat[0] = sat_load(sat_first_dname)));
		throw_null((sat[1] = sat_load(sat_second_dname)));
		throw_null((fa_img = sat_aoi(sat[1], shp_aoi_fname, shp_forest_fname)));
		throw_null((transform = GDALCreateGenImgProjTransformer3(sat[1]->proj_ref, sat[1]->gt_coef, sat[0]->proj_ref, sat[0]->gt_coef)));

		width_first = sat[0]->width;
		height_second = sat[1]->height;
		width_second = sat[1]->width;
		buf = sat[1]->pixel;
		red_buf[0] = sat[0]->pixel[2];
		red_buf[1] = buf[2];
		nred_buf[0] = sat[0]->pixel[3];
		nred_buf[1] = buf[3];
		fa_buf = fa_img->pixel[0];

		for(u = 0; u < 3; u++)
			throw_null((fl[u] = fopen(out_fname[u], "wb")));

		for(v = 0; v < height_second; v++)
		{
			my_printf("Row %u from %u ( %lf %% )", v + 1, height_second, (100.0 * (v + 1)) / height_second);

			for(u = 0; u < width_second; u++)
			{
				pi_second = v * width_second + u;

				if(fa_buf[pi_second])
				{
					index = 0;

					if(ndvi(red_buf[1][pi_second], nred_buf[1][pi_second]) > 0.5)
						index = 2;
					else
					{
						x = u;
						y = v;
						z = 0;

						if(GDALGenImgProjTransform(transform, FALSE, 1, & x, & y, & z, & notused) == TRUE && x >= 0 && y >= 0)
						{
							pi_first = y * width_first + x;

							if(ndvi(red_buf[0][pi_first], nred_buf[0][pi_first]) > 0.5)
								index = 1;
						}
					}

					for(t = 0; t < DIM; t++)
						out_buf[t] = buf[t][pi_second];

					throw((fwrite(out_buf, 1, DIM, fl[index]) != DIM));
				}
			}
		}

		my_printf("Success");

	catch;

		ret = -1;

		my_printf("Fail");

	finally;

		for(u = 0; u < 3; u++)
			if(fl[u] != NULL)
				fclose(fl[u]);

		if(transform != NULL)
			GDALDestroyGenImgProjTransformer(transform);

		sat_destroy(sat[0]);
		sat_destroy(sat[1]);
		sat_destroy(fa_img);

		destroy();

	return ret;
}

