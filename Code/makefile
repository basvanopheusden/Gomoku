final:
	for data in hvh gen eye learn1 learn2 learn3 tai1 tai2 tur; do echo "mex -f ${MATLAB_ROOT}/bin/mexopts.sh -v -output Gomoku_${data}_final CXXFLAGS=\"\\\$CXXFLAGS -Wall -pthread -Wextra -std=c++11 -O3 -fexpensive-optimizations\" heuristic.cpp loglik_mex.cpp bfs.cpp data_${data}.cpp features.cpp data_struct.cpp; exit" | matlab -nodisplay; done

final_fixed_iters:
	for data in hvh gen eye learn1 learn2 learn3 tai1 tai2 tur; do echo "mex -f ${MATLAB_ROOT}/bin/mexopts.sh -v -output Gomoku_${data}_final CXXFLAGS=\"\\\$CXXFLAGS -Wall -pthread -Wextra -std=c++11 -O3 -fexpensive-optimizations\" heuristic.cpp loglik_mex.cpp bfs_fixed_iters.cpp data_${data}.cpp features.cpp data_struct.cpp; exit" | matlab -nodisplay; done

final_fixed_branch:
	for data in hvh gen eye learn1 learn2 learn3 tai1 tai2 tur; do echo "mex -f ${MATLAB_ROOT}/bin/mexopts.sh -v -output Gomoku_${data}_final CXXFLAGS=\"\\\$CXXFLAGS -Wall -pthread -Wextra -std=c++11 -O3 -fexpensive-optimizations\" heuristic_fixed_branch.cpp loglik_mex.cpp bfs.cpp data_${data}.cpp features.cpp data_struct.cpp; exit" | matlab -nodisplay; done

final_fixed_depth:
	for data in hvh gen eye learn1 learn2 learn3 tai1 tai2 tur; do echo "mex -f ${MATLAB_ROOT}/bin/mexopts.sh -v -output Gomoku_${data}_final CXXFLAGS=\"\\\$CXXFLAGS -Wall -pthread -Wextra -std=c++11 -O3 -fexpensive-optimizations\" heuristic_fixed_depth.cpp loglik_mex.cpp bfs.cpp data_${data}.cpp features.cpp data_struct.cpp; exit" | matlab -nodisplay; done

mcts:
	for data in hvh gen eye learn1 learn2 learn3 tai1 tai2 tur; do echo "mex -f ${MATLAB_ROOT}/bin/mexopts.sh -v -output Gomoku_${data}_final CXXFLAGS=\"\\\$CXXFLAGS -Wall -pthread -Wextra -std=c++11 -O3 -fexpensive-optimizations\" heuristic.cpp loglik_mcts_mex.cpp mcts.cpp data_${data}.cpp features.cpp data_struct.cpp; exit" | matlab -nodisplay; done

final_drop_tile:
	for data in hvh gen eye learn1 learn2 learn3 tai1 tai2 tur; do echo "mex -f ${MATLAB_ROOT}/bin/mexopts.sh -v -output Gomoku_${data}_final CXXFLAGS=\"\\\$CXXFLAGS -Wall -pthread -Wextra -std=c++11 -O3 -fexpensive-optimizations\" heuristic_drop_tile.cpp loglik_mex.cpp bfs.cpp data_${data}.cpp features.cpp data_struct.cpp; exit" | matlab -nodisplay; done

clean: 
	rm Gomoku*.mexa64
