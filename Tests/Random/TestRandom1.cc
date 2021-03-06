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


void Gen (int count, RandomStream *r, const char *title)
{
    // Create and initialise buckets for a bar graph
    int i;
    int bucket[100];
    for (i=0; i<100; i++) bucket[i]=0;

    // generate numbers and update bucket counts
    for (i=0; i<count; i++) {
	double result = (*r)();
	int idx = result < 0.0 ? 0 : (result > 100.0) ? 100 : (int) result;
	bucket[idx]++;
    }

    // output a title followed by the bucket counts
    std::cout << std::endl << '"' << title << '"' << std::endl;
    for (i=0; i<100; i++)
	std::cout << i << ".0 " << bucket[i] << std::endl;

    std::cerr << "Chi-Square error measure: " << r->Error() << std::endl;

    delete r;
}

int main (int argc, char **argv)
{
    extern char *optarg;
    extern int optind;
    int c;
    int count=10000;
    int errflg = 0;
    int uniform=0, normal=0, exponential=0, hyperexponential=0, erlang=0;
    int skip=0;

    while ((c = getopt(argc, argv, "s:c:unxhe")) != -1)
        switch(c) {
        case 'c': count = atoi(optarg); break;
        case 'u': uniform++; break;
        case 'n': normal++; break;
        case 'x': exponential++; break;
        case 'h': hyperexponential++; break;
        case 'e': erlang++; break;
        case 's': skip = atoi(optarg); break;

        case '?':
        default: errflg++;
        }

    // ASSERT: argc-optind == number of arguments remaining
    //         argv[optind] == first non-flag argument

    if (errflg || (argc-optind)) {
        std::cerr << "usage: " << argv[0] << "[-c <number>][-u][-n][-x][-h][-e]" << std::endl;

        std::cerr << "\t-c 99\t\tgenerate 99 points for each distribution" << std::endl;
        std::cerr << "\t-u\t\toutput a data set for a uniform distribution" << std::endl;
        std::cerr << "\t-n\t\toutput a data set for a normal distribution" << std::endl;
        std::cerr << "\t-x\t\toutput a data set for an exponential distribution" << std::endl;
        std::cerr << "\t-h\t\toutput a data set for a hyperexponential distribution" << std::endl;
        std::cerr << "\t-e\t\toutput a data set for an erlang distribution" << std::endl;
        return 2;
    }

    // output a normal distribution by default
    if (!uniform && !normal && !exponential && !hyperexponential && !erlang)
	normal++;

    std::cout << "TitleText: Random Number Distributions" 
	 << " (" << count << " numbers/distribution)" 
	 << std::endl;

    for (int q=0; q<skip; q++) delete new UniformStream(0.0, 100.0);

    if (uniform) Gen (count, new UniformStream (0.0, 100.0), "Uniform");
    if (normal) Gen (count, new NormalStream (50.0, 15.0), "Normal");
    if (exponential) Gen (count, new ExponentialStream(50.0), "Exponential");
    if (hyperexponential) Gen (count, new HyperExponentialStream(50.0, 55.0), "HyperExponential");
    if (erlang) Gen (count, new ErlangStream(50.0, 15.0), "Erlang");
    
    return 0;
}
