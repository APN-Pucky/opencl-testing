#include "runner.h"
#include "mpi.h"



/*
template<typename T>
void Runner::add_argument(T& t ,size_t st) 
{
    this->pointers.push_back((void*) &t);
    this->sizes.push_back(st);
}
template<typename T>
void Runner::add_argument(T& t) 
{
    this->pointers.push_back((void*) &t);
    this->sizes.push_back(sizeof(T));
}
*/
//const int num_t = omp_get_max_threads();
int global_ids_a[100];
int* global_ids = &global_ids_a[0];
/*
 * Loads cl programs in directories cl/ 
 */
cl_program load_cl_programs(cl_context context) {
    std::vector<std::string> filenames;
    std::string path1 = "cl";
    std::string path2 = "src/cl";
    boost::filesystem::path full_path(boost::filesystem::current_path());
    debug_printf("Current path is : %s" , full_path.c_str());
    if( ! boost::filesystem::is_directory(path1) ) {
            path1 = path2;
    }
    for (boost::filesystem::directory_entry& entry : boost::filesystem::directory_iterator(path1))  {
        filenames.push_back(entry.path().string());
    }
    std::sort(filenames.begin(),filenames.end());


	int n =  filenames.size();
	std::vector<std::string> files(n);
    const char** programBuffer = (const char**) malloc(n);
    size_t* programSize = ( size_t*) malloc(n);
	for(int i=0; i < n; ++i) {
        std::ifstream inFile;
		inFile.open(filenames[i]); //open the input file
    	std::stringstream strStream;
    	strStream << inFile.rdbuf(); //read the file
    	std::string str = strStream.str(); //str holds the content of the file
		files.push_back(str);
        programBuffer[i] = files.back().c_str();
        programSize[i] = files.back().length();
	}
    cl_int err;
    cl_program tmp = clCreateProgramWithSource(context, n,
            programBuffer, programSize, &err);
    if(err) printf("clCreateProgramWithSource Error %d\n",err);
    return tmp;
}