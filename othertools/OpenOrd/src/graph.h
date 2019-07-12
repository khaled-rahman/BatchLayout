// The graph class contains the methods necessary to draw the
// graph.  It calls on the density server class to obtain
// position and density information

#include <DensityGrid.h>

// layout schedule information
struct layout_schedule {
	int iterations;
	float temperature;
	float attraction;
	float damping_mult;
	time_t time_elapsed;
};

class graph {

public:

    // Methods
	void init_parms ( int rand_seed, float edge_cut, float real_parm );
    void read_parms ( char *parms_file );
	void read_real ( char *real_file );
	void scan_int ( char *filename );
	void read_int ( char *file_name );
	void draw_graph ( int int_out, char *coord_file );
	void write_coord ( const char *file_name );
	void write_sim ( const char *file_name );
	float get_tot_energy ( );
	
	// Con/Decon
	graph( int proc_id, int tot_procs, char *int_file );
		~graph( ) { }
	
private:

	// Methods
	int ReCompute ( );
	void update_nodes ( );
	float Compute_Node_Energy ( int node_ind );
	void Solve_Analytic ( int node_ind, float &pos_x, float &pos_y );
	void get_positions ( vector<int> &node_indices, float return_positions[2*MAX_PROCS] );
	void update_density ( vector<int> &node_indices, float old_positions[2*MAX_PROCS],
			      float new_positions[2*MAX_PROCS] );
	void update_node_pos ( int node_ind,
			       float old_positions[2*MAX_PROCS],
			       float new_positions[2*MAX_PROCS] );
								  
	// MPI information
	int myid, num_procs;
	
	// graph decomposition information
	int num_nodes;					// number of nodes in graph
	float highest_sim;				// highest sim for normalization
	map <int, int> id_catalog;		// id_catalog[file id] = internal id
	map <int, map <int, float> > neighbors;		// neighbors of nodes on this proc.
	
	// graph layout information
	vector<Node> positions;  
	DensityGrid density_server;
  
	// original VxOrd information
	int STAGE, iterations;
	float temperature, attraction, damping_mult;
	float min_edges, CUT_END, cut_length_end, cut_off_length, cut_rate;
	bool first_add, fine_first_add, fineDensity;  
	
	// scheduling variables
	layout_schedule liquid;
	layout_schedule expansion;
	layout_schedule cooldown;
	layout_schedule crunch;
	layout_schedule simmer;
	
	// timing statistics
        time_t start_time, stop_time;
	
	// online clustering information
	int real_iterations;    // number of iterations to hold .real input fixed
	int tot_iterations;
	bool real_fixed;
};
