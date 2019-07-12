// This file contains the methods for the parse.h class

#include <string>
#include <iostream>
#include <map>
#include <cstdlib>
#include <cstring>
#include <cstdio>

using namespace std;

#include <layout.h>
#include <parse.h>

void parse::print_syntax( const char *error_string )
{
  cout << endl << "Error: " << error_string << endl;
  cout << endl << "Layout" << endl
	   <<     "------" << endl
	   << "S. Martin" << endl
	   << "Version " << VERSION << endl << endl
	   << "This program provides a parallel adaptation of a force directed" << endl
	   << "graph layout algorithm for use with large datasets." << endl << endl
	   << "Usage: layout [options] root_file" << endl << endl
	   << "root_file -- the root name of the file being processed." << endl << endl
	   << "INPUT" << endl
	   << "-----" << endl
	   << "root_file.int -- the input file containing the graph to draw using layout." << endl
	   << "  The .int file must have the suffix \".int\" and each line of .int file" << endl
	   << "  should have the form" << endl
	   << "\tnode_id <tab> node_id <tab> weight" << endl
	   << "  where node_id's are integers in sequence starting from 0, and" << endl
	   << "  weight is a float > 0." << endl << endl
	   << "OUTPUT" << endl
	   << "------" << endl
	   << "root_file.icoord -- the resulting output file, containing an ordination" << endl
	   << "  of the graph.  The .icoord file will have the suffix \".icoord\" and" << endl
	   << "  each line of the .icoord file will be of the form" << endl
	   << "\tnode_id <tab> x-coord <tab> y-coord" << endl << endl
	   << "Options:" << endl << endl
	   << "\t-s {int>=0} random seed (default value is 0)" << endl
	   << "\t-c {real[0,1]} edge cutting (default 32/40 = .8)" << endl
	   << "\t   (old max was 39/40 = .975)" << endl
	   << "\t-p input parameters from .parms file" << endl
	   << "\t-r {real[0,1]} input coordinates from .real file" << endl
	   << "\t   (hold fixed until fraction of optimization schedule reached)" << endl
	   << "\t-i {int>=0} intermediate output interval (default 0: no output)" << endl
	   << "\t-e output .iedges file (same prefix as .coord file)" << endl << endl;
 
  #ifdef MUSE_MPI
    MPI_Abort ( MPI_COMM_WORLD, 1 );
  #else
    exit (1);
  #endif
}

parse::parse ( int argc, char** argv)
{
  map<string,string> m;

  // make sure there is at least one argument  
  if ( argc < 2)
	print_syntax ( "not enough arguments!" );
  
  // make sure coord_file ends in ".coord"
  parms_file = real_file = sim_file = coord_file = argv[argc-1];
  parms_file = parms_file + ".parms";
  real_file = real_file + ".real";
  sim_file = sim_file + ".int";
  coord_file = coord_file + ".icoord";
  
  char error_string[200];
  sprintf ( error_string, "%s %d %s", "root file name cannot be longer than", MAX_FILE_NAME-7,
				   "characters.");
  if ( coord_file.length() > MAX_FILE_NAME )
	print_syntax ( error_string );
	
  // echo sim_file and coord_file
  cout << "Using " << sim_file << " for .int file, and " << coord_file << " for .icoord file." << endl;
  
  // set defaults
  rand_seed = 0;
  //edge_cut = 32.0/39.0; // (old default)
  edge_cut = 32.0/40.0;
  int_out = 0;
  edges_out = 0;
  parms_in = 0;
  real_in = -1.0;

  // now check for optional arguments
  string arg;
  for( int i = 1; i<argc-1; i++ )
  {
	arg = argv[i];

	// check for random seed
    if ( arg == "-s" )
	{
		i++;
		if ( i >= (argc-1) )
			print_syntax ( "-s flag has no argument." );
		else
		{
			rand_seed = atoi ( argv[i] );
			if ( rand_seed < 0 )
				print_syntax ( "random seed must be >= 0." );
		}
	}
	// check for edge cutting
	else if ( arg == "-c" )
	{
		i++;
		if ( i >= (argc-1) )
			print_syntax ( "-c flag has no argument." );
		else
		{
			edge_cut = atof ( argv[i] );
			if ( (edge_cut < 0) || (edge_cut > 1) )
				print_syntax ( "edge cut must be between 0 and 1." );
		}
	}		
	// check for intermediate output
	else if ( arg == "-i" )
	{
		i++;
		if ( i >= (argc-1) )
			print_syntax ( "-i flag has no argument." );
		else
		{
			int_out = atoi ( argv[i] );
			if ( int_out < 0 )
				print_syntax ( "intermediate output must be >= 0." );
		}
	}
	// check for .real input
	else if ( arg == "-r" )
	{
		i++;
		if ( i >= (argc-1) )
			print_syntax ( "-r flag has no argument." );
		else
		{
			real_in = atof ( argv[i] );
			if ( (real_in < 0) || (real_in > 1) )
				print_syntax ( "real iteration fraction must be from 0 to 1." );
		}
	}
	else if ( arg == "-e" )
		edges_out = 1;
	else if ( arg == "-p" )
		parms_in = 1;
	else
		print_syntax ( "unrecongized option!" );
  }
  
  if ( parms_in )
    cout << "Using " << parms_file << " for .parms file." << endl;
	
  if ( real_in >= 0 )
    cout << "Using " << real_file << " for .real file." << endl;
	
  // echo arguments input or default
  cout << "Using random seed = " << rand_seed << endl
       << "      edge_cutting = " << edge_cut << endl
       << "      intermediate output = " << int_out << endl
       << "      output .iedges file = " << edges_out << endl;
  if ( real_in >= 0 )
	cout << "      holding .real fixed until iterations = " << real_in << endl;

}
