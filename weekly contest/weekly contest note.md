### \1391. Check if There is a Valid Path in a Grid

> Given a *m* x *n* `grid`. Each cell of the `grid` represents a street. The street of `grid[i][j]` can be:
>
> - **1** which means a street connecting the left cell and the right cell.
> - **2** which means a street connecting the upper cell and the lower cell.
> - **3** which means a street connecting the left cell and the lower cell.
> - **4** which means a street connecting the right cell and the lower cell.
> - **5** which means a street connecting the left cell and the upper cell.
> - **6** which means a street connecting the right cell and the upper cell.
>
> <img src="/Users/apple/Library/Application Support/typora-user-images/image-20200401141518967.png" alt="image-20200401141518967" style="zoom: 33%;" />
>
> You will initially start at the street of the upper-left cell `(0,0)`. A valid path in the grid is a path which starts from the upper left cell `(0,0)` and ends at the bottom-right cell `(m - 1, n - 1)`. **The path should only follow the streets**.
>
> **Notice** that you are **not allowed** to change any street.
>
> Return *true* if there is a valid path in the grid or *false* otherwise.
>
> Eg.
>
> ```
> Input: grid = [[1,2,1],[1,2,1]]
> Output: false
> Explanation: As shown you the street at cell (0, 0) is not connected with any street of any other cell and you will get stuck at cell (0, 0)
> ```

我最开始的想法是，把每一种street看作一个九宫格，可以走的地方为1，然后用动态规划的思想来做，虽然testcase都过了，但是动态规划的思想好像无法应用在可以蜿蜒回绕的路径上。总有没考虑到的情况。所以应该用dfs或者bfs来做

#### Bfs

```c++
class Solution {
public:
    void help(int x, int y, vector<vector<bool>>& visit,queue<pair<int,int>>& qq){
        //走过的地方不能再走了
        if(!visit[x][y]){
            visit[x][y] = true;
            qq.push(make_pair(x,y));
        }
    }
    
    
    bool hasValidPath(vector<vector<int>>& g) {
        if(g.size()==0||g[0].size()==0) return false;
        int xsize = g.size();
        int ysize = g[0].size();
        
        queue<pair<int,int>> qq;
        //标记已经走过的地方
        vector<vector<bool>> visit(xsize,vector<bool>(ysize,false));
        
        visit[0][0] = true;
        //学会用pair：make_pair x.first x.second
        qq.push(make_pair(0, 0));
        
        while(!qq.empty()){
            auto t = qq.front();
            qq.pop();
            int x = t.first,y = t.second;
            if(x==xsize-1 && y==ysize-1) return true;
            
          
          //注意这里并不是if else，因为所有的情况都要push进queue里面，完成bfs
            //上
            if((g[x][y]==2 || g[x][y] == 5 || g[x][y] == 6)
               && x-1 >= 0
               && (g[x-1][y] == 2 || g[x-1][y] == 3 || g[x-1][y] == 4)){
                //往上走，压入queue
                help(x-1,y,visit,qq);
            }
            if ((g[x][y]== 1 || g[x][y] == 3 || g[x][y] == 5)
                      && y-1 >= 0
                      && (g[x][y-1] == 1 || g[x][y-1] == 4 || g[x][y-1] == 6)){
                //往左走
                help(x,y-1,visit,qq);
            }
            if((g[x][y]==2 || g[x][y] == 3 || g[x][y] == 4) && x+1 < xsize && (g[x+1][y] == 2 || g[x+1][y] == 5 || g[x+1][y] == 6)){
                //往下走
                help(x+1,y,visit,qq);
            }
            if ((g[x][y]== 1 || g[x][y] == 4 || g[x][y] == 6) && y+1 < ysize && (g[x][y+1] == 1 || g[x][y+1] == 3 || g[x][y+1] == 5)){
                //往右走
                help(x,y+1,visit,qq);
            }
        }
        return false;
    }
};
```

dfs同理，用递归来做，注意要用visit数组判断是否走过

### \1392. Longest Happy Prefix

kmp来做，复习到再来回顾这道题

