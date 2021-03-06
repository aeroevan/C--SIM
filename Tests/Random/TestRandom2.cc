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

/*
#ifdef __GNUG__
#  include <GetOpt.h>
#endif
*/

#include <stdlib.h>	// to get getopt and atoi
#include <iostream>
#include <unistd.h>

#include <ClassLib/Random.h>

using namespace std;

void Gen (int count, RandomStream *r)
{
    for (int i = 0; i < count/2; i++)
	cout << (*r)() << " " << (*r)() << endl;

    delete r;
}

void GenDraw (int count, Draw* d)
{
    for (int i = 0; i < count; i++)
	cout << (*d)() << endl;

    delete d;
}

int main (int argc, char **argv)
{
    extern char *optarg;
    extern int optind;
    int c;
    int count=10000;
    int errflg = 0;
    int uniform=0, normal=0, exponential=0, hyperexponential=0, erlang=0;
    int draw = 0, skip=0;
    Boolean fullcoverage = FALSE;

    while ((c = getopt(argc, argv, "s:c:dunxhep")) != -1)
        switch(c) {
        case 'c': count = atoi(optarg); break;
        case 'u': uniform++; break;
	case 'd': draw++; break;
        case 'n': normal++; break;
        case 'x': exponential++; break;
        case 'h': hyperexponential++; break;
        case 'e': erlang++; break;
        case 's': skip = atoi(optarg); break;
        case 'p': fullcoverage = TRUE; break;

        case '?':
        default: errflg++;
        }

    // ASSERT: argc-optind == number of arguments remaining
    //         argv[optind] == first non-flag argument

    if (errflg || (argc-optind)) {
        cerr << "usage: " << argv[0] << "[-c <number>][-u][-n][-x][-h][-e][-d]" << endl;

        cerr << "\t-c 99\t\tgenerate 99 points for each distribution" << endl;
        cerr << "\t-u\t\toutput a data set for a uniform distribution" << endl;
        cerr << "\t-n\t\toutput a data set for a normal distribution" << endl;
        cerr << "\t-x\t\toutput a data set for an exponential distribution" << endl;
        cerr << "\t-h\t\toutput a data set for a hyperexponential distribution" << endl;
        cerr << "\t-e\t\toutput a data set for an erlang distribution" << endl;
	cerr << "\t-d\t\toutput a data set for a draw distribution" << endl;
        return 2;
    }

    if (!fullcoverage)
    {
	// output a normal distribution by default
	if (!uniform && !normal && !exponential && !hyperexponential && !erlang && !draw)
	    normal++;

	cout << "TitleText: Random Number Distributions" 
	     << " (" << count << " numbers/distribution)" 
	     << endl;

	for (int q=0; q<skip; q++) delete new UniformStream(0.0, 100.0);

	if (uniform) Gen (count, new UniformStream (0.0, 100.0));
	if (normal) Gen (count, new NormalStream (50.0, 15.0));
	if (exponential) Gen (count, new ExponentialStream(50.0));
	if (hyperexponential) Gen (count, new HyperExponentialStream(50.0, 55.0));
	if (erlang) Gen (count, new ErlangStream(50.0, 15.0));
	if (draw) GenDraw(count, new Draw(0.5));
    }
    else  // for purecoverage
    {
	Gen (count, new UniformStream (0.0, 100.0));
	Gen (count, new NormalStream (50.0, 15.0));
	Gen (count, new ExponentialStream(50.0));
	Gen (count, new HyperExponentialStream(50.0, 55.0));
	Gen (count, new ErlangStream(50.0, 15.0));
	GenDraw (count, new Draw(0.5));
    }
    
    return 0;
}
