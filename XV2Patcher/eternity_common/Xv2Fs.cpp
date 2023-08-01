#include "Xv2Fs.h"

// They are ordered from bigger to lower priority
static const std::vector<std::string> cpk_files =
{
    "data_d0_stv.cpk",

    "movie_p4.cpk",
    "movie_p2.cpk",
    "movie.cpk",
    "movie0.cpk",
    "data2.cpk",
    "data1.cpk",
    "data0.cpk",
    "data.cpk"
};

Xv2Fs::Xv2Fs(const std::string &loose_files_root, bool read_only) : CriFs(loose_files_root, read_only)
{
    int count = 0;

    for (const std::string &cpk : cpk_files)
    {
        if (AddCpk(this->loose_files_root + "cpk/" + cpk))
            count++;
    }

    //DPRINTF("Added: %d cpk.\n", count);
    BuildDirList();
}
