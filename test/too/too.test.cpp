#include "catch.hpp"
#include "state.h"
#include "tyrant.h"
#include "../../src/tuo/h/ocl_load.h"
#include "runner.h"
#include "tuo.h"
#include "../../src/tuo/h/tyrant_optimize.h"
TEST_CASE( "too single", "[single]" ) {
    //int all_cards_size;
    //CLCard* all_cards = load_all_cards(&all_cards_size);
    int all_cards_size = 3;
    int all_cards_size_ = 3;
    CLCard all_cards[all_cards_size];
    load_all_cards(&all_cards_size_);
    printf("Loaded %d cards", all_cards_size);
    //const unsigned int size_card=10+4*3*size_skillspec+Skill::num_skills*2+2;
    const unsigned int size_all_cards = all_cards_size*size_card;
    //const unsigned int size_deck=10;
    const unsigned int sims = 1;
    const unsigned int N = sims;
    const unsigned long seed = 5;

    int mydeck[size_deck];
    int enemydeck[size_deck];
    int* allcards = (int*)all_cards;
    copy_card(&all_cards[0] , tuo::all_cards.cards_by_id[1]);
    copy_card(&all_cards[1] , tuo::all_cards.cards_by_id[34]);
    copy_card(&all_cards[2] , tuo::all_cards.cards_by_id[1000]);
    //for( int i =0; i < size_all_cards;++i) {
    //    allcards[i] = 0;
    //}
    //allcards[0] = 2;
    //allcards[1] = 2;
    //allcards[2] = 2;
    //allcards[4] = 2;
    //allcards[5] = 2;

    //allcards[size_card +0] = 1;
    //allcards[size_card +1] = 1;
    //allcards[size_card +2] = 1;
    //allcards[size_card +4] = 1;
    //allcards[size_card +5] = 1;

    int results[N];
    mydeck[0] = 1000;
    enemydeck[0] = 1000;
    for(int i =2; i  < size_deck ;++i) {
	    mydeck[i] = 34;
	    enemydeck[i] = 1;
    }
    for(int i =0; i  < N; ++i) {
	    results[i] = -2;
    }

    auto r = Runner<int,int*,int,int*,int,int*,int,int*,int,int>("simulate",simulate,N,size_all_cards,allcards,size_deck,mydeck,size_deck,enemydeck,N,results,N,seed);
    r.set_mem<1>(CL_MEM_READ_ONLY,size_all_cards,false);
    r.set_mem<3>(CL_MEM_READ_ONLY,size_deck,false);
    r.set_mem<5>(CL_MEM_READ_ONLY,size_deck,false);
    r.set_mem<7>(CL_MEM_WRITE_ONLY,N,true);

    r.run_noparallel();
    //r.run_openmp();
    //r.run_hybrid();
    //r.run_cpu();
    //r.run_mpi();

    for (int i = 0; i  < N; ++i) printf("%d",results[i]);


	    

    //r.run_cpu();
    //printf("\nsingle run %d\n",results[0]);

    REQUIRE (0 >= 0);
}
TEST_CASE( "too mpi", "[mpclhy]" ) {
    //int all_cards_size;
    //CLCard* all_cards = load_all_cards(&all_cards_size);
    int all_cards_size = 3;
    int all_cards_size_ = 3;
    CLCard all_cards[all_cards_size];
    load_all_cards(&all_cards_size_);
    printf("Loaded %d cards", all_cards_size);
    //const unsigned int size_card=10+4*3*size_skillspec+Skill::num_skills*2+2;
    const unsigned int size_all_cards = all_cards_size*size_card;
    //const unsigned int size_deck=10;
    const unsigned int sims = 1024000;
    const unsigned int N = sims;
    const unsigned long seed = 5;

    int mydeck[size_deck];
    int enemydeck[size_deck];
    int* allcards = (int*)all_cards;
    copy_card(&all_cards[0] , tuo::all_cards.cards_by_id[1]);
    copy_card(&all_cards[1] , tuo::all_cards.cards_by_id[34]);
    copy_card(&all_cards[2] , tuo::all_cards.cards_by_id[1000]);
    //for( int i =0; i < size_all_cards;++i) {
    //    allcards[i] = 0;
    //}
    //allcards[0] = 2;
    //allcards[1] = 2;
    //allcards[2] = 2;
    //allcards[4] = 2;
    //allcards[5] = 2;

    //allcards[size_card +0] = 1;
    //allcards[size_card +1] = 1;
    //allcards[size_card +2] = 1;
    //allcards[size_card +4] = 1;
    //allcards[size_card +5] = 1;

    int results[N];
    mydeck[0] = 1000;
    enemydeck[0] = 1000;
    for(int i =2; i  < size_deck ;++i) {
	    mydeck[i] = 34;
	    enemydeck[i] = 1;
    }
    for(int i =0; i  < N; ++i) {
	    results[i] = -2;
    }

    auto r = Runner<int,int*,int,int*,int,int*,int,int*,int,int>("simulate",simulate,N,size_all_cards,allcards,size_deck,mydeck,size_deck,enemydeck,N,results,N,seed);
    r.set_mem<1>(CL_MEM_READ_ONLY,size_all_cards,false);
    r.set_mem<3>(CL_MEM_READ_ONLY,size_deck,false);
    r.set_mem<5>(CL_MEM_READ_ONLY,size_deck,false);
    r.set_mem<7>(CL_MEM_WRITE_ONLY,N,true);

    //r.run_noparallel();
    //r.run_openmp();
    //r.run_hybrid();
    auto start = std::chrono::high_resolution_clock::now();
    r.run_mpi();

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "\nMPI == > Elapsed time: " << elapsed.count() << " s\n";
    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
    printf("Hello world from processor %s, rank %d out of %d processors",
           processor_name, world_rank, world_size);
    for (int i = 0; i  < N/1000 ; ++i) printf("%d",results[i]);
    printf("\n");

    //r.run_cpu();
    //printf("\nsingle run %d\n",results[0]);

    if(world_rank==0) {
        for(int i =0 ; i  < N;++i) {
            REQUIRE (results[i] == -1);
        }
    }

}
TEST_CASE( "too mpclhy", "[mpclhy]" ) {
    //int all_cards_size;
    //CLCard* all_cards = load_all_cards(&all_cards_size);
    int all_cards_size = 3;
    int all_cards_size_ = 3;
    CLCard all_cards[all_cards_size];
    load_all_cards(&all_cards_size_);
    printf("Loaded %d cards", all_cards_size);
    //const unsigned int size_card=10+4*3*size_skillspec+Skill::num_skills*2+2;
    const unsigned int size_all_cards = all_cards_size*size_card;
    //const unsigned int size_deck=10;
    const unsigned int sims = 1024000;
    const unsigned int N = sims;
    const unsigned long seed = 5;

    int mydeck[size_deck];
    int enemydeck[size_deck];
    int* allcards = (int*)all_cards;
    copy_card(&all_cards[0] , tuo::all_cards.cards_by_id[1]);
    copy_card(&all_cards[1] , tuo::all_cards.cards_by_id[34]);
    copy_card(&all_cards[2] , tuo::all_cards.cards_by_id[1000]);
    //for( int i =0; i < size_all_cards;++i) {
    //    allcards[i] = 0;
    //}
    //allcards[0] = 2;
    //allcards[1] = 2;
    //allcards[2] = 2;
    //allcards[4] = 2;
    //allcards[5] = 2;

    //allcards[size_card +0] = 1;
    //allcards[size_card +1] = 1;
    //allcards[size_card +2] = 1;
    //allcards[size_card +4] = 1;
    //allcards[size_card +5] = 1;

    int results[N];
    mydeck[0] = 1000;
    enemydeck[0] = 1000;
    for(int i =2; i  < size_deck ;++i) {
	    mydeck[i] = 34;
	    enemydeck[i] = 1;
    }
    for(int i =0; i  < N; ++i) {
	    results[i] = -2;
    }

    auto r = Runner<int,int*,int,int*,int,int*,int,int*,int,int>("simulate",simulate,N,size_all_cards,allcards,size_deck,mydeck,size_deck,enemydeck,N,results,N,seed);
    r.set_mem<1>(CL_MEM_READ_ONLY,size_all_cards,false);
    r.set_mem<3>(CL_MEM_READ_ONLY,size_deck,false);
    r.set_mem<5>(CL_MEM_READ_ONLY,size_deck,false);
    r.set_mem<7>(CL_MEM_WRITE_ONLY,N,true);

    //r.run_noparallel();
    //r.run_openmp();
    //r.run_hybrid();
    //r.run_cpu();

    auto start = std::chrono::high_resolution_clock::now();
    r.run_openmp();
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "\nOpenMP == > Elapsed time: " << elapsed.count() << " s\n";
    for (int i = 0; i  < N/1000 ; ++i) printf("%d",results[i]);


    start = std::chrono::high_resolution_clock::now();
    r.run_hybrid();
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "\nHybrid == > Elapsed time: " << elapsed.count() << " s\n";
    for (int i = 0; i  < N/1000 ; ++i) printf("%d",results[i]);

    start = std::chrono::high_resolution_clock::now();
    //r.run_noparallel();
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "\nNoParallel == > Elapsed time: " << elapsed.count() << " s\n";
    for (int i = 0; i  < N/1000 ; ++i) printf("%d",results[i]);


    start = std::chrono::high_resolution_clock::now();
    r.run_opencl();
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "\nOpenCL == > Elapsed time: " << elapsed.count() << " s\n";
    for (int i = 0; i  < N/1000 ; ++i) printf("%d",results[i]);

	    

    //r.run_cpu();
    //printf("\nsingle run %d\n",results[0]);

    REQUIRE (0 >= 0);
}
/*
    const unsigned int size_all_cards = 2*size_card;
    const unsigned int size_deck=10;
    const unsigned int sims = 1024;
    const unsigned int N = sims;
    const unsigned long seed = 5;

    int mydeck[size_deck];
    int enemydeck[size_deck];
    int allcards[size_all_cards];
    for( int i =0; i < size_all_cards;++i) {
        allcards[i] = 0;
    }
    allcards[0] = 2;
    allcards[1] = 2;
    allcards[2] = 2;
    allcards[4] = 2;
    allcards[5] = 2;

    allcards[size_card +0] = 1;
    allcards[size_card +1] = 1;
    allcards[size_card +2] = 1;
    allcards[size_card +4] = 1;
    allcards[size_card +5] = 1;

    int results[N];

    for(int i =0; i  < size_deck ;++i) {
	    mydeck[i] = 1;
	    enemydeck[i] = 2;
    }
    for(int i =0; i  < N; ++i) {
	    results[i] = -2;
    }

    auto r = Runner<int,int*,int,int*,int,int*,int,int*,int,int>("simulate",simulate,N,size_all_cards,allcards,size_deck,mydeck,size_deck,enemydeck,N,results,N,seed);
    r.set_mem<1>(CL_MEM_READ_ONLY,size_all_cards,false);
    r.set_mem<3>(CL_MEM_READ_ONLY,size_deck,false);
    r.set_mem<5>(CL_MEM_READ_ONLY,size_deck,false);
    r.set_mem<7>(CL_MEM_WRITE_ONLY,N,true);
    //r.run_opencl(N);
    //for (int i = 0; i  < N ; ++i) {
	   // printf("%d",results[i]);
    //}

    global_ids[0] = 0;
    r.run_cpu();
    printf("\nsingle run %d\n",results[0]);
    REQUIRE (results[0] >= 0);




    //simulate(allcards,size_all_cards,mydeck,enemydeck,results,N,seed);
	//printf("\nsingle run %d\n",results[0]);
}
//*/