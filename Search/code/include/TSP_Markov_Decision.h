
// Jump to a new state by randomly generating a solution
void Jump_To_Random_State()
{
	Generate_Initial_Solution(); 	
}

Distance_Type Markov_Decision_Process(int Inst_Index)
{
    MCTS_Init(Inst_Index);
    Generate_Initial_Solution();

    Distance_Type best = Greedy_Rollout_2Opt(Start_City);
    Store_Best_Solution();

    for(int i=0;i<3;++i)
    {
        Jump_To_Random_State();
        Distance_Type dist = Greedy_Rollout_2Opt(Start_City);
        if(dist < best)
        {
            best = dist;
            Store_Best_Solution();
        }
    }

    Restore_Best_Solution();
    return best;
}



