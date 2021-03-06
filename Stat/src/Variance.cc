/*
 * Copyright 1990-2008, Mark Little, University of Newcastle upon Tyne
 * and others contributors as indicated 
 * by the @authors tag. All rights reserved. 
 * See the copyright.txt in the distribution for a
 * full listing of individual contributors. 
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU Lesser General Public License, v. 2.1.
 * This program is distributed in the hope that it will be useful, but WITHOUT A 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License,
 * v.2.1 along with this distribution; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, 
 * MA  02110-1301, USA.
 * 
 * (C) 1990-2008,
 */
 /*
 * Copyright (C) 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


#include <errno.h>
#include <iostream>
#include <fstream>
#include <iomanip>

#ifndef VARIANCE_H_
#  include <Stat/Variance.h>
#endif


Variance::Variance () { Variance::reset(); }

Variance::~Variance () {}

void Variance::reset ()
{
    _sqr = 0.0;
    Mean::reset();
}

void Variance::setValue (double value)
{
    _sqr += value*value;
    Mean::setValue(value);
}

double Variance::variance () const
{
    return ((_Number > 1) ? ((_sqr - ((_Sum * _Sum) / _Number)) / (_Number -1)) : 0.0);
}

double Variance::confidence (double)
{
    std::cerr << "Variance::confidence not implemented yet." << std::endl;
    return 0.0;
}

Boolean Variance::saveState (std::ofstream& oFile) const
{
    if (!oFile)
	return FALSE;

    oFile << " " << _sqr;
    return Mean::saveState(oFile);
}

Boolean Variance::saveState (const char* fileName) const
{
    std::ofstream oFile(fileName, std::ios::out);
    Boolean result;
    
    if (!oFile)
    {
        std::cerr << "Variance::saveState - error "
		     << errno << " for file " << fileName << std::endl;
	return FALSE;
    }
    
    result = saveState(oFile);
    oFile.close();
    return result;
}

Boolean Variance::restoreState (std::ifstream& iFile)
{
    if (!iFile)
	return FALSE;
    
    iFile >> _sqr;

    return Mean::restoreState(iFile);
}

Boolean Variance::restoreState (const char* fileName)
{
    std::ifstream iFile(fileName, std::ios::in);
    Boolean result;
    
    if (!iFile)
    {
        std::cerr << "Variance::restoreState - error "
		     << errno << " for file " << fileName << std::endl;
	return FALSE;
    }
    
    result = restoreState(iFile);
    iFile.close();
    return result;
}

std::ostream& Variance::print (std::ostream& strm) const
{
    strm << "\nVariance          : " << variance() << std::endl;
    strm << "Standard Deviation: " << stdDev() << std::endl;

    return Mean::print(strm);
}

#ifdef NO_INLINES
#  define VARIANCE_CC_
#  include <Stat/Variance.n>
#  undef VARIANCE_CC_
#endif
