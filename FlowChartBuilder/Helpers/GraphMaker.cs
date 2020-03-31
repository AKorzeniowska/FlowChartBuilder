using FlowChartBuilder.Models;
using FlowChartBuilder.Providers;
using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartBuilder.Helpers
{
    public class GraphMaker
    {
        private List<LineModel> Lines { get; set; }
        private int[,] Grid { get; set; }
        private List<INode> Nodes { get; set; }
        private List<Connection> Connections { get; set; }

        public List<LineModel> GetLines()
        {
            return this.Lines;
        }

        public List<INode> GetNodes()
        {
            return this.Nodes;
        }


        public GraphMaker(List<INode> nodes, int[,] grid)
        {
            this.Nodes = nodes;
            this.Grid = grid;
            this.Lines = new List<LineModel>();
            this.Connections = new List<Connection>();
        }

        public void LineCreator()
        {
            foreach(var node in Nodes)
            {
                if (node.GetType() == typeof(StartingNode))
                {
                    Connections.Add(new Connection(node.GetId(), (node as StartingNode).GetFollowingNodeId()));
                }
                else if (node.GetType() == typeof(ProcessNode))
                {
                    Connections.Add(new Connection(node.GetId(), (node as ProcessNode).GetFollowingNodeId()));
                }
                else if (node.GetType() == typeof(DecisionNode))
                {
                    Connections.Add(new Connection(node.GetId(), (node as DecisionNode).GetLeftFollowingNodeId()));
                    Connections.Add(new Connection(node.GetId(), (node as DecisionNode).GetRightFollowingNodeId()));
                }
            }


            //var gridCopy = Grid.Clone() as int[,];

            foreach (var connection in Connections)
            {
                var gridCopy = Grid.Clone() as int[,];
                int x1 = 0, x2 = 0, y1 = 0, y2 = 0;

                for (int i = 0; i < gridCopy.GetLength(0); i++)
                {
                    for (int j = 0; j < gridCopy.GetLength(1); j++)
                    {
                        if (gridCopy[i, j] == connection.GetFirstNodeId())
                        {
                            gridCopy[i, j] = 1;
                            x1 = i;
                            y1 = j;
                        }
                        else if (gridCopy[i, j] == connection.GetSecondNodeId())
                        {
                            gridCopy[i, j] = 2;
                            x2 = i;
                            y2 = j;
                        } 
                        else if (gridCopy[i, j] > 0)
                        {
                            gridCopy[i, j] = int.MaxValue;
                        }
                    }
                }

                LineModel newLine = null;
                int[][] moves = null;
                if (x1 > x2 && y1 >= y2)
                {
                    moves = MovesProvider.UpLeft;
                }
                else if (x1 > x2 && y1 < y2)
                {
                    moves = MovesProvider.UpRight;
                }
                else if (x1 <= x2 && y1 >= y2)
                {
                    moves = MovesProvider.DownLeft;
                }
                else if (x1 <= x2 && y1 < y2)
                {
                    moves = MovesProvider.DownRight;
                }

                newLine = LeeAlgorithmInterpreter.DoYourJob(gridCopy, moves, (Math.Abs(x1 - x2) + Math.Abs(y1 - y2)) + 4);
                newLine.AddPointAtStartOfLine(x1, y1);
                Lines.Add(newLine);

                //gridCopy = Grid.Clone() as int[,];
                //foreach (var line in Lines)
                //{
                //    for (int i = 0; i < line.GetPointsOfLine().Count - 1; i++)
                //    {
                //        gridCopy[line.GetPointsOfLine()[i].x, line.GetPointsOfLine()[i].y] = int.MaxValue;
                //    }
                //}

            }
        }

        public void RemoveMiddlePointsFromLines()
        {
            foreach (var line in this.Lines)
            {
                line.RemoveMiddlePoints();
            }
        }
    }
}
