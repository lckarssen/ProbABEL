/**
 * \file   mlinfo.cpp
 * \author The GenABEL team
 *
 * \brief Contains the class implementation of the mlinfo class.
 */
/*
 *
 * Copyright (C) 2009--2016 Various members of the GenABEL team. See
 * the SVN commit logs for more details.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 *
 */


#include <sstream>
#include <fstream>
#include <iostream>
#include "mlinfo.h"

using std::cout;
using std::cerr;
using std::endl;


/**
 * \brief Constructor that reads SNP information from an
 * <tt>.mlinfo</tt> or <tt>.info</tt> file generated by the imputation
 * software.
 *
 * \param filename Name of the mlinfo file
 * \param mapname Name of the map file
 * \param flipMAF Indicates whether alleles should be flipped based on
 * MAF (\sa cmdvars::getFlipMAF and cmdvars::flipMAF)
 */
mlinfo::mlinfo(const char * filename,
               const char * mapname,
               const bool flipMAF)
{
    nsnps = 0;
    std::string line;
    std::string tmp;
    std::ifstream infile(filename);

    if (!infile.is_open())
    {
        cerr << "mlinfo: cannot open info file "
                  << filename << endl;
        exit(1);
    }

    /* Read the header and discard it */
    std::getline(infile, tmp);

    /* Read the remaining lines from the info file */
    while (std::getline(infile, line))
    {
        if (infile.eof()) break;

        std::stringstream line_stream(line);
        line_stream >> tmp;
        name.push_back(tmp);
        line_stream >> tmp;
        A1.push_back(tmp);
        line_stream >> tmp;
        A2.push_back(tmp);
        line_stream >> tmp;
        Freq1.push_back(std::stod(tmp));
        line_stream >> tmp;
        MAF.push_back(std::stod(tmp));
        line_stream >> tmp;
        Quality.push_back(std::stod(tmp));
        line_stream >> tmp;
        Rsq.push_back(std::stod(tmp));
        map.push_back("-999");
        nsnps++;
    }

    cout << "Number of SNPs = " << nsnps << endl;

    infile.close();

    if (flipMAF)
    {
        allelesFlipped = std::vector<bool>(nsnps, false);
    }

    if (mapname != NULL)
    {
        std::ifstream mapfile_in(mapname);

        if (!mapfile_in.is_open())
        {
            cerr << "mlinfo: cannot open map file "
                      << mapname << endl;
            exit(1);
        }

        /* Read the header line and discard it */
        std::getline(mapfile_in, line);

        for (int i = 0; i < nsnps; i++)
        {
            std::getline(mapfile_in, line);

            if (mapfile_in.eof())
            {
                cerr << "mlinfo: reached end of map file; read "
                     << i << " SNPs instead of " << nsnps << endl;
            }

            std::stringstream line_stream(line);
            line_stream >> tmp >> map[i];
        }

        mapfile_in.close();
    }
}


/**
 * \brief Destructor.
 */
mlinfo::~mlinfo()
{
}
