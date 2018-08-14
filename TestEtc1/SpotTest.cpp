
#include <iostream>
#include "spot.hpp" 
#include <windows.h>

#if defined(_MSC_VER) || defined(__ANDROID_API__)
#define USE_OMP
#endif
#ifndef USE_OMP
#include <chrono>
auto const epoch = std::chrono::steady_clock::now();
double now() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - epoch).count() / 1000.0;
};
#else
#include <omp.h>
auto const epoch = omp_get_wtime();
double now() {
	return omp_get_wtime() - epoch;
};
#endif
template<typename FN>
double bench(const FN &fn) {
	auto took = -now();
	return (fn(), took + now());
}
#include <chrono>
#include <thread>
void sleep(double secs) {
	std::chrono::microseconds duration((int)(secs * 1000000));
	std::this_thread::sleep_for(duration);
}

int main(int argc, char **argv) {

	if (argc < 2) {
		std::cout << "用法: " << argv[0] << " image [image [...]]" << std::endl;
		return -1;
	}
	std::string szfile;
	std::string savefile;
	spot::image base;
	spot::image light;

	for (int i = 1; i < argc; ++i) {
		szfile = argv[i];
		std::cout << "[加载中] " << argv[i];
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath_s(szfile.c_str(), drive, dir, fname, ext);
		savefile += drive;
		savefile += dir;
		savefile += fname;
		savefile += "_";

		try {
			double loading1 = bench([&]{
				base.load(spot::texture(szfile.c_str()));
			});
			std::cout << " 加载耗时: " << int(loading1 * 1000) << " 毫秒" << std::endl;

			std::string lastfile = savefile;
			lastfile += "_demo.jpg";
			base.save_as_jpg(lastfile);
			ShellExecuteA(NULL, "open", lastfile.c_str(), NULL, NULL, SW_SHOW);

		}
		catch (...) {
			std::cout << "\r[失败]" << std::endl;
		}
	}

#if 0 
	spot::image base;
	try {
		double loading1 = bench([&]{

			base.load(spot::texture("d:\\big.jpg"));
		});
		std::cout << " 加载耗时: " << int(loading1 * 1000) << " 毫秒" << std::endl;


		std::cout << (base.loaded() ? "\r[ 完成 ]" : "\r[失败]") << std::endl;


	}
	catch (...) {
		std::cout << "\r[失败]" << std::endl;
	}
#endif
	getchar();
	return 0;
	}
