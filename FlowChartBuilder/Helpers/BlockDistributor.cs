using FlowChartBuilder.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FlowChartBuilder.Helpers
{
    public class BlockSpawner
    {
        private int[,] Grid { get; set; }
        private List<INode> Nodes { get; set; }
        private List<int> visited { get; set; }
        private int GridHeight { get; set; }
        private int GridWidth { get; set; }
        private List<int> DecisionNodes { get; set; }

        public BlockSpawner(IEnumerable<INode> nodes)
        {
            if (!nodes.OfType<StartingNode>().Any() || nodes.OfType<StartingNode>().Count() > 1)
                return;
            if (!nodes.OfType<EndingNode>().Any() || nodes.OfType<EndingNode>().Count() > 1)
                return;

            this.Nodes = nodes.ToList();
            var numberOfDecisionNodes = nodes.OfType<DecisionNode>().Count();
            var numberOfProcessNodes = nodes.OfType<ProcessNode>().Count();

            this.GridHeight = 2 *nodes.Count();
            this.GridWidth = 2 * (numberOfDecisionNodes * 6 + 3);
            this.Grid = new int[GridHeight, GridWidth];

            var startingNode = nodes.OfType<StartingNode>().FirstOrDefault() as StartingNode;
            var xPositioner = Convert.ToInt32((GridWidth) / 4);
            var yPositioner = 0;
            this.Grid[yPositioner, xPositioner] = startingNode.GetId();
            visited = new List<int>();
            SetSubtree(yPositioner, xPositioner, startingNode, 1);

            Console.WriteLine("XD");

            this.DecisionNodes = GetLevelsWithdecisionNodes();
            this.Grid = new int[GridHeight, GridWidth];

            startingNode = nodes.OfType<StartingNode>().FirstOrDefault() as StartingNode;
            xPositioner = Convert.ToInt32((GridWidth) / 4);
            yPositioner = 0;
            this.Grid[yPositioner, xPositioner] = startingNode.GetId();
            visited = new List<int>();
            SetSubtreeSecondIteration(yPositioner, xPositioner, startingNode, 0, 1);
        }

        private void SetSubtree(int yPositioner, int xPositioner, INode root, int level)
        {
            if (visited.Contains(root.GetId()))
                return;
            visited.Add(root.GetId());

            if (root.GetType() == typeof(EndingNode)){
                //PrintGrid();
                return;
            }

            else if (root.GetType() == typeof(StartingNode))
            {
                INode nextNode = Nodes.Find(x => x.GetId() == (root as StartingNode).GetFollowingNodeId());
                if (visited.Contains(nextNode.GetId()))
                    return;
                this.Grid[yPositioner + 2, xPositioner] = nextNode.GetId();
                SetSubtree(yPositioner + 2, xPositioner, nextNode, level);
            }
            
            else if (root.GetType() == typeof(ProcessNode))
            {
                INode nextNode = Nodes.Find(x => x.GetId() == (root as ProcessNode).GetFollowingNodeId());
                if (visited.Contains(nextNode.GetId()))
                    return;
                this.Grid[yPositioner + 2, xPositioner] = nextNode.GetId();
                SetSubtree(yPositioner + 2, xPositioner, nextNode, level);
            }

            else if (root.GetType() == typeof(DecisionNode))
            {
                INode nextLeftNode = GetNodeById((root as DecisionNode).GetLeftFollowingNodeId());
                INode nextRightNode = GetNodeById((root as DecisionNode).GetRightFollowingNodeId());

                int xLeftPositioner = xPositioner - (64 / level == 64 ? 32 : 64 / level);
                int xRightPositioner = xPositioner + (64 / level == 64 ? 32 : 64 / level);
                while (this.Grid[yPositioner + 2, xLeftPositioner] != 0)
                {
                    xLeftPositioner += 1;
                }
                while (this.Grid[yPositioner + 2, xRightPositioner] != 0)
                {
                    xRightPositioner -= 1;
                }
                if (!visited.Contains(nextLeftNode.GetId()) && !visited.Contains(nextRightNode.GetId()))
                {
                    this.Grid[yPositioner + 2, xLeftPositioner] = nextLeftNode.GetId();
                    this.Grid[yPositioner + 2, xRightPositioner] = nextRightNode.GetId();
                    SetSubtree(yPositioner + 2, xRightPositioner, nextRightNode, level + 1);
                    SetSubtree(yPositioner + 2, xLeftPositioner, nextLeftNode, level + 1);
                }
                else
                {
                    if (!visited.Contains(nextRightNode.GetId()))
                    {
                        this.Grid[yPositioner + 2, xPositioner] = nextRightNode.GetId();
                        SetSubtree(yPositioner + 2, xPositioner, nextRightNode, level);
                    }
                    if (!visited.Contains(nextLeftNode.GetId()))
                    {
                        this.Grid[yPositioner + 2, xPositioner] = nextLeftNode.GetId();
                        SetSubtree(yPositioner + 2, xPositioner, nextLeftNode, level);
                    }
                }
            }
        }

        private void SetSubtreeSecondIteration(int yPositioner, int xPositioner, INode root, int level, int depth)
        {
            if (visited.Contains(root.GetId()))
                return;
            visited.Add(root.GetId());

            if (root.GetType() == typeof(EndingNode))
            {
                //PrintGrid();
                return;
            }

            else if (root.GetType() == typeof(StartingNode))
            {
                INode nextNode = Nodes.Find(x => x.GetId() == (root as StartingNode).GetFollowingNodeId());
                if (visited.Contains(nextNode.GetId()))
                    return;
                this.Grid[yPositioner + 2, xPositioner] = nextNode.GetId();
                if (this.DecisionNodes.Contains(level))
                    depth *= 2;
                SetSubtreeSecondIteration(yPositioner + 2, xPositioner, nextNode, level + 1, depth);
            }

            else if (root.GetType() == typeof(ProcessNode))
            {
                INode nextNode = Nodes.Find(x => x.GetId() == (root as ProcessNode).GetFollowingNodeId());
                if (visited.Contains(nextNode.GetId()))
                    return;
                this.Grid[yPositioner + 2, xPositioner] = nextNode.GetId();
                if (this.DecisionNodes.Contains(level))
                    depth *= 2;
                SetSubtreeSecondIteration(yPositioner + 2, xPositioner, nextNode, level + 1, depth);
            }

            else if (root.GetType() == typeof(DecisionNode))
            {
                INode nextLeftNode = GetNodeById((root as DecisionNode).GetLeftFollowingNodeId());
                INode nextRightNode = GetNodeById((root as DecisionNode).GetRightFollowingNodeId());

                int xLeftPositioner = xPositioner - (32 / depth + 1);
                int xRightPositioner = xPositioner + (32 / depth + 1);

                if (this.DecisionNodes.Contains(level))
                    depth *= 2;

                while (this.Grid[yPositioner + 2, xLeftPositioner] != 0)
                {
                    xLeftPositioner += 1;
                }
                while (this.Grid[yPositioner + 2, xRightPositioner] != 0)
                {
                    xRightPositioner -= 1;
                }
                if (!visited.Contains(nextLeftNode.GetId()) && !visited.Contains(nextRightNode.GetId()))
                {
                    //xRightPositioner += 1;
                    //xLeftPositioner -= 1;
                    this.Grid[yPositioner + 2, xLeftPositioner] = nextLeftNode.GetId();
                    this.Grid[yPositioner + 2, xRightPositioner] = nextRightNode.GetId();
                    SetSubtreeSecondIteration(yPositioner + 2, xRightPositioner, nextRightNode, level + 1, depth);
                    SetSubtreeSecondIteration(yPositioner + 2, xLeftPositioner, nextLeftNode, level + 1, depth);
                }
                else
                {
                    if (!visited.Contains(nextRightNode.GetId()))
                    {
                        this.Grid[yPositioner + 2, xPositioner] = nextRightNode.GetId();
                        SetSubtreeSecondIteration(yPositioner + 2, xPositioner, nextRightNode, level + 1, depth);
                    }
                    if (!visited.Contains(nextLeftNode.GetId()))
                    {
                        this.Grid[yPositioner + 2, xPositioner] = nextLeftNode.GetId();
                        SetSubtreeSecondIteration(yPositioner + 2, xPositioner, nextLeftNode, level + 1, depth);
                    }
                }
            }
        }

        private INode GetNodeById(int id)
        {
            return Nodes.Find(x => x.GetId() == id);
        }

        public void PrintGrid()
        {
            for (int i = 0; i < this.Grid.GetLength(0); i++)
            {
                for (int j = 0; j < this.Grid.GetLength(1); j++)
                {
                    if (this.Grid[i, j] != 0)
                        Console.Write(this.Grid[i, j].ToString());
                    else
                        Console.Write('-');
                }
                Console.WriteLine("\n");
            }

        }

        public string GetGridAsString()
        {
            string output = "";
            for (int i = 0; i < this.GridHeight; i++)
            {
                for (int j = 0; j < this.GridWidth; j++)
                {
                    if (this.Grid[i, j] != 0)
                        output += (this.Grid[i, j].ToString());
                    else
                        output += '-';
                }
                output += "\n";
            }
            return output;
        }
        public int[,] GetGrid()
        {
            return this.Grid;
        }

        private List<int> GetLevelsWithdecisionNodes()
        {
            int counter = 0;
            var decisionNodeIds = Nodes.Where(x => x.GetType() == typeof(DecisionNode)).Select(x => x.GetId());
            var levelsWithDecisionNodes = new List<int>();
            for (int i = 0; i < this.GridHeight; i++)
            {
                for (int j = 0; j < this.GridWidth; j++)
                {
                    if (decisionNodeIds.Contains(this.Grid[i, j]) && !levelsWithDecisionNodes.Contains(i))
                        counter++;
                }
                if (counter >= 2)
                {
                    levelsWithDecisionNodes.Add(i);
                }
                counter = 0;
            }
            return levelsWithDecisionNodes.Select(x => x/=2).ToList();
        }

        public void RemoveEmptyLines()
        {
            var rowsToRemove = new List<int>();
            var colsToRemove = new List<int>();

            int counter = 0;


            for (int i = 0; i < this.GridHeight; i++)
            {
                for (int j = 0; j < this.GridWidth; j++)
                {
                    if (this.Grid[i, j] == 0)
                        counter++;
                }
                if (counter == this.GridWidth)
                    rowsToRemove.Add(i);
                counter = 0;
            }

            for (int i = 0; i < this.GridWidth; i++)
            {
                for (int j = 0; j < this.GridHeight; j++)
                {
                    if (this.Grid[j, i] == 0)
                        counter++;
                }
                if (counter == this.GridHeight)
                    colsToRemove.Add(i);
                counter = 0;
            }

            //if (highRows.Count >= 4)
            //    highRows.RemoveRange(highRows.Count - 4, 3);
            //else
            //    highRows = new List<int>();
            //if (lowRows.Count >= 4)
            //    lowRows.RemoveRange(0, 3);
            //else
            //    lowRows = new List<int>();
            //if (rightCols.Count >= 4)
            //    rightCols.RemoveRange(rightCols.Count - 4, 3);
            //else
            //    rightCols = new List<int>();
            //if (leftCols.Count >= 4)
            //    leftCols.RemoveRange(0, 3);
            //else
            //    leftCols = new List<int>();

            //rowsToRemove = highRows.Union(lowRows).ToList();
            //colsToRemove = rightCols.Union(leftCols).ToList();

            var newGrid = TrimArray(rowsToRemove, colsToRemove, this.Grid);
            this.Grid = newGrid;

        }

        private int[,] TrimArray(List<int> rowsToRemove, List<int> columnsToRemove, int[,] originalArray)
        {
            int[,] result = new int[originalArray.GetLength(0) - rowsToRemove.Count, originalArray.GetLength(1) - columnsToRemove.Count];

            for (int i = 0, j = 0; i < originalArray.GetLength(0); i++)
            {
                if (rowsToRemove.Contains(i))
                    continue;

                for (int k = 0, u = 0; k < originalArray.GetLength(1); k++)
                {
                    if (columnsToRemove.Contains(k))
                        continue;

                    result[j, u] = originalArray[i, k];
                    u++;
                }
                j++;
            }

            return result;
        }

    }
}
