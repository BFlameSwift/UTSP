
// Jump to a new state by randomly generating a solution
// If the randomly generated tour is infeasible, retry a few times
void Jump_To_Random_State()
{
    int tries = 0;
    do {
        Generate_Initial_Solution();
        tries++;
    } while(!Check_Solution_Feasible() && tries < 5);
    // As a fall back, simply generate again even if still infeasible
    if(!Check_Solution_Feasible())
        Generate_Initial_Solution();
}

// The main Markov Decision Process combining local search and MCTS
Distance_Type Markov_Decision_Process(int Inst_Index)
{
    MCTS_Init(Inst_Index);                      // Initialize MCTS parameters
    Generate_Initial_Solution();                // Initial random tour
    Local_Search_by_2Opt_Move();                // Refine with 2-Opt
    if(!Check_Solution_Feasible())              // Guard against invalid tour
        Jump_To_Random_State();
    MCTS();                                     // Targeted sampling via MCTS

    int jump = 0;
    while(((double)clock()-Current_Instance_Begin_Time) /CLOCKS_PER_SEC < Param_T*Virtual_City_Num)
    {
        if (restart) {
            int old_rec_only = rec_only;
            if (restart_reconly) {
                rec_only = rand() % 2;
                if (rec_only != old_rec_only)
                    Identify_Candidate_Set();
            }
            Max_Depth = 10 + (rand() % 80);
        }

        ++jump;
        Jump_To_Random_State();
        Local_Search_by_2Opt_Move();
        if(!Check_Solution_Feasible())
            continue;
        MCTS();
    }
    std::cout << "jump:" << jump << std::endl;

    Restore_Best_Solution();

    if(Check_Solution_Feasible())
        return Get_Solution_Total_Distance();
    else
        return Inf_Cost;
}



