
#include "process.h"

int process(char * sat_dname, char * result_fname, char * shp_aoi_fname, char * shp_forest_fname, char ** svm_fname)
{
	try;
	
		int ret = 0;
		unsigned v, u, t, pixel_index, height, width;
		uint8_t index, * fa_buf, ** sat_buf;
		double vec[DIM];
		s_sat * sat = NULL, * fa_img = NULL;
		s_svm * svm[2] = { NULL, NULL };

		my_printf("Begin");

		// ############################################################################ 
		// Загрузка драйверов форматов, загрузка спутникового снимка, загрузка SVM

		init();

		throw_null((sat = sat_load(sat_dname)));

		for(v = 0; v < 2; v++)
			throw_null((svm[v] = svm_load(svm_fname[v])));

		// ############################################################################ 
		// Расчет aoi

		throw_null((fa_img = sat_aoi(sat, shp_aoi_fname, shp_forest_fname)));

		// ############################################################################ 
		// Обработка спутникового снимка

		height = sat->height;
		width = sat->width;
		sat_buf = sat->pixel;
		fa_buf = fa_img->pixel[0];

		for(v = 0; v < height; v++)
		{
			my_printf("Row %u from %u ( %lf %% )", v + 1, height, (100.0 * (v + 1)) / height);

			for(u = 0; u < width; u++)
			{
				pixel_index = v * width + u;

				if(fa_buf[pixel_index] > 0)
				{
					for(t = 0; t < DIM; t++)
						vec[t] = sat_buf[t][pixel_index];

					if(svm_run(svm[0], vec) > 0)
						index = 3;
					else if(svm_run(svm[1], vec) > 0)
						index = 2;
					else
						index = 1;
				}
				else
					index = 0;

				fa_buf[pixel_index] = index;
			}
		}

		// ############################################################################ 
		// Сохранение результирующего изображения

		throw(sat_save_ch(fa_img, result_fname, 0));

		my_printf("Success");

	catch;

		ret = -1;

		my_printf("Fail");

	finally;

		for(v = 0; v < 2; v++)
			svm_destroy(svm[v]);

		sat_destroy(sat);
		sat_destroy(fa_img);

		destroy();

	return ret;
}

