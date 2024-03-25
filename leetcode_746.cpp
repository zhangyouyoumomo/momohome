class Solution {
public:
    int min(int a,int b){
        if(a<b)return a;
        else return b;
    }
    int minCostClimbingStairs(vector<int>& cost) {
        int n=cost.size();
        int dp[n];
        dp[0]=cost[0];
        dp[1]=cost[1];//must not from 0 to 1,start from 1 direct 
        for(int i=2;i<n;i++){
        dp[i]=cost[i]+min(dp[i-1],dp[i-2]);
         }
         return min(dp[n-1],dp[n-2]);
    }
};